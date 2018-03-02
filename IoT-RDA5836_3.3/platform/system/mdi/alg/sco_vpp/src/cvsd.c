#include "cvsd.h"
#include "cs_types.h"
#include "string.h"

//#define OPEN_OPTIMIZE
#define USE_NEW_CEVA_IP_CVSD
static short CvsdFiltter3Temp[16] = {0};

#ifndef USE_NEW_CEVA_IP_CVSD
static short CvsdEncFiltter1History[5] = {0};
static short CvsdEncFiltter2History[3] = {0};
static short CvsdEncFiltter3History[3] = {0};

static short CvsdDecFiltter1History0[5] = {0};
static short CvsdDecFiltter2History0[3] = {0};
static short CvsdDecFiltter3History0[3] = {0};
static short CvsdDecFiltter1History1[5] = {0};
static short CvsdDecFiltter2History1[3] = {0};
static short CvsdDecFiltter3History1[3] = {0};

static short CvsdFiltter1Temp[4] = {0};
static short CvsdFiltter2Temp[8] = {0};

short CvsdEncFilter1Coef[5] ={0xFF80,
							0x0000,
							0x05C0,
							0x0300,
							0xFE80};

short CvsdEncFilter2Coef[3] ={0x0080,
							0x0480,
							0x02C0};

short CvsdEncFilter3Coef[3] ={0x0080,  
							0x0480,
							0x02C0};

short CvsdDecFilter1Coef[5] ={0xFF80,
							0x0000,
							0x05C0,
							0x0300,
							0xFE80
							};

short CvsdDecFilter2Coef[3] ={0x0080,
							0x0480,
							0x02C0
							};

short CvsdDecFilter3Coef[3] ={0x0080,  
							0x0480,
							0x02C0
							};
#else
typedef int bits18;
typedef int bits19;
typedef int bits21;
typedef int bits22;
typedef int bits24;
//Up sample filter
static bits18 up_h1y11_a2, up_h1y11_b1, up_h1y11_b2;
static bits18 up_h1y21_b1, up_h1y21_b2;
static bits18 up_h1y22_a1, up_h1y22_a2, up_h1y22_b1, up_h1y22_b2;

static bits18 up_h2y11_a2, up_h2y11_b1, up_h2y11_b2;
static bits18 up_h2y21_b1, up_h2y21_b2;
static bits18 up_h2y22_a1, up_h2y22_a2, up_h2y22_b1, up_h2y22_b2;

static bits18 up_h3y11_a2, up_h3y11_b1, up_h3y11_b2;
static bits18 up_h3y21_b1, up_h3y21_b2;
static bits18 up_h3y22_a1, up_h3y22_a2, up_h3y22_b1, up_h3y22_b2;

//Down sample filter
static bits18 down_h1y11_a2, down_h1y11_b1, down_h1y11_b2;
static bits18 down_h1y21_b1, down_h1y21_b2;
static bits18 down_h1y22_a1, down_h1y22_a2, down_h1y22_b1, down_h1y22_b2;

static bits18 down_h2y11_a2, down_h2y11_b1, down_h2y11_b2;
static bits18 down_h2y21_b1, down_h2y21_b2;
static bits18 down_h2y22_a1, down_h2y22_a2, down_h2y22_b1, down_h2y22_b2;

static bits18 down_h3y11_a2, down_h3y11_b1, down_h3y11_b2;
static bits18 down_h3y21_b1, down_h3y21_b2;
static bits18 down_h3y22_a1, down_h3y22_a2, down_h3y22_b1, down_h3y22_b2;
#endif

unsigned short CvsdEncJudgeAlpha = 0x0a;
static int   CvsdEncLastData = 0;
static int   CvsdEncDelta   = 0;
int CvsdEncFirstFrameFlag = 0;

unsigned short CvsdDecJudgeAlpha = 0x0a;
static int   CvsdDecLastData = 0;
static int   CvsdDecDelta   = 0;

