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
#ifndef ANALOG_AUDIO_IIR_H
#define ANALOG_AUDIO_IIR_H

#ifdef ANDROID
#include <cutils/log.h>
#define IIRLOGI(msg, ...)	LOGI(msg, ##__VA_ARGS__)
#define IIRLOGE(msg, ...)	LOGE(msg, ##__VA_ARGS__)
#else
#include "mcip_debug.h"
#define DEBUG
#define IIRLOGI	diag_printf
#define IIRLOGE	diag_printf
#endif

#define FIXED_POINT


#ifdef FIXED_POINT
typedef int coeff_t;
typedef int data_t;

#define COEFF_SHIFT 19
#define DATA_SHIFT 13

#define CAT(a,...)	PRIMITIVE_CAT(a,__VA_ARGS__)
#define PRIMITIVE_CAT(a,...)	a ## __VA_ARGS__
#define MULT16_32_QCOEFF	CAT(MULT16_32_Q, COEFF_SHIFT)
#else
typedef double coeff_t;
typedef double data_t;

#define QCONST16(x,bits)	(x)
#define QCONST32(x,bits)	(x)
#define SHR32(a,shift)		(a)
#define PSHR32(a,shift)		(a)
#define MULT16_32_QCOEFF(a,b)	((a)*(b))
#endif

typedef struct
{
	coeff_t num[3];
	coeff_t den[3];
}sIIRcoefficients;

UINT8  calc_coeffs_for_voc(CALIB_AUDIO_IIR_EQ_T *eq, int samplerate, int itf);
sIIRcoefficients *get_coeffs_point(void);
#if 0
void clean_iir_history(void);

void init_iir_history(int itf, int fs, signed char *gain, double *qual,
		unsigned short *freq);

int iir(int itf, int type, short *data, int length, int nch, int clean_history);
#endif
UINT8  calc_coeffs_for_call_voc(CALIB_AUDIO_IIR_EQ_T *eq, int samplerate, int itf);

#endif
