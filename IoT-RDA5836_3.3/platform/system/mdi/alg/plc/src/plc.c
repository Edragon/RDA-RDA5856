/* 
 * Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


/////////////////////////////////////////////////////////////////////////
//Packets Loss Concealment                                             //
/////////////////////////////////////////////////////////////////////////
#include "plc.h"
#include "cs_types.h"
#include "cos.h"

/////////////////////////////////////////////////////////
//quality set                                          //
/////////////////////////////////////////////////////////
#define FINE_PLC_PITCH_SEARCH
#define DATA_FOR_FIND_PITCH_SHIFT (3)


/////////////////////////////////////////////////////////
//basic set                                          //
/////////////////////////////////////////////////////////
#define PITCH_MIN 40 /* minimum allowed pitch, 200 Hz */
#define PITCH_MAX 120 /* maximum allowed pitch, 66 Hz */
#define PITCHDIFF (PITCH_MAX - PITCH_MIN)
#define POVERLAPMAX (PITCH_MAX >> 2)/* maximum pitch OLA window */
#define HISTORYLEN (PITCH_MAX * 3 + POVERLAPMAX) /* history buffer length*/
#define NDEC 2 /* 2:1 decimation */
#define CORRLEN 160 /* 20 ms correlation length */
#define CORRBUFLEN (CORRLEN + PITCH_MAX) /* correlation buffer length */
//#define CORRMINPOWER ((Float)250.) /* minimum power */
#define CORRMINPOWER (250) /* minimum power */
#define EOVERLAPINCR 32 /* end OLA increment per frame, 4 ms */
//#define FRAMESZ 80 /* 10 ms at 8 KHz */
//#define FRAMESZ 160 /* 20 ms at 8 KHz */
#define FRAMESZ 60 /*3.25 ms at 8 KHz */
//#define ATTENFAC ((Float).2) /* attenuation factor per 10 ms frame */
#define ATTENFAC (FIXED_Q/5) /* attenuation factor per 10 ms frame */
//#define ATTENINCR (ATTENFAC/FRAMESZ) /* attenuation per sample */
#define ATTENINCR (ATTENFAC / FRAMESZ) /* attenuation per sample */
#define FIXED_Q (1<<12)
#define FIXED_SQRT_STEP 16

#define DATA_SHIFT(x,n) (x>>n)
int erasecnt; /* consecutive erased frames */
int poverlap; /* overlap based on pitch */
int poffset; /* offset into pitch period */
int pitch; /* pitch estimate */
int pitchblen; /* current pitch buffer length */
//Float *pitchbufend; /* end of pitch buffer */
//Float *pitchbufstart; /* start of pitch buffer */
//Float pitchbuf[HISTORYLEN]; /* buffer for cycles of speech */
//Float lastq[POVERLAPMAX]; /* saved last quarter wavelength */
int *pitchbufend; /* end of pitch buffer */
int *pitchbufstart; /* start of pitch buffer */

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
extern uint8 AudioOverlayBuf[400*2];
#define g_deviceLinkSet_3 0x81c02070  /*waring: g_deviceLinkSet only be used when not ble connet.  g_deviceLinkSet have 8 buffer, 3 - 8 notused addr:0x81c02070 size:2020 byte*/
#endif

#define PLC_MALLOC_MEM

#ifdef PLC_MALLOC_MEM

#ifndef mmc_MemMalloc_BssRam
 #define  mmc_MemMalloc_BssRam(size) COS_Malloc_NoCache((UINT32)size)
 #define mmc_MemFreeAll_BssRam(p)      COS_Free((PVOID)p)
#endif
 
int *pitchbuf = NULL;
int *lastq = NULL;
short *history = NULL;
#else
int pitchbuf[HISTORYLEN]; /* buffer for cycles of speech */
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
int lastq[POVERLAPMAX]; /* saved last quarter wavelength */
short history[HISTORYLEN]; /* history buffer */
#else
int lastq[POVERLAPMAX]; /* saved last quarter wavelength */
short *history = (short *)AudioOverlayBuf; /*share from at_calib */
#endif
#endif

int gFrameLen = 0;
int gAttenIncr = 0;

