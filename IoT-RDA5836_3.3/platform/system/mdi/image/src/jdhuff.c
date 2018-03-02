/*
 * jdhuff.c
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains Huffman entropy decoding routines.
 *
 * Much of the complexity here has to do with supporting input suspension.
 * If the data source module demands suspension, we want to be able to back
 * up to the start of the current MCU.  To do this, we copy state variables
 * into local working storage, and update them back to the permanent
 * storage only upon successful completion of an MCU.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jdhuff.h"		/* Declarations shared with jdphuff.c */

extern short decode_mcu_index;

/*
 * Expanded entropy decoder object for Huffman decoding.
 *
 * The savable_state subrecord contains fields that change within an MCU,
 * but must not be updated permanently until we complete the MCU.
 */

typedef struct {
  int last_dc_val[MAX_COMPS_IN_SCAN]; /* last DC coef for each component */
} savable_state;

/* This macro is to work around compilers with missing or broken
 * structure assignment.  You'll need to fix this code if you have
 * such a compiler and you change MAX_COMPS_IN_SCAN.
 */

#ifndef NO_STRUCT_ASSIGN
#define ASSIGN_STATE(dest,src)  ((dest) = (src))
#else
#if MAX_COMPS_IN_SCAN == 4
#define ASSIGN_STATE(dest,src)  \
	((dest).last_dc_val[0] = (src).last_dc_val[0], \
	 (dest).last_dc_val[1] = (src).last_dc_val[1], \
	 (dest).last_dc_val[2] = (src).last_dc_val[2], \
	 (dest).last_dc_val[3] = (src).last_dc_val[3])
#endif
#endif


typedef struct {
  struct jpeg_entropy_decoder pub; /* public fields */

  /* These fields are loaded into local variables at start of each MCU.
   * In case of suspension, we exit WITHOUT updating them.
   */
  bitread_perm_state bitstate;	/* Bit buffer at start of MCU */
  savable_state saved;		/* Other state at start of MCU */

  /* These fields are NOT loaded into local working state. */
  unsigned int restarts_to_go;	/* MCUs left in this restart interval */

  /* Pointers to derived tables (these workspaces have image lifespan) */
  d_derived_tbl * dc_derived_tbls[NUM_HUFF_TBLS];
  d_derived_tbl * ac_derived_tbls[NUM_HUFF_TBLS];

  /* Precalculated info set up by start_pass for use in decode_mcu: */

  /* Pointers to derived tables to be used for each block within an MCU */
  d_derived_tbl * dc_cur_tbls[D_MAX_BLOCKS_IN_MCU];
  d_derived_tbl * ac_cur_tbls[D_MAX_BLOCKS_IN_MCU];
  /* Whether we care about the DC and AC coefficient values for each block */
  boolean dc_needed[D_MAX_BLOCKS_IN_MCU];
  boolean ac_needed[D_MAX_BLOCKS_IN_MCU];
} huff_entropy_decoder;

typedef huff_entropy_decoder * huff_entropy_ptr;


/*
 * Initialize for a Huffman-compressed scan.
 */

METHODDEF(void)
start_pass_huff_decoder (j_decompress_ptr cinfo)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
  int ci, blkn, dctbl, actbl;
  jpeg_component_info * compptr;

  /* Check that the scan parameters Ss, Se, Ah/Al are OK for sequential JPEG.
   * This ought to be an error condition, but we make it a warning because
   * there are some baseline files out there with all zeroes in these bytes.
   */
  if (cinfo->Ss != 0 || cinfo->Se != DCTSIZE2-1 ||
      cinfo->Ah != 0 || cinfo->Al != 0)
    WARNMS(cinfo, JWRN_NOT_SEQUENTIAL);

  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[ci];
    dctbl = compptr->dc_tbl_no;
    actbl = compptr->ac_tbl_no;
    /* Compute derived values for Huffman tables */
    /* We may do this more than once for a table, but it's not expensive */
    jpeg_make_d_derived_tbl(cinfo, TRUE, dctbl,
			    & entropy->dc_derived_tbls[dctbl]);
    jpeg_make_d_derived_tbl(cinfo, FALSE, actbl,
			    & entropy->ac_derived_tbls[actbl]);
    /* Initialize DC predictions to 0 */
    entropy->saved.last_dc_val[ci] = 0;
  }

  /* Precalculate decoding info for each block in an MCU of this scan */
  for (blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++) {
    ci = cinfo->MCU_membership[blkn];
    compptr = cinfo->cur_comp_info[ci];
    /* Precalculate which table to use for each block */
    entropy->dc_cur_tbls[blkn] = entropy->dc_derived_tbls[compptr->dc_tbl_no];
    entropy->ac_cur_tbls[blkn] = entropy->ac_derived_tbls[compptr->ac_tbl_no];
    /* Decide whether we really care about the coefficient values */
    if (compptr->component_needed) {
      entropy->dc_needed[blkn] = TRUE;
      /* we don't need the ACs if producing a 1/8th-size image */
      entropy->ac_needed[blkn] = (compptr->DCT_scaled_size > 1);
    } else {
      entropy->dc_needed[blkn] = entropy->ac_needed[blkn] = FALSE;
    }
  }

  /* Initialize bitread state variables */
  entropy->bitstate.bits_left = 0;
  entropy->bitstate.get_buffer = 0; /* unnecessary, but keeps Purify quiet */
  entropy->pub.insufficient_data = FALSE;

  /* Initialize restart counter */
  entropy->restarts_to_go = cinfo->restart_interval;
}