int CvsdInit(void)
{
	CvsdEncFirstFrameFlag = 0;
	
	CvsdEncJudgeAlpha = 0x0a;
	CvsdEncLastData = 0;
	CvsdEncDelta   = 0;

	CvsdDecJudgeAlpha = 0x0a;
	CvsdDecLastData = 0;
	CvsdDecDelta   = 0;
#ifndef USE_NEW_CEVA_IP_CVSD
	memset(CvsdEncFiltter1History, 0, sizeof(CvsdEncFiltter1History));
	memset(CvsdEncFiltter2History, 0, sizeof(CvsdEncFiltter2History));
	memset(CvsdEncFiltter3History, 0, sizeof(CvsdEncFiltter3History));

	memset(CvsdDecFiltter1History0, 0, sizeof(CvsdDecFiltter1History0));
	memset(CvsdDecFiltter2History0, 0, sizeof(CvsdDecFiltter2History0));
	memset(CvsdDecFiltter3History0, 0, sizeof(CvsdDecFiltter3History0));

	memset(CvsdDecFiltter1History1, 0, sizeof(CvsdDecFiltter1History1));
	memset(CvsdDecFiltter2History1, 0, sizeof(CvsdDecFiltter2History1));
	memset(CvsdDecFiltter3History1, 0, sizeof(CvsdDecFiltter3History1));

	memset(CvsdFiltter1Temp, 0, sizeof(CvsdFiltter1Temp));
	memset(CvsdFiltter2Temp, 0, sizeof(CvsdFiltter2Temp));
#else
    up_h1y11_a2 = 0;
    up_h1y11_b1 = 0;
    up_h1y11_b2 = 0;;
    up_h1y21_b1 = 0;
	up_h1y21_b2 = 0;
    up_h1y22_a1 = 0;
	up_h1y22_a2 = 0;
	up_h1y22_b1 = 0;
	up_h1y22_b2 = 0;

    up_h2y11_a2 = 0;
	up_h2y11_b1 = 0;
	up_h2y11_b2 = 0;
    up_h2y21_b1 = 0;
	up_h2y21_b2 = 0;
    up_h2y22_a1 = 0;
	up_h2y22_a2 = 0;
	up_h2y22_b1 = 0;
	up_h2y22_b2 = 0;

    up_h3y11_a2 = 0;
	up_h3y11_b1 = 0;
	up_h3y11_b2 = 0;
    up_h3y21_b1 = 0;
	up_h3y21_b2 = 0;
    up_h3y22_a1 = 0;
	up_h3y22_a2 = 0;
	up_h3y22_b1 = 0;
	up_h3y22_b2 = 0;

    //Down sample filter
    down_h1y11_a2 = 0;
	down_h1y11_b1 = 0;
	down_h1y11_b2 = 0;
    down_h1y21_b1 = 0;
	down_h1y21_b2 = 0;
    down_h1y22_a1 = 0;
	down_h1y22_a2 = 0;
	down_h1y22_b1 = 0;
	down_h1y22_b2 = 0;

    down_h2y11_a2 = 0;
	down_h2y11_b1 = 0;
	down_h2y11_b2 = 0;
    down_h2y21_b1 = 0;
	down_h2y21_b2 = 0;
    down_h2y22_a1 = 0;
	down_h2y22_a2 = 0;
	down_h2y22_b1 = 0;
	down_h2y22_b2 = 0;

    down_h3y11_a2 = 0;
	down_h3y11_b1 = 0;
	down_h3y11_b2 = 0;
    down_h3y21_b1 = 0;
	down_h3y21_b2 = 0;
    down_h3y22_a1 = 0;
	down_h3y22_a2 = 0;
	down_h3y22_b1 = 0;
	down_h3y22_b2 = 0;
#endif
	memset(CvsdFiltter3Temp, 0, sizeof(CvsdFiltter3Temp));
	
	return 1;
}


//iEncSize uint: in 1 pcm sample;
int CvsdEnc(short *pInPutData, short *pOutPutdata, int iEncSize)
{
	unsigned int i,j;
	unsigned short CvsdEncResult = 0;
	for(i=0; i<iEncSize; i++)
	{
		for(j=0; j<16; j++)
		{
			int CvsdEncData = (*(pInPutData+j+i*16))<<10;
			CvsdEncResult >>= 1;
			CvsdEncJudgeAlpha <<= 1;
			if(CvsdEncData<CvsdEncLastData)
			{
                CvsdEncResult |= 0x8000;
				CvsdEncJudgeAlpha |= 0x0001;
			}
			if(((CvsdEncJudgeAlpha&0x000f)==0)||((CvsdEncJudgeAlpha&0x000f)==0x000f))//
			{
                 CvsdEncDelta += CVSD_ENC_DELTA_MIN;
				 if(CvsdEncDelta > CVSD_ENC_DELTA_MAX)
				 {
					 CvsdEncDelta = CVSD_ENC_DELTA_MAX;
				 }

			}else
			{
				CvsdEncDelta = CvsdEncDelta - (CvsdEncDelta>>10);
				if(CvsdEncDelta < CVSD_ENC_DELTA_MIN)
				{
					CvsdEncDelta = CVSD_ENC_DELTA_MIN;
				}
			}

			if(CvsdEncFirstFrameFlag == 0)
			{
				CvsdEncFirstFrameFlag = 1;
				CvsdEncLastData = 0;
			}
			else if(CvsdEncData>=CvsdEncLastData)
			{
				CvsdEncLastData += CvsdEncDelta;
				if(CvsdEncLastData > CVSD_LAST_DATA_MAX)
				{
					CvsdEncLastData = CVSD_LAST_DATA_MAX;
				}
			    CvsdEncLastData = CvsdEncLastData - (CvsdEncLastData>>5);
			}
			else
			{
				CvsdEncLastData -= CvsdEncDelta;
				if(CvsdEncLastData < CVSD_LAST_DATA_MIN)
				{
					CvsdEncLastData = CVSD_LAST_DATA_MIN;
				}
				CvsdEncLastData = CvsdEncLastData - (CvsdEncLastData>>5);
			}
		}
        *(pOutPutdata+i) = CvsdEncResult;
	}
	return 1;
}

int CvsdDec(short *pInPutData, short *pOutPutdata, int iDecSize)
{
	unsigned int i,j;
	unsigned short CvsdDecInPutData = *pInPutData;
	//unsigned short CvsdDecResult = 0;
	for(i=0; i<iDecSize; i++)
	{
		for(j=0; j<16; j++)
		{
			CvsdDecJudgeAlpha <<= 1;
			CvsdDecJudgeAlpha |= (CvsdDecInPutData&1);
			if(((CvsdDecJudgeAlpha&0x000f)==0)||((CvsdDecJudgeAlpha&0x000f)==0x000f))
			{
                 CvsdDecDelta += CVSD_DEC_DELTA_MIN;
				 if(CvsdDecDelta > CVSD_DEC_DELTA_MAX)
				 {
					 CvsdDecDelta = CVSD_DEC_DELTA_MAX;
				 }

			}else
			{
				CvsdDecDelta = CvsdDecDelta - (CvsdDecDelta>>10);
				if(CvsdDecDelta < CVSD_DEC_DELTA_MIN)
				{
					CvsdDecDelta = CVSD_DEC_DELTA_MIN;
				}
			}

			if((CvsdDecInPutData&1) == 0)
			{
				CvsdDecLastData += CvsdDecDelta;

				if(CvsdDecLastData > CVSD_LAST_DATA_MAX)
				{
					CvsdDecLastData = CVSD_LAST_DATA_MAX;
				}
			    CvsdDecLastData = CvsdDecLastData - (CvsdDecLastData>>5);
			}
			else
			{
				CvsdDecLastData -= CvsdDecDelta;
				if(CvsdDecLastData < CVSD_LAST_DATA_MIN)
				{
					CvsdDecLastData = CVSD_LAST_DATA_MIN;
				}
				CvsdDecLastData = CvsdDecLastData - (CvsdDecLastData>>5);
			}
			*(pOutPutdata+j) = (CvsdDecLastData>0) ?(short) ((CvsdDecLastData+CVSD_DEC_OUTPUT_ROUND)>>10) : (short) ((CvsdDecLastData-CVSD_DEC_OUTPUT_ROUND)>>10);
			CvsdDecInPutData >>= 1;
		}
	}
	return 1;
}