#if 0 
int fix_sqrt(__int64 x,int Fixstep)    
{
	int bit = 7;
	int i,z,Sn;
	__int64 Xn,Yn,Xn1,Yn1;
	__int64 ComHalf = 1;
	x = x*64; 
	while(x >= (ComHalf<<bit))
		bit = bit + 1;
	bit = bit -1;
	if( bit&0x1)
		bit = bit+1;
	if(x == (ComHalf<<bit))
		z = ComHalf<<(bit/2-3);
	else
	{
		Xn = x + (ComHalf<<bit);
		Yn = x - (ComHalf<<bit);
		if(Yn == 0)
			Sn = 0;
		else if(Yn>0)
			Sn = -1;
		else
			Sn = 1;
		for (i = 1;i<=Fixstep;i++)
		{
			Xn1 = Xn + ((Sn*Yn)>>i);
			Yn1 = Yn + ((Sn*Xn)>>i);
			Xn = Xn1;
			Yn = Yn1;
			if(Yn == 0)
				Sn = 0;
			else if(Yn>0)
				Sn = -1;
			else
				Sn = 1;
		}
		Xn1 = Xn1*1234;
		z = Xn1>>(11+bit/2+2);
		if(z&0x1)
			z = (z>>1) + 1;
		else
			z = z>>1;
	}
	return z;
}
#else
unsigned short fix_sqrt(unsigned long a,int Fixstep)
{
	unsigned long rem = 0;
	unsigned long root = 0;
	unsigned long divisor = 0;
	int i = 0;
	for( i=0; i<16; i++){
		root <<= 1;
		rem = ((rem << 2) + (a >> 30));
		a <<= 2;
		divisor = (root<<1) + 1;
		if(divisor <= rem){
			rem -= divisor;
			root++;
		}
	}
	return (unsigned short)(root);
}
#endif

void convertsf(short *f, int *t, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++)
		t[i] = f[i];
}

void convertfs(int *f, short *t, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++)
		t[i] = (short)f[i];
}

void copyf(int *f, int *t, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++)
		t[i] = f[i];
}

void copys(short *f, short *t, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++)
		t[i] = f[i];
}

void zeros(short *s, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++)
	s[i] = 0;
}
void PlcInit(int frameLen)
{	   
	gFrameLen = frameLen;
	gAttenIncr = ATTENFAC/frameLen;
	erasecnt = 0;
#ifdef PLC_MALLOC_MEM
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
       if(pitchbuf == NULL)
       {
		pitchbuf = (int *)mmc_MemMalloc_BssRam(HISTORYLEN*sizeof(int));
       }
	if(lastq == NULL)
	{
		lastq = (int *)mmc_MemMalloc_BssRam(POVERLAPMAX*sizeof(int));
	}
	if(history == NULL)
	{
		history = (short *)mmc_MemMalloc_BssRam(HISTORYLEN*sizeof(short));	
	}
#else
	pitchbuf = (int *)(g_deviceLinkSet_3);
	lastq = (int *)((int *)g_deviceLinkSet_3 + HISTORYLEN);
	history = (short *)AudioOverlayBuf;
#endif

#endif
	pitchbufend = pitchbuf + HISTORYLEN;
	zeros(history, HISTORYLEN);
}
void PlcDeInit(void)
{
#ifdef PLC_MALLOC_MEM
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
       if(pitchbuf != NULL)
   	{
   		mmc_MemFreeAll_BssRam(pitchbuf);
		pitchbuf = NULL;
   	}
	if(lastq != NULL)
	{
   		mmc_MemFreeAll_BssRam(lastq);
		lastq = NULL;	}
	if(history != NULL)
	{
   		mmc_MemFreeAll_BssRam(history);
		history = NULL;	
	}
#else
	pitchbuf = NULL;
	lastq = NULL;
	history = NULL;
#endif
#endif
}

/*
* Save a frames worth of new speech in the history buffer.
* Return the output speech delayed by POVERLAPMAX.
*/
void savespeech(short *s)
{
	/* make room for new signal */
	copys(&history[gFrameLen], history, HISTORYLEN - gFrameLen);
	/* copy in the new frame */
	copys(s, &history[HISTORYLEN - gFrameLen], gFrameLen);
	/* copy out the delayed frame */
	copys(&history[HISTORYLEN - gFrameLen - POVERLAPMAX], s, gFrameLen);
}

/*
* A good frame was received and decoded.
* If right after an erasure, do an overlap add with the synthetic signal.
* Add the frame to history buffer.
*/

void addtohistory(short *s)
{
	if (erasecnt) {
		short overlapbuf[FRAMESZ];
		/*
		* longer erasures require longer overlaps
		* to smooth the transition between the synthetic
		* and real signal.
		*/
		int olen = poverlap + (erasecnt - 1) * EOVERLAPINCR;
		if (olen > gFrameLen)
		olen = gFrameLen;
		getfespeech(overlapbuf, olen);
		overlapaddatend(s, overlapbuf, olen);
		erasecnt = 0;
	}
	savespeech(s);
}