/*
 * Compute the derived values for a Huffman table.
 * This routine also performs some validation checks on the table.
 *
 * Note this is also used by jdphuff.c.
 */

GLOBAL(void)
jpeg_make_d_derived_tbl (j_decompress_ptr cinfo, boolean isDC, int tblno,
			 d_derived_tbl ** pdtbl)
{
  JHUFF_TBL *htbl;
  d_derived_tbl *dtbl;
  int p, i, l, si, numsymbols;
  int lookbits, ctr;
  char huffsize[257];
  unsigned int huffcode[257];
  unsigned int code;

  /* Note that huffsize[] and huffcode[] are filled in code-length order,
   * paralleling the order of the symbols themselves in htbl->huffval[].
   */

  /* Find the input Huffman table */
  if (tblno < 0 || tblno >= NUM_HUFF_TBLS)
    ERREXIT1(cinfo, JERR_NO_HUFF_TABLE, tblno);
  htbl =
    isDC ? cinfo->dc_huff_tbl_ptrs[tblno] : cinfo->ac_huff_tbl_ptrs[tblno];
  if (htbl == NULL)
    ERREXIT1(cinfo, JERR_NO_HUFF_TABLE, tblno);

  /* Allocate a workspace if we haven't already done so. */
  if (*pdtbl == NULL)
    *pdtbl = (d_derived_tbl *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(d_derived_tbl));
  dtbl = *pdtbl;
  dtbl->pub = htbl;		/* fill in back link */
  
  /* Figure C.1: make table of Huffman code length for each symbol */

  p = 0;
  for (l = 1; l <= 16; l++) {
    i = (int) htbl->bits[l];
    if (i < 0 || p + i > 256)	/* protect against table overrun */
      ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
    while (i--)
      huffsize[p++] = (char) l;
  }
  huffsize[p] = 0;
  numsymbols = p;
  
  /* Figure C.2: generate the codes themselves */
  /* We also validate that the counts represent a legal Huffman code tree. */
  
  code = 0;
  si = huffsize[0];
  p = 0;
  while (huffsize[p]) {
    while (((int) huffsize[p]) == si) {
      huffcode[p++] = code;
      code++;
    }
    /* code is now 1 more than the last code used for codelength si; but
     * it must still fit in si bits, since no code is allowed to be all ones.
     */
    if (((INT32) code) >= (((INT32) 1) << si))
      ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
    code <<= 1;
    si++;
  }

  /* Figure F.15: generate decoding tables for bit-sequential decoding */

  p = 0;
  for (l = 1; l <= 16; l++) {
    if (htbl->bits[l]) {
      /* valoffset[l] = huffval[] index of 1st symbol of code length l,
       * minus the minimum code of length l
       */
      dtbl->valoffset[l] = (INT32) p - (INT32) huffcode[p];
      p += htbl->bits[l];
      dtbl->maxcode[l] = huffcode[p-1]; /* maximum code of length l */
    } else {
      dtbl->maxcode[l] = -1;	/* -1 if no codes of this length */
    }
  }
  dtbl->maxcode[17] = 0xFFFFFL; /* ensures jpeg_huff_decode terminates */

  /* Compute lookahead tables to speed up decoding.
   * First we set all the table entries to 0, indicating "too long";
   * then we iterate through the Huffman codes that are short enough and
   * fill in all the entries that correspond to bit sequences starting
   * with that code.
   */

  MEMZERO(dtbl->look_nbits, SIZEOF(dtbl->look_nbits));

  p = 0;
  for (l = 1; l <= HUFF_LOOKAHEAD; l++) {
    for (i = 1; i <= (int) htbl->bits[l]; i++, p++) {
      /* l = current code's length, p = its index in huffcode[] & huffval[]. */
      /* Generate left-justified code followed by all possible bit sequences */
      lookbits = huffcode[p] << (HUFF_LOOKAHEAD-l);
      for (ctr = 1 << (HUFF_LOOKAHEAD-l); ctr > 0; ctr--) {
	dtbl->look_nbits[lookbits] = l;
	dtbl->look_sym[lookbits] = htbl->huffval[p];
	lookbits++;
      }
    }
  }

  /* Validate symbols as being reasonable.
   * For AC tables, we make no check, but accept all byte values 0..255.
   * For DC tables, we require the symbols to be in range 0..15.
   * (Tighter bounds could be applied depending on the data depth and mode,
   * but this is sufficient to ensure safe decoding.)
   */
  if (isDC) {
    for (i = 0; i < numsymbols; i++) {
      int sym = htbl->huffval[i];
      if (sym < 0 || sym > 15)
	ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
    }
  }
}


