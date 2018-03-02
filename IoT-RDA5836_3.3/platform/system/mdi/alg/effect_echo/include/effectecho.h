/*

Echo effect             

Copyright (c) 2017, RDA microelectronics, Inc.

Author: Shunzhou Ye

*/
#ifndef _Effectecho_H
#define _Effectecho_H

//#define SAMPLING_RATE 44100
#define FRAMELEN 1024
#define Mono 1
#define Stereo 2
#define Single 0
#define Left 1
#define Right 2

#define ECHO_DELAY_MAX 500

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <string.h>

#include "Mathhalf.h"
#include "Typedef.h"

struct effectecho_param
{
	Shortword delay;
	Shortword decay;
	//float delay;
	//float decay;
};
typedef struct effectecho_param effectecho_param;

void Effectecho_init(Shortword Maxdelaysamle, Shortword *buf);
void Effectecho_config(effectecho_param *effectecho_control, Flag mSampleRate, Shortword Maxdelaysamle);
void Effectecho_destory(void);
void Effectecho_process(Shortword *input, Shortword *output, Shortword framelen);

#endif