#ifndef USE_NEW_CEVA_IP_CVSD
short CvsdHighPassFilterCoef[2] ={0x1000,0x0A80
							};
int CvsdHighPassFilter(short *pInPutData, int iFilterSize, short *pHistoryX, short *pHistoryY)
{
	unsigned int i,j;
	int iFilterDataTemp;
	for(j=0; j<iFilterSize; j++)
	{
		pHistoryX[0] = *(pInPutData+j);

		iFilterDataTemp = (pHistoryX[0] - pHistoryX[1])*CvsdHighPassFilterCoef[0] + pHistoryY[1]*CvsdHighPassFilterCoef[1];

		iFilterDataTemp >>= 12;
		
		if(iFilterDataTemp > 0x7fff)
		{
			iFilterDataTemp = 0x7fff;
		}
		else if(iFilterDataTemp < -0x7fff)
		{
			iFilterDataTemp = -0x7fff;
		}

		pHistoryX[1] = pHistoryX[0];
		pHistoryY[1] = iFilterDataTemp;
		*(pInPutData+j) = iFilterDataTemp;
	}
	return 1;
}
int CvsdUpSampleFilter1(short *pInPutData, short *pOutPutdata, int iFilterSize)
{
	unsigned int i,j;
	int iFilterDataTemp0,iFilterDataTemp1;
	//21 level filter
	for(j=0; j<iFilterSize; j++)
	{
		iFilterDataTemp0 = CVSD_FILTER_ROUND;
	    iFilterDataTemp1 = CVSD_FILTER_ROUND;
		CvsdEncFiltter1History[0] = *(pInPutData+j);
#ifndef OPEN_OPTIMIZE
		for(i=0; i<5; i++)
		{
			iFilterDataTemp0 += CvsdEncFiltter1History[i]*(*(CvsdEncFilter1Coef+i));
			iFilterDataTemp1 += CvsdEncFiltter1History[i]*(*(CvsdEncFilter1Coef+4-i));
		}
		for(i=5; i>0; i--)
		{
			CvsdEncFiltter1History[i] = CvsdEncFiltter1History[i-1];
		}
#else
		iFilterDataTemp0 += (*CvsdEncFiltter1History)*(*CvsdEncFilter1Coef);
		iFilterDataTemp0 += CvsdEncFiltter1History[1]*(*(CvsdEncFilter1Coef+1));
		iFilterDataTemp0 += CvsdEncFiltter1History[2]*(*(CvsdEncFilter1Coef+2));
		iFilterDataTemp0 += CvsdEncFiltter1History[3]*(*(CvsdEncFilter1Coef+3));
		iFilterDataTemp0 += CvsdEncFiltter1History[4]*(*(CvsdEncFilter1Coef+4));


		iFilterDataTemp1 += (*CvsdEncFiltter1History)*(*(CvsdEncFilter1Coef+4));
		iFilterDataTemp1 += CvsdEncFiltter1History[1]*(*(CvsdEncFilter1Coef+3));
		iFilterDataTemp1 += CvsdEncFiltter1History[2]*(*(CvsdEncFilter1Coef+2));
		iFilterDataTemp1 += CvsdEncFiltter1History[3]*(*(CvsdEncFilter1Coef+1));
		iFilterDataTemp1 += CvsdEncFiltter1History[4]*(*(CvsdEncFilter1Coef));

		CvsdEncFiltter1History[4] = CvsdEncFiltter1History[3];
		CvsdEncFiltter1History[3] = CvsdEncFiltter1History[2];
		CvsdEncFiltter1History[2] = CvsdEncFiltter1History[1];
		CvsdEncFiltter1History[1] = *CvsdEncFiltter1History;

#endif
		iFilterDataTemp0 >>= 11;
		iFilterDataTemp1 >>= 11;


		if(iFilterDataTemp0 > 0x7fff)
		{
			iFilterDataTemp0 = 0x7fff;
		}
		else if(iFilterDataTemp0 < -0x7fff)
		{
			iFilterDataTemp0 = -0x7fff;
		}
		if(iFilterDataTemp1 > 0x7fff)
		{
			iFilterDataTemp1 = 0x7fff;
		}
		else if(iFilterDataTemp1 < -0x7fff)
		{
			iFilterDataTemp1 = -0x7fff;
		}
		*(pOutPutdata+2*j) = iFilterDataTemp0;
		*(pOutPutdata+2*j+1) = iFilterDataTemp1;
	}
	return 1;
}

