/*
 * wrbmp.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains routines to write output images in Microsoft "BMP"
 * format (MS Windows 3.x and OS/2 1.x flavors).
 * Either 8-bit colormapped or 24-bit full-color format can be written.
 * No compression is supported.
 *
 * These routines may need modification for non-Unix environments or
 * specialized applications.  As they stand, they assume output to
 * an ordinary stdio stream.
 *
 * This code contributed by James Arthur Boucher.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#ifndef SHEEN_VC_DEBUG
//#include "oslmemory.h"
#endif

#ifdef BMP_SUPPORTED

extern short finish_output_index;

/*
 * To support 12-bit JPEG data, we'd have to scale output down to 8 bits.
 * This is not yet implemented.
 */

#if BITS_IN_JSAMPLE != 8
  Sorry, this code only copes with 8-bit JSAMPLEs. /* deliberate syntax err */
#endif

/*
 * Since BMP stores scanlines bottom-to-top, we have to invert the image
 * from JPEG's top-to-bottom order.  To do this, we save the outgoing data
 * in a virtual array during put_pixel_row calls, then actually emit the
 * BMP file during finish_output.  The virtual array contains one JSAMPLE per
 * pixel if the output is grayscale or colormapped, three if it is full color.
 */

/* Private version of data destination object */

typedef struct {
  struct djpeg_dest_struct pub;	/* public fields */

  //boolean is_os2;		/* saves the OS2 format request flag */

  //jvirt_sarray_ptr whole_image;	/* needed to reverse row order */
  char*  whole_y_part;
  char* whole_u_part;
  char* whole_v_part;
  JDIMENSION data_width;	/* JSAMPLEs per row */
  JDIMENSION row_width;		/* physical width of one row in the BMP file */
//  int pad_bytes;		/* number of padding bytes needed per row */
  JDIMENSION cur_output_row;	/* next row# to write to virtual array */
} bmp_dest_struct;

typedef bmp_dest_struct * bmp_dest_ptr;

#undef  DRV_RGB2PIXEL565
#define DRV_RGB2PIXEL565(r,g,b) \
((( (r) & 0xf8 ) << 8) | (( (g) & 0xfc) << 3) | (( (b) & 0xf8) >> 3))

extern void (*callBackInJpgDec)();
EXTERN(long) jdiv_round_up JPP((long a, long b));

/*
 * Write some pixel data.
 * In this module rows_supplied will always be 1.
 */

METHODDEF(void)
put_pixel_rows (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo,
		JDIMENSION rows_supplied)
/* This version is for writing 24-bit pixels */
{
	bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
	register JSAMPROW inptr;
	register JDIMENSION col;
	short*  pout = (short*)cinfo->dest_buffer;
	JSAMPLE rr,gg,bb;
	int cut_col_num;//sheen

	//dest->cur_output_row++; sheen
	
	/* Transfer data.  Note destination values must be in BGR order
	* (even though Microsoft's own documents say the opposite).
	*/
	inptr = dest->pub.buffer[0];
	cut_col_num=0;//sheen
	//for (col = cinfo->output_width; col > 0; col--)  sheen
	for (col =0; col< cinfo->output_width; col++) 
	{
		if(col >= cinfo->smp_cut_col_start)//sheen
		{
			rr = *inptr++;	/* can omit GETJSAMPLE() safely */
			gg = *inptr++;
			bb = *inptr++;
			
			*pout++=DRV_RGB2PIXEL565(rr,gg,bb);
			cut_col_num++;//sheen
			if(cut_col_num >= cinfo->smp_cut_col_end-cinfo->smp_cut_col_start +1)//sheen
				break;
		}
		else
		{
			inptr+=3;
		}

	}
	
	cinfo->dest_buffer = (char*)pout;
	/* Zero out the pad bytes. */
	//  pad = dest->pad_bytes;
//  while (--pad >= 0)
//    *outptr++ = 0;
}

METHODDEF(void)
put_pixel_rows_yuv411(j_decompress_ptr cinfo, djpeg_dest_ptr dinfo,
		JDIMENSION rows_supplied)