/*
* Generate the synthetic signal.
* At the beginning of an erasure determine the pitch, and extract
* one pitch period from the tail of the signal. Do an OLA for 1/4
* of the pitch to smooth the signal. Then repeat the extracted signal
* for the length of the erasure. If the erasure continues for more than
* 10 ms, increase the number of periods in the pitchbuffer. At the end
* of an erasure, do an OLA with the start of the first good frame.
* The gain decays as the erasure gets longer.
*/
void dofe(short *out)
{
	if (erasecnt == 0) {
		convertsf(history, pitchbuf, HISTORYLEN); /* get history */
		pitch = findpitch(); /* find pitch */
		poverlap = pitch >> 2; /* OLA 1/4 wavelength */
		/* save original last poverlap samples */
		copyf(pitchbufend - poverlap, lastq, poverlap);
		poffset = 0; /* create pitch buffer with 1 period */
		pitchblen = pitch;
		pitchbufstart = pitchbufend - pitchblen;
		Foverlapadd(lastq, pitchbufstart - poverlap,
		pitchbufend - poverlap, poverlap);
		/* update last 1/4 wavelength in history buffer */
		convertfs(pitchbufend - poverlap, &history[HISTORYLEN-poverlap],
		poverlap);
		getfespeech(out, gFrameLen); /* get synthesized speech */
	} else if (erasecnt == 1 || erasecnt == 2) {
		/* tail of previous pitch estimate */
		short tmp[POVERLAPMAX];
		int saveoffset = poffset; /* save offset for OLA */
		getfespeech(tmp, poverlap); /* continue with old pitchbuf */
		/* add periods to the pitch buffer */
		poffset = saveoffset;
		while (poffset > pitch)
		poffset -= pitch;
		pitchblen += pitch; /* add a period */
		pitchbufstart = pitchbufend - pitchblen;
		Foverlapadd(lastq, pitchbufstart - poverlap,
		pitchbufend - poverlap, poverlap);
		/* overlap add old pitchbuffer with new */
		getfespeech(out, gFrameLen);
		Soverlapadd(tmp, out, out, poverlap);
		scalespeech(out);
	} else if (erasecnt > 5) {
		zeros(out, gFrameLen);
	} else {
		getfespeech(out, gFrameLen);
		scalespeech(out);
	}
	erasecnt++;
	savespeech(out);
}

/*
* Estimate the pitch.
* l - pointer to first sample in last 20 ms of speech.
* r - points to the sample PITCH_MAX before l
*/
int findpitch()
{
	int i, j, k;
	int bestmatch;
	int bestcorr;
	int corr; /* correlation */
	int energy; /* running energy */
	int scale; /* scale correlation by average power */
	int *rp; /* segment to match */
	int *l = pitchbufend - CORRLEN;
	int *r = pitchbufend - CORRBUFLEN;
    int temp;
	/* coarse search */
	rp = r;
	energy = 0;
	corr = 0;
	for (i = 0; i < CORRLEN; i += NDEC) {
		energy += (rp[i]>>DATA_FOR_FIND_PITCH_SHIFT) * (rp[i]>>DATA_FOR_FIND_PITCH_SHIFT);
		corr += (rp[i]>>DATA_FOR_FIND_PITCH_SHIFT) * (l[i]>>DATA_FOR_FIND_PITCH_SHIFT);
	}
	scale = energy;
	if (scale < (CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT))
		scale = CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT;
	temp = fix_sqrt(scale,FIXED_SQRT_STEP);
	corr = corr / temp;
	bestcorr = corr;
	bestmatch = 0;
	for (j = NDEC; j <= PITCHDIFF; j += NDEC) { 
		energy -= (rp[0]>>DATA_FOR_FIND_PITCH_SHIFT) * (rp[0]>>DATA_FOR_FIND_PITCH_SHIFT);
		energy += (rp[CORRLEN]>>DATA_FOR_FIND_PITCH_SHIFT) * (rp[CORRLEN]>>DATA_FOR_FIND_PITCH_SHIFT);
		rp += NDEC;
		corr = 0;
		for (i = 0; i < CORRLEN; i += NDEC)
			corr += (rp[i]>>DATA_FOR_FIND_PITCH_SHIFT) * (l[i]>>DATA_FOR_FIND_PITCH_SHIFT);
		scale = energy;
		if (scale < (CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT))
			scale = (CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT);
		temp = fix_sqrt(scale,FIXED_SQRT_STEP);
		corr = corr / temp;
		if (corr >= bestcorr) {
			bestcorr = corr;
			bestmatch = j;
		}
	}
	/* fine search */
#ifdef FINE_PLC_PITCH_SEARCH
	j = bestmatch - (NDEC - 1);
	if (j < 0)
		j = 0;
	k = bestmatch + (NDEC - 1);
	if (k > PITCHDIFF)
		k = PITCHDIFF;
	rp = &r[j];
	energy = 0;
	corr = 0;
	for (i = 0; i < CORRLEN; i++) {
		energy += (rp[i] >>DATA_FOR_FIND_PITCH_SHIFT)*(rp[i]>>DATA_FOR_FIND_PITCH_SHIFT);
		corr += (rp[i] >>DATA_FOR_FIND_PITCH_SHIFT)*(l[i]>>DATA_FOR_FIND_PITCH_SHIFT);
	}
	scale = energy;
	if (scale < CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT)
		scale = CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT;
	temp = fix_sqrt(scale,FIXED_SQRT_STEP);
	corr = corr/ temp;
	bestcorr = corr;
	bestmatch = j;
	for (j++; j <= k; j++) {
		energy -= (rp[0]>>DATA_FOR_FIND_PITCH_SHIFT)*(rp[0]>>DATA_FOR_FIND_PITCH_SHIFT);
		energy += (rp[CORRLEN]>>DATA_FOR_FIND_PITCH_SHIFT)*(rp[CORRLEN]>>DATA_FOR_FIND_PITCH_SHIFT);
		rp++;
		corr = 0;
		for (i = 0; i < CORRLEN; i++)
			corr += (rp[i]>>DATA_FOR_FIND_PITCH_SHIFT)*(l[i]>>DATA_FOR_FIND_PITCH_SHIFT);
		scale = energy;
		if (scale < (CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT))
			scale = (CORRMINPOWER>>DATA_FOR_FIND_PITCH_SHIFT);
		temp = fix_sqrt(scale,FIXED_SQRT_STEP);
		corr = (corr)/temp;
		if (corr > bestcorr) {
			bestcorr = corr;
			bestmatch = j;
		}
	}
#endif
	bestmatch = PITCH_MAX - bestmatch;
	if(bestmatch > PITCH_MAX)
	{
		bestmatch = PITCH_MAX;
	}

	if(bestmatch < PITCH_MIN)
	{
		bestmatch = PITCH_MIN;
	}
	return bestmatch;
}