int CvsdUpSampleFilter2(short *pInPutData, short *pOutPutdata, int iFilterSize)
{
	unsigned int i,j;
	int iFilterDataTemp0,iFilterDataTemp1;
	
	//21 level filter
	for(j=0; j<iFilterSize; j++)
	{
		iFilterDataTemp0 = CVSD_FILTER_ROUND;
	    iFilterDataTemp1 = CVSD_FILTER_ROUND;
		CvsdEncFiltter2History[0] = *(pInPutData+j);
#ifndef OPEN_OPTIMIZE
		for(i=0; i<3; i++)
		{
			iFilterDataTemp0 += CvsdEncFiltter2History[i]*(*(CvsdEncFilter2Coef+i));
			iFilterDataTemp1 += CvsdEncFiltter2History[i]*(*(CvsdEncFilter2Coef+2-i));
		}
		for(i=2; i>0; i--)
		{
			CvsdEncFiltter2History[i] = CvsdEncFiltter2History[i-1];
		}
#else
		iFilterDataTemp0 += (*CvsdEncFiltter2History)*(*CvsdEncFilter2Coef);
		iFilterDataTemp0 += CvsdEncFiltter2History[1]*(*(CvsdEncFilter2Coef+1));
		iFilterDataTemp0 += CvsdEncFiltter2History[2]*(*(CvsdEncFilter2Coef+2));


		iFilterDataTemp1 += (*CvsdEncFiltter2History)*(*(CvsdEncFilter2Coef+2));
		iFilterDataTemp1 += CvsdEncFiltter2History[1]*(*(CvsdEncFilter2Coef+1));
		iFilterDataTemp1 += CvsdEncFiltter2History[2]*(*(CvsdEncFilter2Coef));
		
		CvsdEncFiltter2History[2] = CvsdEncFiltter2History[1];
		CvsdEncFiltter2History[1] = *CvsdEncFiltter2History;

#endif
		iFilterDataTemp0 >>= 11;
		iFilterDataTemp1 >>= 11;

		if(iFilterDataTemp0 > 0x7fff)
		{
			iFilterDataTemp0 = 0x7fff;
		}
		else if(iFilterDataTemp0 < -0x7fff)
		{
			iFilterDataTemp0 = -0x7fff;
		}
		if(iFilterDataTemp1 > 0x7fff)
		{
			iFilterDataTemp1 = 0x7fff;
		}
		else if(iFilterDataTemp1 < -0x7fff)
		{
			iFilterDataTemp1 = -0x7fff;
		}
		*(pOutPutdata+2*j) = iFilterDataTemp0;
		*(pOutPutdata+2*j+1) = iFilterDataTemp1;
	}
	return 1;
}

int CvsdUpSampleFilter3(short *pInPutData, short *pOutPutdata, int iFilterSize)
{
	unsigned int i,j;
	int iFilterDataTemp0,iFilterDataTemp1;
	
	//21 level filter
	for(j=0; j<iFilterSize; j++)
	{
		iFilterDataTemp0 = CVSD_FILTER_ROUND;
	    iFilterDataTemp1 = CVSD_FILTER_ROUND;
		CvsdEncFiltter3History[0] = *(pInPutData+j);
		
#ifndef OPEN_OPTIMIZE
		for(i=0; i<3; i++)
		{
			iFilterDataTemp0 += CvsdEncFiltter3History[i]*(*(CvsdEncFilter3Coef+i));
			iFilterDataTemp1 += CvsdEncFiltter3History[i]*(*(CvsdEncFilter3Coef+2-i));
		}
		for(i=2; i>0; i--)
		{
			CvsdEncFiltter3History[i] = CvsdEncFiltter3History[i-1];
		}
#else
		iFilterDataTemp0 += (*CvsdEncFiltter3History)*(*CvsdEncFilter3Coef);
		iFilterDataTemp0 += CvsdEncFiltter3History[1]*(*(CvsdEncFilter3Coef+1));
		iFilterDataTemp0 += CvsdEncFiltter3History[2]*(*(CvsdEncFilter3Coef+2));


		iFilterDataTemp1 += (*CvsdEncFiltter3History)*(*(CvsdEncFilter3Coef+2));
		iFilterDataTemp1 += CvsdEncFiltter3History[1]*(*(CvsdEncFilter3Coef+1));
		iFilterDataTemp1 += CvsdEncFiltter3History[2]*(*(CvsdEncFilter3Coef));
		
		CvsdEncFiltter3History[2] = CvsdEncFiltter3History[1];
		CvsdEncFiltter3History[1] = *CvsdEncFiltter3History;
#endif
		iFilterDataTemp0 >>= 11;
		iFilterDataTemp1 >>= 11;

		
		if(iFilterDataTemp0 > 0x7fff)
		{
			iFilterDataTemp0 = 0x7fff;
		}
		else if(iFilterDataTemp0 < -0x7fff)
		{
			iFilterDataTemp0 = -0x7fff;
		}
		if(iFilterDataTemp1 > 0x7fff)
		{
			iFilterDataTemp1 = 0x7fff;
		}
		else if(iFilterDataTemp1 < -0x7fff)
		{
			iFilterDataTemp1 = -0x7fff;
		}
		*(pOutPutdata+2*j) = iFilterDataTemp0;
		*(pOutPutdata+2*j+1) = iFilterDataTemp1;
	}
	return 1;
}


int CVSDEncode(short *pInPutData, short *pOutPutdata, int iEncSize)
{
	int i;
	if(iEncSize<=0)
	{
		return 0;
	}
	if((pInPutData == NULL)||(pOutPutdata == NULL))
	{
		return 0;
	}
	for(i=0; i<iEncSize/2; i++)
	{   //input 2 pcm sample ,ouput 1 cvsd data;
		CvsdUpSampleFilter1(pInPutData+2*i, CvsdFiltter1Temp, 2); 
		CvsdUpSampleFilter2(CvsdFiltter1Temp, CvsdFiltter2Temp, 4);
		CvsdUpSampleFilter3(CvsdFiltter2Temp, CvsdFiltter3Temp, 8);
		CvsdEnc(CvsdFiltter3Temp, pOutPutdata+i, 1);
	}
	return 1;
}