/* This version is for writing 24-bit pixels */
{
  bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
  register JSAMPROW inptr;


  /* Transfer data.  Note destination values must be in BGR order
   * (even though Microsoft's own documents say the opposite).
   */
  inptr = dest->pub.buffer[0];
  memcpy(dest->whole_y_part,inptr,cinfo->output_width<<1);
  dest->whole_y_part+=cinfo->output_width<<1;

  inptr+=cinfo->output_width<<1;
  memcpy(dest->whole_u_part,inptr,cinfo->output_width>>1);
  dest->whole_u_part+=cinfo->output_width>>1;

  inptr+=cinfo->output_width>>1;
  memcpy(dest->whole_v_part,inptr,cinfo->output_width>>1);
  dest->whole_v_part+=cinfo->output_width>>1;
  
  /* Zero out the pad bytes. */
  //pad = dest->pad_bytes;
  //while (--pad >= 0)
  //  *outptr++ = 0;
}


METHODDEF(void)
put_gray_rows (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo,
	       JDIMENSION rows_supplied)
/* This version is for grayscale OR quantized color output */
{
//  bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
//  JSAMPARRAY image_ptr;
//  register JSAMPROW inptr, outptr;
//  register JDIMENSION col;
//  //int pad;
//
//  /* Access next row in virtual array */
//  image_ptr = (*cinfo->mem->access_virt_sarray)
//    ((j_common_ptr) cinfo, dest->whole_image,
//     dest->cur_output_row, (JDIMENSION) 1, TRUE);
//  dest->cur_output_row++;
//
//  /* Transfer data. */
//  inptr = dest->pub.buffer[0];
//  outptr = image_ptr[0];
//  for (col = cinfo->output_width; col > 0; col--) {
//    *outptr++ = *inptr++;	/* can omit GETJSAMPLE() safely */
//  }

  /* Zero out the pad bytes. */
//  pad = dest->pad_bytes;
//  while (--pad >= 0)
//    *outptr++ = 0;

	//ERREXIT(cinfo, JERR_BMP_COLORSPACE);

	//add.sheen
	bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
	register JSAMPROW inptr;
	register JDIMENSION col;
	short*  pout = (short*)cinfo->dest_buffer;
	JSAMPLE rr,gg,bb;
	int cut_col_num;//sheen
	
	/*not support yuv420 output now.sheen*/
	if(cinfo->is_yuv_format==2)
		return;
	
	//dest->cur_output_row++;sheen
	
	/* Transfer data.  Note destination values must be in BGR order
	* (even though Microsoft's own documents say the opposite).
	*/
	inptr = dest->pub.buffer[0];
	cut_col_num=0;//sheen
	//output rgb565.sheen
	//for (col = cinfo->output_width; col > 0; col--) 
	for (col =0; col< cinfo->output_width;  col++) 
	{
		if(col >= cinfo->smp_cut_col_start)//sheen
		{
			rr = gg = bb= *inptr++;	/* can omit GETJSAMPLE() safely */
			
			*pout++=DRV_RGB2PIXEL565(rr,gg,bb);

			cut_col_num++;//sheen
			if(cut_col_num >= cinfo->smp_cut_col_end-cinfo->smp_cut_col_start +1)//sheen
				break;
		}
		else
		{
			inptr++;
		}
	}
	
	cinfo->dest_buffer = (char*)pout;
	//add end.sheen
}