/*
 * Out-of-line code for bit fetching (shared with jdphuff.c).
 * See jdhuff.h for info about usage.
 * Note: current values of get_buffer and bits_left are passed as parameters,
 * but are returned in the corresponding fields of the state struct.
 *
 * On most machines MIN_GET_BITS should be 25 to allow the full 32-bit width
 * of get_buffer to be used.  (On machines with wider words, an even larger
 * buffer could be used.)  However, on some machines 32-bit shifts are
 * quite slow and take time proportional to the number of places shifted.
 * (This is true with most PC compilers, for instance.)  In this case it may
 * be a win to set MIN_GET_BITS to the minimum value of 15.  This reduces the
 * average shift distance at the cost of more calls to jpeg_fill_bit_buffer.
 */

#ifdef SLOW_SHIFT_32
#define MIN_GET_BITS  15	/* minimum allowable value */
#else
#define MIN_GET_BITS  (BIT_BUF_SIZE-7)
#endif


GLOBAL(boolean)
jpeg_fill_bit_buffer (bitread_working_state * state,
		      register bit_buf_type get_buffer, register int bits_left,
		      int nbits)
/* Load up the bit buffer to a depth of at least nbits */
{
  /* Copy heavily used state fields into locals (hopefully registers) */
  register const JOCTET * next_input_byte = state->next_input_byte;
  register size_t bytes_in_buffer = state->bytes_in_buffer;
  j_decompress_ptr cinfo = state->cinfo;

  /* Attempt to load at least MIN_GET_BITS bits into get_buffer. */
  /* (It is assumed that no request will be for more than that many bits.) */
  /* We fail to do so only if we hit a marker or are forced to suspend. */
   
  if (cinfo->unread_marker == 0) {	/* cannot advance past a marker */
    while (bits_left < MIN_GET_BITS) {
      register int c;

      /* Attempt to read a byte */
      if (bytes_in_buffer == 0) {
	if (! (*cinfo->src->fill_input_buffer) (cinfo))
	  return FALSE;
	next_input_byte = cinfo->src->next_input_byte;
	bytes_in_buffer = cinfo->src->bytes_in_buffer;
      }
      bytes_in_buffer--;
      c = GETJOCTET(*next_input_byte++);

      /* If it's 0xFF, check and discard stuffed zero byte */
      if (c == 0xFF) {
	/* Loop here to discard any padding FF's on terminating marker,
	 * so that we can save a valid unread_marker value.  NOTE: we will
	 * accept multiple FF's followed by a 0 as meaning a single FF data
	 * byte.  This data pattern is not valid according to the standard.
	 */
	do {
	  if (bytes_in_buffer == 0) {
	    if (! (*cinfo->src->fill_input_buffer) (cinfo))
	      return FALSE;
	    next_input_byte = cinfo->src->next_input_byte;
	    bytes_in_buffer = cinfo->src->bytes_in_buffer;
	  }
	  bytes_in_buffer--;
	  c = GETJOCTET(*next_input_byte++);
	} while (c == 0xFF);

	if (c == 0) {
	  /* Found FF/00, which represents an FF data byte */
	  c = 0xFF;
	} else {
	  /* Oops, it's actually a marker indicating end of compressed data.
	   * Save the marker code for later use.
	   * Fine point: it might appear that we should save the marker into
	   * bitread working state, not straight into permanent state.  But
	   * once we have hit a marker, we cannot need to suspend within the
	   * current MCU, because we will read no more bytes from the data
	   * source.  So it is OK to update permanent state right away.
	   */
	  cinfo->unread_marker = c;
	  /* See if we need to insert some fake zero bits. */
	  goto no_more_bytes;
	}
      }

      /* OK, load c into get_buffer */
      get_buffer = (get_buffer << 8) | c;
      bits_left += 8;
    } /* end while */
  } else {
  no_more_bytes:
    /* We get here if we've read the marker that terminates the compressed
     * data segment.  There should be enough bits in the buffer register
     * to satisfy the request; if so, no problem.
     */
    if (nbits > bits_left) {
      /* Uh-oh.  Report corrupted data to user and stuff zeroes into
       * the data stream, so that we can produce some kind of image.
       * We use a nonvolatile flag to ensure that only one warning message
       * appears per data segment.
       */
      if (! cinfo->entropy->insufficient_data) {
	WARNMS(cinfo, JWRN_HIT_MARKER);
	cinfo->entropy->insufficient_data = TRUE;
      }
      /* Fill the buffer with zero bits */
      get_buffer <<= MIN_GET_BITS - bits_left;
      bits_left = MIN_GET_BITS;
    }
  }

  /* Unload the local registers */
  state->next_input_byte = next_input_byte;
  state->bytes_in_buffer = bytes_in_buffer;
  state->get_buffer = get_buffer;
  state->bits_left = bits_left;

  return TRUE;
}