int CvsdDownSampleFilter1(short *pInPutData, short *pOutPutdata, int iFilterSize)
{
	unsigned int i,j;
	int iFilterDataTemp0,iFilterDataTemp1;
	
	for(j=0; j<iFilterSize; j++)
	{
		iFilterDataTemp0 = CVSD_FILTER_ROUND;
		CvsdDecFiltter1History0[0] = *(pInPutData+2*j);
#ifndef OPEN_OPTIMIZE
		for(i=0; i<5; i++)
		{
			iFilterDataTemp0 += CvsdDecFiltter1History0[i]*(*(CvsdDecFilter1Coef+i));
		}
		for(i=4; i>0; i--)
		{
			CvsdDecFiltter1History0[i] = CvsdDecFiltter1History0[i-1];
		}
#else
		iFilterDataTemp0 += (*CvsdDecFiltter1History0)*(*CvsdDecFilter1Coef);
		iFilterDataTemp0 += CvsdDecFiltter1History0[1]*(*(CvsdDecFilter1Coef+1));
		iFilterDataTemp0 += CvsdDecFiltter1History0[2]*(*(CvsdDecFilter1Coef+2));
		iFilterDataTemp0 += CvsdDecFiltter1History0[3]*(*(CvsdDecFilter1Coef+3));
		iFilterDataTemp0 += CvsdDecFiltter1History0[4]*(*(CvsdDecFilter1Coef+4));

		CvsdDecFiltter1History0[4] = CvsdDecFiltter1History0[3];
		CvsdDecFiltter1History0[3] = CvsdDecFiltter1History0[2];
		CvsdDecFiltter1History0[2] = CvsdDecFiltter1History0[1];
		CvsdDecFiltter1History0[1] = *CvsdDecFiltter1History0;
#endif
		iFilterDataTemp0 >>= 11;
		if(iFilterDataTemp0 > 0x7fff)
		{
			iFilterDataTemp0 = 0x7fff;
		}
		else if(iFilterDataTemp0 < -0x7fff)
		{
			iFilterDataTemp0 = -0x7fff;
		}
		*(pOutPutdata + j) = iFilterDataTemp0;
	}
	return 1;
}

int CvsdDownSampleFilter2(short *pInPutData, short *pOutPutdata, int iFilterSize)
{
	unsigned int i,j;
	int iFilterDataTemp0,iFilterDataTemp1;
	
	for(j=0; j<iFilterSize; j++)
	{
		iFilterDataTemp0 = CVSD_FILTER_ROUND;
		CvsdDecFiltter2History0[0] = *(pInPutData+2*j);
#ifndef OPEN_OPTIMIZE
		for(i=0; i<3; i++)
		{
			iFilterDataTemp0 += CvsdDecFiltter2History0[i]*(*(CvsdDecFilter2Coef+i));
		}
		for(i=2; i>0; i--)
		{
			CvsdDecFiltter2History0[i] = CvsdDecFiltter2History0[i-1];
		}
#else
		iFilterDataTemp0 += (*CvsdDecFiltter2History0)*(*CvsdDecFilter2Coef);
		iFilterDataTemp0 += CvsdDecFiltter2History0[1]*(*(CvsdDecFilter2Coef+1));
		iFilterDataTemp0 += CvsdDecFiltter2History0[2]*(*(CvsdDecFilter2Coef+2));

		CvsdDecFiltter2History0[2] = CvsdDecFiltter2History0[1];
		CvsdDecFiltter2History0[1] = *CvsdDecFiltter2History0;
#endif
		iFilterDataTemp0 >>= 11;

		if(iFilterDataTemp0 > 0x7fff)
		{
			iFilterDataTemp0 = 0x7fff;
		}
		else if(iFilterDataTemp0 < -0x7fff)
		{
			iFilterDataTemp0 = -0x7fff;
		}

		*(pOutPutdata + j) = iFilterDataTemp0;

	}
	return 1;
}

int CvsdDownSampleFilter3(short *pInPutData, short *pOutPutdata, int iFilterSize)
{
	unsigned int i,j;
	int iFilterDataTemp0,iFilterDataTemp1;
	
	for(j=0; j<iFilterSize; j++)
	{
		iFilterDataTemp0 = CVSD_FILTER_ROUND;
		CvsdDecFiltter3History0[0] = *(pInPutData+2*j);
#ifndef OPEN_OPTIMIZE
		for(i=0; i<3; i++)
		{
			iFilterDataTemp0 += CvsdDecFiltter3History0[i]*(*(CvsdDecFilter3Coef+i));
			iFilterDataTemp1 += CvsdDecFiltter3History1[i]*(*(CvsdDecFilter3Coef+2-i));
		}
		for(i=2; i>0; i--)
		{
			CvsdDecFiltter3History0[i] = CvsdDecFiltter3History0[i-1];
			CvsdDecFiltter3History1[i] = CvsdDecFiltter3History1[i-1];
		}
#else
		iFilterDataTemp0 += (*CvsdDecFiltter3History0)*(*CvsdDecFilter3Coef);
		iFilterDataTemp0 += CvsdDecFiltter3History0[1]*(*(CvsdDecFilter3Coef+1));
		iFilterDataTemp0 += CvsdDecFiltter3History0[2]*(*(CvsdDecFilter3Coef+2));

		CvsdDecFiltter3History0[2] = CvsdDecFiltter3History0[1];
		CvsdDecFiltter3History0[1] = *CvsdDecFiltter3History0;
#endif
		iFilterDataTemp0 >>= 11;
		if(iFilterDataTemp0 > 0x7fff)
		{
			iFilterDataTemp0 = 0x7fff;
		}
		else if(iFilterDataTemp0 < -0x7fff)
		{
			iFilterDataTemp0 = -0x7fff;
		}
		*(pOutPutdata + j) = iFilterDataTemp0;

	}
	return 1;
}