//output rgb565 directly
METHODDEF(void)
finish_output_bmp_rgb565 (j_decompress_ptr cinfo, djpeg_dest_ptr dinfo)
{
   JDIMENSION num_scanlines;
   int iPixelScale=8000;//about 30x(16X16) sheen
   int cut_row_num;//sheen

   cut_row_num=0;//sheen
   while (cinfo->output_scanline < cinfo->output_height) {
    num_scanlines = jpeg_read_scanlines(cinfo, dinfo->buffer,//imcu row :IDCT  YUV->RBG 
					dinfo->buffer_height);

	if(cinfo->is_yuv_format!=2 && cinfo->output_scanline > cinfo->smp_cut_row_start)//sheen
	{
		(*dinfo->put_pixel_rows) (cinfo, dinfo, num_scanlines);//output to dest buffer through tmp buffer
		cut_row_num++;
	}

	if(cut_row_num >= cinfo->smp_cut_row_end-cinfo->smp_cut_row_start +1)//over sheen
		break;
    
	if(callBackInJpgDec)//sheen
	{
		if(cinfo->output_scanline*cinfo->output_width > iPixelScale)
		{
			callBackInJpgDec();
			iPixelScale+=8000;
		}
	}

	
  }
   
}


/*
 * The module selection routine for BMP format output.
 */