/*
 * Out-of-line code for Huffman code decoding.
 * See jdhuff.h for info about usage.
 */

GLOBAL(int)
jpeg_huff_decode (bitread_working_state * state,
		  register bit_buf_type get_buffer, register int bits_left,
		  d_derived_tbl * htbl, int min_bits)
{
  register int l = min_bits;
  register INT32 code;

  /* HUFF_DECODE has determined that the code is at least min_bits */
  /* bits long, so fetch that many bits in one swoop. */

  CHECK_BIT_BUFFER(*state, l, return -1);
  code = GET_BITS(l);

  /* Collect the rest of the Huffman code one bit at a time. */
  /* This is per Figure F.16 in the JPEG spec. */

  while (code > htbl->maxcode[l]) {
    code <<= 1;
    CHECK_BIT_BUFFER(*state, 1, return -1);
    code |= GET_BITS(1);
    l++;
  }

  /* Unload the local registers */
  state->get_buffer = get_buffer;
  state->bits_left = bits_left;

  /* With garbage input we may reach the sentinel value l = 17. */

  if (l > 16) {
    WARNMS(state->cinfo, JWRN_HUFF_BAD_CODE);
    return 0;			/* fake a zero as the safest result */
  }

  return htbl->pub->huffval[ (int) (code + htbl->valoffset[l]) ];
}


/*
 * Figure F.12: extend sign bit.
 * On some machines, a shift and add will be faster than a table lookup.
 */

#ifdef AVOID_TABLES

#define HUFF_EXTEND(x,s)  ((x) < (1<<((s)-1)) ? (x) + (((-1)<<(s)) + 1) : (x))

#else

#define HUFF_EXTEND(x,s)  ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))

static const int extend_test[16] =   /* entry n is 2**(n-1) */
  { 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000 };

static const int extend_offset[16] = /* entry n is (-1 << n) + 1 */
  { 0, ((-1)<<1) + 1, ((-1)<<2) + 1, ((-1)<<3) + 1, ((-1)<<4) + 1,
    ((-1)<<5) + 1, ((-1)<<6) + 1, ((-1)<<7) + 1, ((-1)<<8) + 1,
    ((-1)<<9) + 1, ((-1)<<10) + 1, ((-1)<<11) + 1, ((-1)<<12) + 1,
    ((-1)<<13) + 1, ((-1)<<14) + 1, ((-1)<<15) + 1 };

#endif /* AVOID_TABLES */


/*
 * Check for a restart marker & resynchronize decoder.
 * Returns FALSE if must suspend.
 */

LOCAL(boolean)
process_restart (j_decompress_ptr cinfo)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
  int ci;

  /* Throw away any unused bits remaining in bit buffer; */
  /* include any full bytes in next_marker's count of discarded bytes */
  cinfo->marker->discarded_bytes += entropy->bitstate.bits_left / 8;
  entropy->bitstate.bits_left = 0;

  /* Advance past the RSTn marker */
  if (! (*cinfo->marker->read_restart_marker) (cinfo))
    return FALSE;

  /* Re-initialize DC predictions to 0 */
  for (ci = 0; ci < cinfo->comps_in_scan; ci++)
    entropy->saved.last_dc_val[ci] = 0;

  /* Reset restart counter */
  entropy->restarts_to_go = cinfo->restart_interval;

  /* Reset out-of-data flag, unless read_restart_marker left us smack up
   * against a marker.  In that case we will end up treating the next data
   * segment as empty, and we can avoid producing bogus output pixels by
   * leaving the flag set.
   */
  if (cinfo->unread_marker == 0)
    entropy->pub.insufficient_data = FALSE;

  return TRUE;
}


/*
 * Decode and return one MCU's worth of Huffman-compressed coefficients.
 * The coefficients are reordered from zigzag order into natural array order,
 * but are not dequantized.
 *
 * The i'th block of the MCU is stored into the block pointed to by
 * MCU_data[i].  WE ASSUME THIS AREA HAS BEEN ZEROED BY THE CALLER.
 * (Wholesale zeroing is usually a little faster than retail...)
 *
 * Returns FALSE if data source requested suspension.  In that case no
 * changes have been made to permanent state.  (Exception: some output
 * coefficients may already have been assigned.  This is harmless for
 * this module, since we'll just re-assign them on the next call.)
 */