//iDecSize uint: out 1 pcm sample;
 int CVSDDecode(short *pInPutData, short *pOutPutdata, int iDecSize)
 {
	 int i;
	 if(iDecSize<=0)
	 {
		 return 0;
	 }
	 if((pInPutData == NULL)||(pOutPutdata == NULL))
	 {
		 return 0;
	 }
//TM_SendTestPointRequest (984,   55555);
	 for(i=0; i<iDecSize/2; i++)
	 {	 //input 1 short  cvsd sample ,ouput 2 short  pcm data;
	     	 CvsdDec(pInPutData+i, CvsdFiltter3Temp, 1);
		 CvsdDownSampleFilter3(CvsdFiltter3Temp, CvsdFiltter2Temp, 8);
		 CvsdDownSampleFilter2(CvsdFiltter2Temp, CvsdFiltter1Temp, 4);
		 CvsdDownSampleFilter1(CvsdFiltter1Temp, pOutPutdata+2*i, 2); 
	 }
	 return 1;
 }
#endif

#ifdef USE_NEW_CEVA_IP_CVSD
#if 1
int CvsdDecNew(short *pInPutData, short *pOutPutdata, int iDecSize)
{
	unsigned int i,j;
	unsigned short CvsdDecInPutData = *pInPutData;
	int CvsdDecLastDataSat; // 22bits
	//unsigned short CvsdDecResult = 0;
	for(i=0; i<iDecSize; i++)
	{
		for(j=0; j<16; j++)
		{
			CvsdDecJudgeAlpha <<= 1;
			CvsdDecJudgeAlpha |= (CvsdDecInPutData&1);
			if(((CvsdDecJudgeAlpha&0x000f)==0)||((CvsdDecJudgeAlpha&0x000f)==0x000f))
			{
                 CvsdDecDelta += CVSD_DEC_DELTA_MIN;
				 if(CvsdDecDelta > CVSD_DEC_DELTA_MAX)
				 {
					 CvsdDecDelta = CVSD_DEC_DELTA_MAX;
				 }

			}
			else
			{

				//CvsdDecDelta = CvsdDecDelta - (CvsdDecDelta>>10);
				CvsdDecDelta = CvsdDecDelta - ((CvsdDecDelta+512)>>10);

				if(CvsdDecDelta < CVSD_DEC_DELTA_MIN)
				{
					CvsdDecDelta = CVSD_DEC_DELTA_MIN;
				}
			}

            if(CvsdDecLastData > (CVSD_LAST_DATA_MAX>>4))
			{
			    CvsdDecLastDataSat = (CVSD_LAST_DATA_MAX>>4);
			}
			else if(CvsdDecLastData < (CVSD_LAST_DATA_MIN>>4))
			{
				CvsdDecLastDataSat = (CVSD_LAST_DATA_MIN>>4);
			}
			else {
				CvsdDecLastDataSat = CvsdDecLastData;
			}

			CvsdDecLastDataSat = CvsdDecLastDataSat - ((CvsdDecLastDataSat>=0) ? ((CvsdDecLastDataSat + 16) >> 5) : ((CvsdDecLastDataSat - 16) >> 5));

			if((CvsdDecInPutData&1) == 0)
			{
				//CvsdDecLastData += CvsdDecDelta;
				CvsdDecLastData = CvsdDecLastDataSat + (CvsdDecDelta>>4);

				
			    //CvsdDecLastData = CvsdDecLastData - (CvsdDecLastData>>5);
			}
			else
			{
				CvsdDecLastData = CvsdDecLastDataSat - (CvsdDecDelta>>4);
				
				//CvsdDecLastData = CvsdDecLastData - (CvsdDecLastData>>5);
			}

			//*(pOutPutdata+j) = (CvsdDecLastData>=0) ?(short) ((CvsdDecLastData+CVSD_DEC_OUTPUT_ROUND)>>10) : (short) ((CvsdDecLastData-CVSD_DEC_OUTPUT_ROUND)>>10);
			//*(pOutPutdata+j) = (CvsdDecLastData>=0) ?(short) ((CvsdDecLastData+CVSD_DEC_OUTPUT_ROUND)>>10) : (short) ((CvsdDecLastData)>>10); //Need check
			*(pOutPutdata+j) = (short) (CvsdDecLastDataSat>>6);
			CvsdDecInPutData >>= 1;
		}
	} 
	return 1; 
}
#endif
INLINE void allpassh1y11(bits18 *a1, bits18 *a2, bits18 *b1, bits18 *b2)
{
	*b1 = -*a1;
	*b2 = -*a2;
}
INLINE void allpassh2y21(bits18 *a1, bits18 *a2, bits18 *b1, bits18 *b2)
{  
	bits18 y_in, b1_int, y_out, y_div_rounded;
	bits19 y_div;

	y_in = *a2 - *a1;

	y_div = y_in; //RTL: y_div=(y_in[17] == 1'b0) ? {1'b0, y_in[17:0]} : {1'b1, y_in[17:0]}; Sign extention
	y_div_rounded = (y_div >= 0) ? ((y_div+1)>>1) : (( y_div-1) >> 1); //rouding by half-adjust and div by 2

	//y = 1/10
	y_out = y_div_rounded;

	b1_int = y_out - *a2;
	*b1 = b1_int;
	*b2 = b1_int - y_in;
}
INLINE void allpassh2y11(bits18 *a1, bits18 *a2, bits18 *b1, bits18 *b2)
{  
	bits18 y_in, b1_int, y_out, y_div_rounded, y_in_shifted1;
	bits19 y_div;

	y_in = *a2 - *a1;

	y_div = y_in; //RTL: y_div=(y_in[17] == 1'b0) ? {1'b0, y_in[17:0]} : {1'b1, y_in[17:0]}; Sign extention
	y_div_rounded = (y_div >= 0) ? ((y_div+1)>>1) : (( y_div-1) >> 1); //rouding by half-adjust and div by 2
	y_in_shifted1 = y_div; //RTL:y_in_shifted1 = y_div[17:0]

	//y = 11/10
    y_out = y_in_shifted1 + y_div_rounded;

	b1_int = y_out - *a2;
	*b1 = b1_int;
	*b2 = b1_int - y_in;
}
INLINE void allpassh3y11(bits18 *a1, bits18 *a2, bits18 *b1, bits18 *b2)
{  
	bits18 y_in, b1_int, y_out;
	bits22 y_res, y_div, y_in_shifted2, y_in_shifted4;

	y_in = *a2 - *a1;

	y_div = y_in; //RTL: y_div=(y_in[17] == 1'b0) ? {1'b0, y_in[17:0]} : {1'b1, y_in[17:0]}; Sign extention
	y_in_shifted2 = y_div << 2;
	y_in_shifted4 = y_div << 4;

	//y = 10101/10000
    y_res = y_in_shifted4 + y_in_shifted2 + y_div;
	y_out = (y_res >= 0) ? ((y_res+8) >> 4) : ((y_res-8) >> 4);

	b1_int = y_out - *a2;
	*b1 = b1_int;
	*b2 = b1_int - y_in;
}
INLINE void allpassh3y21(bits18 *a1, bits18 *a2, bits18 *b1, bits18 *b2)
{  
	bits18 y_in, b1_int, y_out;
	bits21 y_res, y_div, y_in_shifted1;

	y_in = *a2 - *a1;
	
	y_div = y_in; //RTL: y_div=(y_in[17] == 1'b0) ? {1'b0, y_in[17:0]} : {1'b1, y_in[17:0]}; Sign extention
	y_in_shifted1 = y_div << 1;

	//y = 0011/1000
    y_res = y_in_shifted1 + y_div;
	y_out = (y_res >= 0) ? ((y_res + 4) >> 3) : ((y_res - 4) >> 3);
	
	b1_int = y_out - *a2;
	*b1 = b1_int;
	*b2 = b1_int - y_in;
}