/*
* Get samples from the circular pitch buffer. Update poffset so
* when subsequent frames are erased the signal continues.
*/
void getfespeech(short *out, int sz)
{
	while (sz) {
		int cnt = pitchblen - poffset;
		if (cnt > sz)
			cnt = sz;
		convertfs(&pitchbufstart[poffset], out, cnt);
		poffset += cnt;
		if (poffset == pitchblen)
			poffset = 0;
		out += cnt;
		sz -= cnt;
	}
}
 
void scalespeech(short *out)
{
	int i,pcmval;
	int g = FIXED_Q - (erasecnt - 1) * ATTENFAC;
	for (i = 0; i < gFrameLen; i++) {
		pcmval = (out[i] * g);
        pcmval /= FIXED_Q;
		out[i] = (short)pcmval;
		//g -= ATTENINCR;
		g -= gAttenIncr;
	}
}

/*
* Overlap add left and right sides
*/

void Foverlapadd(int *l, int *r, int *o, int cnt)
{
	int i;
	int incr = FIXED_Q / cnt;
	int lw = FIXED_Q - incr;
	int rw = incr;

	for (i = 0; i < cnt; i++) {
		int t = lw * l[i]; 
		t += rw * r[i];
        t /= FIXED_Q;
		if (t > 32767)
			t = 32767;
		else if (t < -32768)
			t = -32768;
		o[i] = t;
		lw -= incr;
		rw += incr;
	}
}

void Soverlapadd(short *l, short *r, short *o, int cnt)
{
	int i;
	int incr = FIXED_Q / cnt;
	int lw = FIXED_Q - incr;
	int rw = incr;

	for (i = 0; i < cnt; i++) {
		int t = lw * l[i];
		t += rw * r[i];
		t /= FIXED_Q;
 		if (t > 32767)
			t = 32767;
		else if (t < -32768)
			t = -32768;
		o[i] = (short)t;
		lw -= incr;
		rw += incr;
	}
}

/*
* Overlap add the end of the erasure with the start of the first good frame
* Scale the synthetic speech by the gain factor before the OLA.
*/
void overlapaddatend(short *s, short *f, int cnt)
{
	int i;
	int incr;
	int gain;
	int incrg;
	int lw;
	int rw;

	incr = FIXED_Q / cnt;
	gain = (1 - (erasecnt - 1) * ATTENFAC);
	if (gain < 0)
		gain = 0;
	incrg = incr * gain;
	lw = (FIXED_Q - incr) * gain;
	rw = incr;
	for (i = 0; i < cnt; i++) {
		int t = lw * f[i];
		t += rw * s[i];
		t /= FIXED_Q;
		if (t > 32767)
			t = 32767;
		else if (t < -32768)
			t = -32768;
		s[i] = (short)t;
		lw -= incrg;
		rw += incr;
	}
}