METHODDEF(boolean)
decode_mcu (j_decompress_ptr cinfo, JBLOCKROW *MCU_data)
{
  huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
  int blkn;
  BITREAD_STATE_VARS;
  savable_state state;

  /* Process restart marker if needed; may have to suspend */
  if (cinfo->restart_interval) {
    if (entropy->restarts_to_go == 0)
      if (! process_restart(cinfo))
	return FALSE;
  }

  /* If we've run out of data, just leave the MCU set to zeroes.
   * This way, we return uniform gray for the remainder of the segment.
   */
  if (! entropy->pub.insufficient_data) {

    /* Load up working state */
    BITREAD_LOAD_STATE(cinfo,entropy->bitstate);
    ASSIGN_STATE(state, entropy->saved);

    /* Outer loop handles each block in the MCU */

    for (blkn = 0; blkn < cinfo->blocks_in_MCU; blkn++) {
      JBLOCKROW block = MCU_data[blkn];
      d_derived_tbl * dctbl = entropy->dc_cur_tbls[blkn];
      d_derived_tbl * actbl = entropy->ac_cur_tbls[blkn];
#ifndef SHEEN_VC_DEBUG
      register int s, k, r;
#else
		int s, k, r;
#endif

      /* Decode a single block's worth of coefficients */

      /* Section F.2.2.1: decode the DC coefficient difference */
#ifndef SHEEN_VC_DEBUG
      HUFF_DECODE(s, br_state, dctbl, return FALSE, label1);
#else//for debug sheen
		HUFF_DECODE2(&s, &br_state, dctbl, 0, 0,&bits_left,&get_buffer);
#endif
      if (s) {
	CHECK_BIT_BUFFER(br_state, s, return FALSE);
	r = GET_BITS(s);
	s = HUFF_EXTEND(r, s);
      }

      if (entropy->dc_needed[blkn]) {
	/* Convert DC difference to actual value, update last_dc_val */
	int ci = cinfo->MCU_membership[blkn];
	s += state.last_dc_val[ci];
	state.last_dc_val[ci] = s;
	/* Output the DC coefficient (assumes jpeg_natural_order[0] = 0) */
	(*block)[0] = (JCOEF) s;
      }

      if (entropy->ac_needed[blkn] &&
		  cinfo->input_iMCU_row >= cinfo->MCU_cut_row_start && //sheen
		  cinfo->input_iMCU_row <= cinfo->MCU_cut_row_end && //sheen
		  cinfo->MCU_cur_col_pos >= cinfo->MCU_cut_col_start &&//sheen
		  cinfo->MCU_cur_col_pos <= cinfo->MCU_cut_col_end //sheen
		  ) {

	/* Section F.2.2.2: decode the AC coefficients */
	/* Since zeroes are skipped, output area must be cleared beforehand */
	for (k = 1; k < DCTSIZE2; k++) {
#ifndef SHEEN_VC_DEBUG
	  HUFF_DECODE(s, br_state, actbl, return FALSE, label2);
#else//for debug sheen
	  HUFF_DECODE2(&s, &br_state, actbl, 0, 0,&bits_left,&get_buffer);
#endif
      
	  r = s >> 4;
	  s &= 15;
      
	  if (s) {
	    k += r;
	    CHECK_BIT_BUFFER(br_state, s, return FALSE);
	    r = GET_BITS(s);
	    s = HUFF_EXTEND(r, s);
	    /* Output coefficient in natural (dezigzagged) order.
	     * Note: the extra entries in jpeg_natural_order[] will save us
	     * if k >= DCTSIZE2, which could happen if the data is corrupted.
	     */
	    (*block)[jpeg_natural_order[k]] = (JCOEF) s;
	  } else {
	    if (r != 15)
	      break;
	    k += 15;
	  }
	}

      } else {

	/* Section F.2.2.2: decode the AC coefficients */
	/* In this path we just discard the values */
	for (k = 1; k < DCTSIZE2; k++) {
#ifndef SHEEN_VC_DEBUG
	  HUFF_DECODE(s, br_state, actbl, return FALSE, label3);
#else//for debug sheen
	  HUFF_DECODE2(&s, &br_state, actbl, 0, 0,&bits_left,&get_buffer);
#endif
      
	  r = s >> 4;
	  s &= 15;
      
	  if (s) {
	    k += r;
	    CHECK_BIT_BUFFER(br_state, s, return FALSE);
	    DROP_BITS(s);
	  } else {
	    if (r != 15)
	      break;
	    k += 15;
	  }
	}

      }
    }

    /* Completed MCU, so update state */
    BITREAD_SAVE_STATE(cinfo,entropy->bitstate);
    ASSIGN_STATE(entropy->saved, state);
  }

  /* Account for restart interval (no-op if not using restarts) */
  entropy->restarts_to_go--;

  return TRUE;
}


/*
 * Module initialization routine for Huffman entropy decoding.
 */

GLOBAL(void)
jinit_huff_decoder (j_decompress_ptr cinfo)
{
  huff_entropy_ptr entropy;
  int i;

  entropy = (huff_entropy_ptr)
    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(huff_entropy_decoder));
  cinfo->entropy = (struct jpeg_entropy_decoder *) entropy;
  entropy->pub.start_pass = start_pass_huff_decoder;
  entropy->pub.decode_mcu = decode_mcu;
  decode_mcu_index=0;

  /* Mark tables unallocated */
  for (i = 0; i < NUM_HUFF_TBLS; i++) {
    entropy->dc_derived_tbls[i] = entropy->ac_derived_tbls[i] = NULL;
  }
}