INLINE void allpassh3y22(bits18 *a1,  bits18 *a2, bits18 *b1, bits18 *b2)   
{  
	bits18 y_in, b1_int, y_out;
	bits24 y_res, y_div, y_in_shifted2, y_in_shifted3, y_in_shifted6;

	y_in = *a2 - *a1;
	
	y_div = y_in; //RTL: y_div=(y_in[17] == 1'b0) ? {1'b0, y_in[17:0]} : {1'b1, y_in[17:0]}; Sign extention
	y_in_shifted2 = y_div << 2;
	y_in_shifted3 = y_div << 3;
	y_in_shifted6 = y_div << 6;

	//y = 1001101/1000000
    y_res = y_in_shifted6 + y_in_shifted3 + y_in_shifted2 + y_div;
	y_out = (y_res >= 0) ? ((y_res + 32) >> 6) : ((y_res - 32) >> 6);
	
	b1_int = y_out - *a2;
	*b1 = b1_int;
	*b2 = b1_int - y_in;
}

int CVSDEncode(short *pInPutData, short *pOutPutdata, int iEncSize)
{
	int i,j;
	bits18 sample_in_up16k, data_from_h3u;
	bits18 data_from_h3u_up32k, data_from_h2u;
	bits18 data_from_h2u_up64k, data_from_h1u;
	short sample_out;

	if(iEncSize<=0)
	{
		return 0;
	}
	if((pInPutData == NULL)||(pOutPutdata == NULL))
	{
		return 0;
	}
	for(i=0; i<iEncSize/2; i++)
	{   //input 2 pcm sample ,ouput 1 cvsd data;
		
		for(j=0; j<16; j++) {
			if(j == 0) {
			    sample_in_up16k = pInPutData[2*i];
			}
			else if(j == 8) {
			    sample_in_up16k = pInPutData[2*i+1];
			}
			else {
				sample_in_up16k = 0;
			}

			//filter H3u
			//filter_h3y11
			allpassh3y11(&sample_in_up16k, &up_h3y11_a2, &up_h3y11_b1, &up_h3y11_b2);
			//filter_h3y22
			allpassh3y22(&up_h3y22_a1, &up_h3y22_a2, &up_h3y22_b1, &up_h3y22_b2);
			//filter_h3y21
			allpassh3y21(&sample_in_up16k, &up_h3y22_b2, &up_h3y21_b1, &up_h3y21_b2);

			if(j%4 == 0) {
			    up_h3y11_a2 = up_h3y11_b1;
				up_h3y22_a1 = up_h3y21_b1;
				up_h3y22_a2 = up_h3y22_b1;

			    data_from_h3u_up32k = up_h3y21_b2 + up_h3y11_b2;
			}
			else {
				data_from_h3u_up32k = 0;
			}

			//filter H2u
			//filter_h2y11
			allpassh2y11(&data_from_h3u_up32k, &up_h2y11_a2, &up_h2y11_b1, &up_h2y11_b2);
			//filter_h2y22
			allpassh2y11(&up_h2y22_a1, &up_h2y22_a2, &up_h2y22_b1, &up_h2y22_b2);
			//filter_h2y21
			allpassh2y21(&data_from_h3u_up32k, &up_h2y22_b2, &up_h2y21_b1, &up_h2y21_b2);
			
			if(j%2 == 0) {
			    up_h2y11_a2 = up_h2y11_b1;
				up_h2y22_a1 = up_h2y21_b1;
				up_h2y22_a2 = up_h2y22_b1;

				data_from_h2u_up64k = up_h2y21_b2 + up_h2y11_b2;
			}
			else {
				data_from_h2u_up64k = 0;
			}

			//filter H1u
			//filter_h1y11
			allpassh1y11(&data_from_h2u_up64k, &up_h1y11_a2, &up_h1y11_b1, &up_h1y11_b2);
			//filter_h1y22
			allpassh1y11(&up_h1y22_a1, &up_h1y22_a2, &up_h1y22_b1, &up_h1y22_b2);
			//filter_h1y21
			allpassh2y21(&data_from_h2u_up64k, &up_h1y22_b2, &up_h1y21_b1, &up_h1y21_b2);

			up_h1y11_a2 = up_h1y11_b1;
			up_h1y22_a1 = up_h1y21_b1;
			up_h1y22_a2 = up_h1y22_b1;

			sample_out = up_h1y21_b2 + up_h1y11_b2;

            //saturation
			 if(sample_out > 0x7FFF) {
				sample_out = 0x7FFF;
			 }
			 else {
			   if(sample_out < -0x7FFF) {
				   sample_out = -0x8000;
			   }
			 }
			 CvsdFiltter3Temp[j] = sample_out;
			 
		}
		CvsdEnc(CvsdFiltter3Temp, pOutPutdata+i, 1);

#ifdef DEBUG_DUMP_DATA
             memcpy(CvsdFiltter1TempDump+4*i, CvsdFiltter1Temp, 8);
             memcpy(CvsdFiltter2TempDump+8*i, CvsdFiltter2Temp, 16);
             memcpy(CvsdFiltter3TempDump+16*i, CvsdFiltter3Temp, 32);
#endif
	}
	return 1;
}