//shrink and output rgb565
METHODDEF(void) finish_output_bmp_shrink_rgb565(j_decompress_ptr cinfo, djpeg_dest_ptr dinfo)
{
#define   Q15      16384
#define   Q14      8192
#define   S15      14

	JSAMPROW   ux;
	JDIMENSION x,y,nx,ny=0;
	JDIMENSION df,nf,i,j=0,k;
	short qx[2],qy[2],q[4];
	short rr,gg,bb;
	JDIMENSION width,height,bx,by;
	bmp_dest_ptr dest = (bmp_dest_ptr) dinfo;
	//register FILE * outfile = dest->pub.output_file;
	short* pout = (short*)cinfo->dest_buffer;
	JDIMENSION num_scanlines;//JSAMPARRAY image_ptr;
	int* f;
	int output_startX,output_startY;//sheen
	int output_endX;//sheen
	int cut_row_num,cut_col_num;//sheen
	char pixelBytes;//sheen
	

	width = cinfo->output_width;
       height = cinfo->output_height;
	bx = cinfo->dest_width;
	by = cinfo->dest_height;
	if(cinfo->out_color_space == JCS_GRAYSCALE)//1 byte per pixel gray,sheen
		pixelBytes=1;
	else
		pixelBytes=3;
	df=bx*pixelBytes;
	nf=df*by;
	//l = (width)*3; sheen

	output_startY=cinfo->smp_cut_row_start*height/by;//sheen
	output_startX=cinfo->smp_cut_col_start*width/bx;//sheen
	if(bx>1)
		output_endX=jdiv_round_up((long)(cinfo->smp_cut_col_end*(width-1)), (long)(bx-1));//sheen
	else
		output_endX=0;

	f = OslMalloc(bx*24);
	if(f==NULL)
		ERREXIT(cinfo, JERR_FILE_WRITE);
    
	k=(bx<<S15)/width*by/height;
	
    memset(f,0,df<<3);

	cut_row_num=0;//sheen
	
	for(y=0;y<height;y++)
	{
	
		num_scanlines = jpeg_read_scanlines(cinfo, dinfo->buffer,//imcu row :IDCT  YUV->RBG 
					dinfo->buffer_height);

		if (num_scanlines!=1) {
			ERREXIT(cinfo, JERR_FILE_WRITE);
		}

		//ny+=by;

		if(y >= output_startY)//sheen
		{
			ny+=by;
			ux = dest->pub.buffer[0];
			nx=0;
			
			if (ny>height) {
				qy[1]=(((ny-height)<<S15)/by);
				qy[0]=(Q15-qy[1]);
				
				
				for(x=0;x<width;x++) 
				{
					
					nx+=bx;
					
					if(x >= output_startX && x<=output_endX)//sheen
					{
						if (nx>width) 
						{
							qx[1]=((nx-width)<<S15)/bx;
							qx[0]=(Q15-qx[1]);
							
							
							for(i=0;i<4;i++)
								q[i]=qx[i&1]*qy[i>>1]>>S15;
							
							for(i=0;i<pixelBytes;i++)//sheen
							{
								f[j]+=(*ux)*q[0];
								f[j+pixelBytes]+=(*ux)*q[1];
								f[j+df]+=(*ux)*q[2];
								f[(j++)+df+pixelBytes]+=(*(ux++))*q[3];
							}
						}
						else for(i=0;i<pixelBytes;i++) {
							f[j+i]+=(*ux)*qy[0];
							f[j+df+i]+=(*(ux++))*qy[1];
						}
					}
					else if(x<=output_endX)
					{
						if (nx>width)
							j+=pixelBytes;
						ux+=pixelBytes;
					}
					else
					{
						break;
					}
					if (nx>=width) nx-=width;
					if (!nx) j+=pixelBytes;
				}
				
				{//0 1
					nf=0;
					cut_col_num=0;//sheen
					for (j=0;j<df;j+=pixelBytes)
					{
						if(j>= (cinfo->smp_cut_col_start*pixelBytes))
						{
							if(cinfo->out_color_space != JCS_GRAYSCALE)//sheen
							{
								rr=(((f[nf++]>>S15)*k+Q14)>>S15);
								gg=(((f[nf++]>>S15)*k+Q14)>>S15);
								bb=(((f[nf++]>>S15)*k+Q14)>>S15);
							}
							else
							{
								rr=gg=bb=(((f[nf++]>>S15)*k+Q14)>>S15);
							}
							
							//					bb = range_limit[bb];
							//					gg = range_limit[gg];
							//					rr = range_limit[rr];
							
							*pout++=DRV_RGB2PIXEL565(rr,gg,bb);
							++cut_col_num;//sheen
							if(cut_col_num >= cinfo->smp_cut_col_end-cinfo->smp_cut_col_start +1)
								break;
						}
						else
						{
							nf+=pixelBytes;
						}
					}
					memcpy(f,f+df,df*4);
					memset(f+df,0,df*4);
					j=0;
					cut_row_num++;//sheen
				}
			}
			else {
				for(x=0;x<width;x++)
				{
					
					nx+=bx;
					
					if(x >= output_startX && x<=output_endX)//sheen
					{
						if (nx>width) 
						{
							qx[1]=((nx-width)<<S15)/bx;
							qx[0]=(Q15-qx[1]);
							
							
							for(i=0;i<pixelBytes;i++)
							{
								f[j]+=(*ux)*qx[0];
								f[(j++)+pixelBytes]+=(*(ux++))*qx[1];
							}
						}
						else 
						{
							for(i=0;i<pixelBytes;i++)
							{
								f[j+i]+=((*ux)<<S15);
								ux++;
							}
						}
					}
					else if(x<=output_endX)
					{
						if (nx>width)
							j+=pixelBytes;
						ux+=pixelBytes;
					}
					else
					{
						break;
					}
					
					if (nx>=width) nx-=width;
					if (!nx) j+=pixelBytes;
				}
				if (ny<height) //0
				{
					j=0;//j-=df;
				}
				else //7
				{
					nf=0;
					cut_col_num=0;//sheen
					for (j=0;j<df;j+=pixelBytes)
					{
						if(j>= (cinfo->smp_cut_col_start*pixelBytes))
						{
							if(cinfo->out_color_space != JCS_GRAYSCALE)//sheen
							{
								rr=(((f[nf++]>>S15)*k+Q14)>>S15);
								gg=(((f[nf++]>>S15)*k+Q14)>>S15);
								bb=(((f[nf++]>>S15)*k+Q14)>>S15);
							}
							else
							{
								rr=gg=bb=(((f[nf++]>>S15)*k+Q14)>>S15);
							}
							
							//					bb = range_limit[bb];
							//					gg = range_limit[gg];
							//					rr = range_limit[rr];
							
							*pout++=DRV_RGB2PIXEL565(rr,gg,bb);
							++cut_col_num;//sheen
							if(cut_col_num >= cinfo->smp_cut_col_end-cinfo->smp_cut_col_start +1)
								break;
						}
						else
						{
							nf+=pixelBytes;
						}
					}
					j=0;
					memset(f,0,df<<3);
					cut_row_num++;//sheen
					
				}
			}
		}

		if (ny>=height) ny-=height;
		if(cut_row_num >= cinfo->smp_cut_row_end-cinfo->smp_cut_row_start +1)//over sheen
			break;
	}
	
	//fwrite(b,2,bx*by,outfile);
	//fflush(outfile);
	OslMfree(f);
	//free(b);


}

