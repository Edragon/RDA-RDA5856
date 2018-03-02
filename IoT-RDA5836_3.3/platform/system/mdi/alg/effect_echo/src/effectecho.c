/*

Echo effect             

Copyright (c) 2017, RDA microelectronics, Inc.

Author: Shunzhou Ye

*/
#ifdef KALAOK_SUPPORT
#include "Effectecho.h"
#include "cs_types.h"


typedef struct
{
	Shortword histLen;
	Shortword histPos;
	Shortword  decay;
	Shortword *history; 

} effectecho_priv_t;

static effectecho_priv_t echo_priv = {0, 0, 0, NULL};

void Effectecho_init(Shortword Maxdelaysamle, Shortword *buf)
{
	echo_priv.histLen = Maxdelaysamle;
	echo_priv.histPos = 0;
	echo_priv.decay = 0;
    echo_priv.history = buf;
    
	memset(echo_priv.history, 0, sizeof(Shortword) * Maxdelaysamle );
}

void Effectecho_config(effectecho_param *effectecho_control, Flag mSampleRate, Shortword Maxdelaysamle)
{
    mSampleRate = (mSampleRate + 512) >> 10;//approximately equal to mSampleRate /= 1000;
	echo_priv.decay = effectecho_control ->decay;
	echo_priv.histLen = effectecho_control->delay*mSampleRate;
	
	if(echo_priv.histLen > Maxdelaysamle)
	{
		echo_priv.histLen = Maxdelaysamle;
	}
}

void Effectecho_destory(void)
{
    //if(echo_priv.history != NULL)
    //{
	//    COS_Free(echo_priv.history);
    //}
    echo_priv.history = NULL;
}

void Effectecho_process(Shortword *input, Shortword *output, Shortword framelen)
{
	Shortword *ibuf = input;
	Shortword *obuf = output;
	int i;

	for ( i = 0; i < framelen; i++, echo_priv.histPos++)
	{
		if (echo_priv.histPos == echo_priv.histLen)
		{
		 	echo_priv.histPos = 0;
		}

		echo_priv.history[echo_priv.histPos] = obuf[i] = add(mult(echo_priv.history[echo_priv.histPos], echo_priv.decay),ibuf[i]);
		
		//priv->history[priv->histPos] = obuf[i] = ibuf[i] + priv->history[priv->histPos] * priv->decay;
	}
		
}
#endif

