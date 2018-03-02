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
//Packets Loss Concealment                                              /
/////////////////////////////////////////////////////////////////////////
#ifndef _CVSD_PLC_H_
#define _CVSD_PLC_H_
typedef float Float;

void dofe(short *s); /* synthesize speech for erasure */
void addtohistory(short *s); /* add a good frame to history buffer */
void scalespeech(short *out);
void getfespeech(short *out, int sz);
void savespeech(short *s);
int findpitch();
void Foverlapadd(int *l, int *r, int *o, int cnt);
void Soverlapadd(short *l, short *r, short *o, int cnt);
void overlapaddatend(short *s, short *f, int cnt);
void convertsf(short *f, int *t, int cnt);
void convertfs(int *f, short *t, int cnt);
void copyf(int *f, int *t, int cnt);
void copys(short *f, short *t, int cnt);
void zeros(short *s, int cnt);
void PlcInit(int frameLen);
void PlcDeInit(void);

#endif