GLOBAL(djpeg_dest_ptr)
jinit_write_bmp (j_decompress_ptr cinfo)
{
  bmp_dest_ptr dest;
  JDIMENSION row_width;

  /* Create module interface object, fill in method pointers */
  dest = (bmp_dest_ptr)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  SIZEOF(bmp_dest_struct));
  //dest->pub.start_output = start_output_bmp;
  if(cinfo->is_yuv_format!=2)
  {
	  if (((cinfo->image_width/cinfo->scale_denom)==cinfo->dest_width && (cinfo->image_height/cinfo->scale_denom)==cinfo->dest_height))
	  {
         dest->pub.finish_output = finish_output_bmp_rgb565;
		 finish_output_index=2;
	  }
	  else
	  {
		 dest->pub.finish_output = finish_output_bmp_shrink_rgb565;
		 finish_output_index=1;
	  }
  }
  else
  {
	  dest->pub.finish_output=finish_output_bmp_rgb565;
	  finish_output_index=0;
  }
  

//  dest->is_os2 = is_os2;

  if (cinfo->out_color_space == JCS_GRAYSCALE) {
    dest->pub.put_pixel_rows = put_gray_rows;
  } else if (cinfo->out_color_space == JCS_RGB) {
    if (cinfo->quantize_colors)
      dest->pub.put_pixel_rows = put_gray_rows;
    else if (cinfo->is_yuv_format==2)
	  dest->pub.put_pixel_rows = put_pixel_rows_yuv411;
	else
      dest->pub.put_pixel_rows = put_pixel_rows;
  } else {
    ERREXIT(cinfo, JERR_BMP_COLORSPACE);
  }

  /* Calculate output image dimensions so we can allocate space */
  jpeg_calc_output_dimensions(cinfo);

  /* Determine width of rows in the BMP file (padded to 4-byte boundary). */
  row_width = cinfo->output_width * cinfo->output_components;
  dest->data_width = row_width;
  //while ((row_width & 3) != 0) row_width++;
  dest->row_width = row_width;
//  dest->pad_bytes = (int) (row_width - dest->data_width);

  if (cinfo->is_yuv_format==2) 
  {
//      dest->whole_y_part = (*cinfo->mem->request_virt_sarray)
//		  ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
//		  cinfo->output_width, cinfo->output_height, (JDIMENSION) 2);
//	  dest->whole_u_part = (*cinfo->mem->request_virt_sarray)
//		  ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
//		  cinfo->output_width>>1, cinfo->output_height>>1, (JDIMENSION) 1);
//	  dest->whole_v_part = (*cinfo->mem->request_virt_sarray)
//		  ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
//		  cinfo->output_width>>1, cinfo->output_height>>1, (JDIMENSION) 1);

      dest->whole_y_part = cinfo->dest_buffer;
	  //dest->whole_u_part = cinfo->dest_buffer + cinfo->image_width*cinfo->image_height;
	  //dest->whole_v_part = dest->whole_u_part + (cinfo->image_width*cinfo->image_height>>2);
	  dest->whole_u_part = cinfo->dest_bufferU;
	  dest->whole_v_part = cinfo->dest_bufferV;
	  dest->pub.buffer_height = 2;
	
  }
  else
  {
	  /* Allocate space for inversion array, prepare for write pass */
//	  dest->whole_image = (*cinfo->mem->request_virt_sarray)
//		  ((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
//		  row_width, cinfo->output_height, (JDIMENSION) 1);

	  dest->pub.buffer_height = 1;
  }

   dest->cur_output_row = 0;
  
  /* Create decompressor output buffer. */
  dest->pub.buffer = (*cinfo->mem->alloc_sarray)
	  ((j_common_ptr) cinfo, JPOOL_IMAGE, row_width, (JDIMENSION) 1);
  
 

  return (djpeg_dest_ptr) dest;
}

#endif /* BMP_SUPPORTED */