#if defined(MEDIA_VOCVID_SUPPORT)
#include "hal_voc.h"
#include "vpp_jpeg_dec2_asm.h"
#ifdef SHEEN_VC_DEBUG
#include "voc2_library.h"
#include "voc2_define.h"
#include "voc2_simulator.h"
#endif
#endif

//for voc function index
short finish_output_index;
short process_data_index;
short decompress_data_index;
short decode_mcu_index;
short post_process_data_index;
extern const int jpeg_natural_order[];

#if defined(MEDIA_VOCVID_SUPPORT)
void jinit_voc_decoder(j_decompress_ptr cinfo)
{
	int temp;
	int i;
	huff_entropy_ptr entropy = (huff_entropy_ptr) cinfo->entropy;
	//my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
	int * quantptr;
	unsigned char *pbsptr;


	//bs ptr int align.
	//in mcu only ff00 no ff follow marks except eoi ffd9
	temp= (int)entropy->bitstate.bits_left>>3;//bytes in get_buffer
	pbsptr= (unsigned char*)cinfo->src->next_input_byte;
	if(temp>0)
	{
		while(temp>0)
		{
			//replace 0xff
			while((entropy->bitstate.get_buffer&0xff) != *(pbsptr- 1))
			{
				*(pbsptr-1)= (unsigned char)(entropy->bitstate.get_buffer&0xff);//write back for voc
				pbsptr-= 1;
			}
			temp--;
			entropy->bitstate.get_buffer>>=8;
			pbsptr-= 1;
		}
	}

	entropy->bitstate.bits_left &=0x3;
	temp= 4- ((int)pbsptr&0x3);
	if(temp !=4)
	{
		while(temp>0)
		{
			entropy->bitstate.get_buffer<<=8;
			entropy->bitstate.get_buffer |= *pbsptr;
			pbsptr+=1;
			entropy->bitstate.bits_left +=8;
			temp--;
		}
	}
	cinfo->src->next_input_byte= pbsptr;
	//bs align end

	//memset(hal_VocGetPointer(JPEG_GX_bs_cache),0,284);

	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_bs_cache)))= (INT32)entropy->bitstate.get_buffer;
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_bs_buf_extaddr)))= (INT32)cinfo->src->next_input_byte&VOC_CFG_DMA_EADDR_MASK;
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_bs_bitcnt)))= (INT32)(entropy->bitstate.bits_left|(cinfo->output_height<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dest_buffer_EXT)))= (INT32)cinfo->dest_buffer&VOC_CFG_DMA_EADDR_MASK;
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dest_bufferU_EXT)))= (INT32)cinfo->dest_bufferU&VOC_CFG_DMA_EADDR_MASK;
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dest_bufferV_EXT)))= (INT32)cinfo->dest_bufferV&VOC_CFG_DMA_EADDR_MASK;
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_width)))= (INT32)(cinfo->image_width|(cinfo->image_height<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dest_width)))= (INT32)(cinfo->dest_width|(cinfo->dest_height<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_vert_offset)))= (INT32)(0|(1<<16));//(INT32)(coef->MCU_vert_offset|(coef->MCU_rows_per_iMCU_row<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_ctr)))= (INT32)(0|(cinfo->MCUs_per_row<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_total_iMCU_rows)))= (INT32)(cinfo->total_iMCU_rows|(cinfo->blocks_in_MCU<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_comps_in_scan)))= (INT32)(cinfo->comps_in_scan|(cinfo->restart_interval<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_restarts_to_go)))= (INT32)(entropy->restarts_to_go|(entropy->pub.insufficient_data<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_cut_row_start)))= (INT32)(cinfo->MCU_cut_row_start|(cinfo->MCU_cut_col_start<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_cut_row_end)))= (INT32)(cinfo->MCU_cut_row_end|(cinfo->MCU_cut_col_end<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_component_needed)))= (INT32)(cinfo->cur_comp_info[0]->component_needed|(cinfo->cur_comp_info[1]->component_needed<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_component_needed+2)))= (INT32)(cinfo->cur_comp_info[2]->component_needed|(cinfo->cur_comp_info[0]->MCU_blocks<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_blocks+1)))= (INT32)(cinfo->cur_comp_info[1]->MCU_blocks|(cinfo->cur_comp_info[2]->MCU_blocks<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_DCT_scaled_size)))= (INT32)(cinfo->cur_comp_info[0]->DCT_scaled_size|(cinfo->cur_comp_info[1]->DCT_scaled_size<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_DCT_scaled_size+2)))= (INT32)(cinfo->cur_comp_info[2]->DCT_scaled_size|(cinfo->cur_comp_info[0]->MCU_width<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_width+1)))= (INT32)(cinfo->cur_comp_info[1]->MCU_width|(cinfo->cur_comp_info[2]->MCU_width<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_height)))= (INT32)(cinfo->cur_comp_info[0]->MCU_height|(cinfo->cur_comp_info[1]->MCU_height<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_height+2)))= (INT32)(cinfo->cur_comp_info[2]->MCU_height|(cinfo->cur_comp_info[0]->last_col_width<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_last_col_width+1)))= (INT32)(cinfo->cur_comp_info[1]->last_col_width|(cinfo->cur_comp_info[2]->last_col_width<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_last_row_height)))= (INT32)(cinfo->cur_comp_info[0]->last_row_height|(cinfo->cur_comp_info[1]->last_row_height<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_last_row_height+2)))= (INT32)(cinfo->cur_comp_info[2]->last_row_height|(cinfo->cur_comp_info[0]->MCU_sample_width<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_sample_width+1)))= (INT32)(cinfo->cur_comp_info[1]->MCU_sample_width|(cinfo->cur_comp_info[2]->MCU_sample_width<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_width_in_blocks)))= (INT32)(cinfo->cur_comp_info[0]->width_in_blocks|(cinfo->cur_comp_info[1]->width_in_blocks<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_width_in_blocks+2)))= (INT32)(cinfo->cur_comp_info[2]->width_in_blocks|(cinfo->cur_comp_info[0]->v_samp_factor<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_v_samp_factor+1)))= (INT32)(cinfo->cur_comp_info[1]->v_samp_factor|(cinfo->cur_comp_info[2]->v_samp_factor<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_h_samp_factor)))= (INT32)(cinfo->cur_comp_info[0]->h_samp_factor|(cinfo->cur_comp_info[1]->h_samp_factor<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_h_samp_factor+2)))= (INT32)(cinfo->cur_comp_info[2]->h_samp_factor);
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dc_tbl_no)))= (INT32)(cinfo->cur_comp_info[0]->dc_tbl_no|(cinfo->cur_comp_info[1]->dc_tbl_no<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dc_tbl_no+2)))= (INT32)(cinfo->cur_comp_info[2]->dc_tbl_no|(cinfo->cur_comp_info[0]->ac_tbl_no<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_ac_tbl_no+1)))= (INT32)(cinfo->cur_comp_info[1]->h_samp_factor|(cinfo->cur_comp_info[1]->h_samp_factor<<16));
	for(i=0;i<5;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_MCU_membership+2*i)))= (INT32)(cinfo->MCU_membership[2*i]|(cinfo->MCU_membership[2*i+1]<<16));
	for(i=0;i<5;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_dc_needed+2*i)))= (INT32)(entropy->dc_needed[2*i]|(entropy->dc_needed[2*i+1]<<16));
	for(i=0;i<5;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_ac_needed+2*i)))= (INT32)(entropy->ac_needed[2*i]|(entropy->ac_needed[2*i+1]<<16));

	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_finish_output)))= (INT32)(finish_output_index|(process_data_index<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_decompress_data)))= (INT32)(decompress_data_index|(decode_mcu_index<<16));
	*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GX_post_process_data)))= (INT32)(post_process_data_index);

	//table
	for(i=0;i<18;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_maxcode0+2*i)))= (INT32)(entropy->dc_derived_tbls[0]->maxcode[i]);
	for(i=0;i<17;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_valoffset0+2*i)))= (INT32)(entropy->dc_derived_tbls[0]->valoffset[i]);
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_look_nbits0+2*i)))=
			(INT32)(entropy->dc_derived_tbls[0]->look_nbits[4*i]|(entropy->dc_derived_tbls[0]->look_nbits[4*i+1]<<8)|
			(entropy->dc_derived_tbls[0]->look_nbits[4*i+2]<<16)|(entropy->dc_derived_tbls[0]->look_nbits[4*i+3]<<24));
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_look_sym0+2*i)))=
			(INT32)(entropy->dc_derived_tbls[0]->look_sym[4*i]|(entropy->dc_derived_tbls[0]->look_sym[4*i+1]<<8)|
			(entropy->dc_derived_tbls[0]->look_sym[4*i+2]<<16)|(entropy->dc_derived_tbls[0]->look_sym[4*i+3]<<24));

	for(i=0;i<18;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_maxcode1+2*i)))= (INT32)(entropy->dc_derived_tbls[1]->maxcode[i]);
	for(i=0;i<17;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_valoffset1+2*i)))= (INT32)(entropy->dc_derived_tbls[1]->valoffset[i]);
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_look_nbits1+2*i)))=
		(INT32)(entropy->dc_derived_tbls[1]->look_nbits[4*i]|(entropy->dc_derived_tbls[1]->look_nbits[4*i+1]<<8)|
		(entropy->dc_derived_tbls[1]->look_nbits[4*i+2]<<16)|(entropy->dc_derived_tbls[1]->look_nbits[4*i+3]<<24));
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_look_sym1+2*i)))=
		(INT32)(entropy->dc_derived_tbls[1]->look_sym[4*i]|(entropy->dc_derived_tbls[1]->look_sym[4*i+1]<<8)|
		(entropy->dc_derived_tbls[1]->look_sym[4*i+2]<<16)|(entropy->dc_derived_tbls[1]->look_sym[4*i+3]<<24));

	for(i=0;i<18;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_maxcode0+2*i)))= (INT32)(entropy->ac_derived_tbls[0]->maxcode[i]);
	for(i=0;i<17;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_valoffset0+2*i)))= (INT32)(entropy->ac_derived_tbls[0]->valoffset[i]);
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_look_nbits0+2*i)))=
		(INT32)(entropy->ac_derived_tbls[0]->look_nbits[4*i]|(entropy->ac_derived_tbls[0]->look_nbits[4*i+1]<<8)|
		(entropy->ac_derived_tbls[0]->look_nbits[4*i+2]<<16)|(entropy->ac_derived_tbls[0]->look_nbits[4*i+3]<<24));
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_look_sym0+2*i)))=
		(INT32)(entropy->ac_derived_tbls[0]->look_sym[4*i]|(entropy->ac_derived_tbls[0]->look_sym[4*i+1]<<8)|
		(entropy->ac_derived_tbls[0]->look_sym[4*i+2]<<16)|(entropy->ac_derived_tbls[0]->look_sym[4*i+3]<<24));

	for(i=0;i<18;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_maxcode1+2*i)))= (INT32)(entropy->ac_derived_tbls[1]->maxcode[i]);
	for(i=0;i<17;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_valoffset1+2*i)))= (INT32)(entropy->ac_derived_tbls[1]->valoffset[i]);
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_look_nbits1+2*i)))=
		(INT32)(entropy->ac_derived_tbls[1]->look_nbits[4*i]|(entropy->ac_derived_tbls[1]->look_nbits[4*i+1]<<8)|
		(entropy->ac_derived_tbls[1]->look_nbits[4*i+2]<<16)|(entropy->ac_derived_tbls[1]->look_nbits[4*i+3]<<24));
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_look_sym1+2*i)))=
		(INT32)(entropy->ac_derived_tbls[1]->look_sym[4*i]|(entropy->ac_derived_tbls[1]->look_sym[4*i+1]<<8)|
		(entropy->ac_derived_tbls[1]->look_sym[4*i+2]<<16)|(entropy->ac_derived_tbls[1]->look_sym[4*i+3]<<24));

	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_huffval0+2*i)))=
		(INT32)(cinfo->dc_huff_tbl_ptrs[0]->huffval[4*i]|(cinfo->dc_huff_tbl_ptrs[0]->huffval[4*i+1]<<8)|
		(cinfo->dc_huff_tbl_ptrs[0]->huffval[4*i+2]<<16)|(cinfo->dc_huff_tbl_ptrs[0]->huffval[4*i+3]<<24));

	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_dc_huffval1+2*i)))=
		(INT32)(cinfo->dc_huff_tbl_ptrs[1]->huffval[4*i]|(cinfo->dc_huff_tbl_ptrs[1]->huffval[4*i+1]<<8)|
		(cinfo->dc_huff_tbl_ptrs[1]->huffval[4*i+2]<<16)|(cinfo->dc_huff_tbl_ptrs[1]->huffval[4*i+3]<<24));
	
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_huffval0+2*i)))=
		(INT32)(cinfo->ac_huff_tbl_ptrs[0]->huffval[4*i]|(cinfo->ac_huff_tbl_ptrs[0]->huffval[4*i+1]<<8)|
		(cinfo->ac_huff_tbl_ptrs[0]->huffval[4*i+2]<<16)|(cinfo->ac_huff_tbl_ptrs[0]->huffval[4*i+3]<<24));
	
	for(i=0;i<64;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_ac_huffval1+2*i)))=
		(INT32)(cinfo->ac_huff_tbl_ptrs[1]->huffval[4*i]|(cinfo->ac_huff_tbl_ptrs[1]->huffval[4*i+1]<<8)|
		(cinfo->ac_huff_tbl_ptrs[1]->huffval[4*i+2]<<16)|(cinfo->ac_huff_tbl_ptrs[1]->huffval[4*i+3]<<24));

	for(i=0;i<32;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_jpeg_natural_order+2*i)))=
			(INT32)(jpeg_natural_order[2*i]|(jpeg_natural_order[2*i+1]<<16));

	quantptr= cinfo->cur_comp_info[0]->dct_table;
	for(i=0;i<32;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_iquant_dct_tableY+2*i)))=
		(INT32)(quantptr[2*i]|(quantptr[2*i+1]<<16));
	
	quantptr= cinfo->cur_comp_info[1]->dct_table;
	for(i=0;i<32;i++)
		*((INT32*) NoCacheAddr(hal_VocGetPointer(JPEG_GY_iquant_dct_tableUV+2*i)))=
		(INT32)(quantptr[2*i]|(quantptr[2*i+1]<<16));
	//table end
}
#endif



