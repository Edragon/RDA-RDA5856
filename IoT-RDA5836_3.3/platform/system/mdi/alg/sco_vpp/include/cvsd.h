#ifndef __CVSD_H_
#define __CVSD_H_

// BUFFER_SIZE must >= 2;
#define BUFFER_SIZE 2

// 2048/4096;
#define CVSD_FILTER_ROUND 2048

//0.5
#define CVSD_DEC_OUTPUT_ROUND 0x200

#define CVSD_DELTA_MAX (0x140000)
#define CVSD_DELTA_MIN (0x2800)
#define CVSD_ENC_DELTA_MAX (CVSD_DELTA_MAX)
#define CVSD_ENC_DELTA_MIN (CVSD_DELTA_MIN)
#define CVSD_DEC_DELTA_MAX (CVSD_DELTA_MAX)
#define CVSD_DEC_DELTA_MIN (CVSD_DELTA_MIN)
#define CVSD_LAST_DATA_MAX     (0x01fffc00)
#define CVSD_LAST_DATA_MIN     (-CVSD_LAST_DATA_MAX)

int CvsdInit(void);
int CVSDEncode(short *pInPutData, short *pOutPutdata, int iEncSize);
int CVSDDecode(short *pInPutData, short *pOutPutdata, int iDecSize);


#endif