//iDecSize uint: out 1 pcm sample;
 int CVSDDecode(short *pInPutData, short *pOutPutdata, int iDecSize)
 {
	 int i,j;
	 bits18 sample_18b;
	 bits18 data_h1u_int, data_from_h1u_down32k;
	 bits18 data_h2u_int, data_h2u_int_div2, data_from_h2u_down16k;
	 bits18 data_h3u_int_c, data_h3u_int, data_h3u_int_div2, data_from_h3u_down8k;
	 static short decimator_data_out;

	 if(iDecSize<=0)
	 {
		 return 0;
	 }
	 
	 if((pInPutData == NULL)||(pOutPutdata == NULL))
	 {
		 return 0;
	 }
	 
	 for(i=0; i<(iDecSize/2); i++)
	 {	 //input 1 short  cvsd sample ,ouput 2 short  pcm data;
	     CvsdDecNew(pInPutData+i, CvsdFiltter3Temp, 1);
		 
		 for(j=0;j<16;j++) {
			 sample_18b = CvsdFiltter3Temp[j];
			 /*Filter H1U*/
			 //filter_h1y11 
			 allpassh1y11(&sample_18b, &down_h1y11_a2, &down_h1y11_b1, &down_h1y11_b2);
			 //filter_h1y22
			 allpassh1y11(&down_h1y22_a1, &down_h1y22_a2, &down_h1y22_b1, &down_h1y22_b2);
			 //filter_h1y21
			 allpassh2y21(&sample_18b, &down_h1y22_b2, &down_h1y21_b1, &down_h1y21_b2);

			 down_h1y11_a2 = down_h1y11_b1;
			 down_h1y22_a1 = down_h1y21_b1;
			 down_h1y22_a2 = down_h1y22_b1;

			 data_h1u_int = down_h1y21_b2 + down_h1y11_b2;
			 data_from_h1u_down32k = (data_h1u_int >= 0) ? ((data_h1u_int + 1)>>1) : ((data_h1u_int - 1)>>1);
			     
			 /*Filter H2U*/
			 //filter_h2y11
			 allpassh2y11(&data_from_h1u_down32k, &down_h2y11_a2, &down_h2y11_b1, &down_h2y11_b2);
			 //filter_h2y22
			 allpassh2y11(&down_h2y22_a1, &down_h2y22_a2, &down_h2y22_b1, &down_h2y22_b2);
			 //filter_h2y21
			 allpassh2y21(&data_from_h1u_down32k, &down_h2y22_b2, &down_h2y21_b1, &down_h2y21_b2);

			 if(j%2==0) {
				 
			 
			     down_h2y11_a2 = down_h2y11_b1;
			     down_h2y22_a1 = down_h2y21_b1;
			     down_h2y22_a2 = down_h2y22_b1;     
			 }

			 data_h2u_int = down_h2y21_b2 + down_h2y11_b2;
			 data_from_h2u_down16k = (data_h2u_int >=0) ? ((data_h2u_int + 1)>>1) : ((data_h2u_int - 1)>>1);

			 /*Filter H3U*/
			 //filter_h3y11
			 allpassh3y11(&data_from_h2u_down16k, &down_h3y11_a2, &down_h3y11_b1, &down_h3y11_b2);
			 //filter_h3y22
			 allpassh3y22(&down_h3y22_a1, &down_h3y22_a2, &down_h3y22_b1, &down_h3y22_b2);
			 //filter_h3y21
			 allpassh3y21(&data_from_h2u_down16k, &down_h3y22_b2, &down_h3y21_b1, &down_h3y21_b2);

			 if(j%4 == 0) {
			     down_h3y11_a2 = down_h3y11_b1;
			     down_h3y22_a1 = down_h3y21_b1;
			     down_h3y22_a2 = down_h3y22_b1;	 
			 }

			 if((j%8) == 0) {
				data_h3u_int_c = down_h3y21_b2 + down_h3y11_b2;
			    data_h3u_int = data_h3u_int_c;
			    decimator_data_out = (data_h3u_int>=0) ? ((data_h3u_int + 1)>>1) : ((data_h3u_int - 1)>>1);
				
				//saturation
				if(decimator_data_out > 0x7FFF) {
				    decimator_data_out = 0x7FFF;
				}
				else if(decimator_data_out < -0x7FFF) {
					decimator_data_out = -0x7FFF;
				}

			     if(j==0) {
			         pOutPutdata[2*i] = decimator_data_out;
			     }
				 else {
			         pOutPutdata[2*i+1] = decimator_data_out;
			     }
			 }
		 }	 
	 }

	 return 1;
 } 
 #endif
