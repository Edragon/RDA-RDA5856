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


#ifndef SHEEN_VC_DEBUG
#include "global.h"
#endif

#ifdef MP4_3GP_SUPPORT

#include "isomedia_dev.h"

uint32 gf_m4a_get_profile(GF_M4ADecSpecInfo *cfg)
{
	switch (cfg->base_object_type) {
	case 2: /*AAC LC*/
		if (cfg->nb_chan<=2) return (cfg->base_sr<=24000) ? 0x28 : 0x29; /*LC@L1 or LC@L2*/
		return (cfg->base_sr<=48000) ? 0x2A : 0x2B; /*LC@L4 or LC@L5*/
	case 5: /*HE-AAC*/
		if (cfg->nb_chan<=2) return (cfg->base_sr<=24000) ? 0x2C : 0x2D; /*HE@L2 or HE@L3*/
		return (cfg->base_sr<=48000) ? 0x2E : 0x2F; /*HE@L4 or HE@L5*/
	/*default to HQ*/
	default:
		if (cfg->nb_chan<=2) return (cfg->base_sr<24000) ? 0x0E : 0x0F; /*HQ@L1 or HQ@L2*/
		return 0x10; /*HQ@L3*/
	}
}

GF_Err gf_m4a_get_config(int8 *dsi, uint32 dsi_size, GF_M4ADecSpecInfo *cfg)
{
	GF_BitStream *bs;
	memset(cfg, 0, sizeof(GF_M4ADecSpecInfo));
	if (!dsi || !dsi_size || (dsi_size<2) ) return GF_NON_COMPLIANT_BITSTREAM;
	bs = gf_bs_new(dsi, dsi_size, GF_BITSTREAM_READ);

	cfg->base_object_type = gf_bs_read_int(bs, 5);
	cfg->base_sr_index = gf_bs_read_int(bs, 4);
	if (cfg->base_sr_index == 0x0F) {
		cfg->base_sr = gf_bs_read_int(bs, 24);
	} else {
		cfg->base_sr = GF_M4ASampleRates[cfg->base_sr_index];
	}
	cfg->nb_chan = gf_bs_read_int(bs, 4);
	if (cfg->base_object_type==5) {
		cfg->has_sbr = 1;
		cfg->sbr_sr_index = gf_bs_read_int(bs, 4);
		if (cfg->sbr_sr_index == 0x0F) {
			cfg->sbr_sr = gf_bs_read_int(bs, 24);
		} else {
			cfg->sbr_sr = GF_M4ASampleRates[cfg->sbr_sr_index];
		}
		cfg->sbr_object_type = gf_bs_read_int(bs, 5);
	}

	gf_bs_align(bs);
	if (gf_bs_available(bs)>=2) {
		uint32 sync = gf_bs_read_int(bs, 11);
		if (sync==0x2b7) {
			cfg->sbr_object_type = gf_bs_read_int(bs, 5);
			cfg->has_sbr = gf_bs_read_int(bs, 1);
			if (cfg->has_sbr) {
				cfg->sbr_sr_index = gf_bs_read_int(bs, 4);
				if (cfg->sbr_sr_index == 0x0F) {
					cfg->sbr_sr = gf_bs_read_int(bs, 24);
				} else {
					cfg->sbr_sr = GF_M4ASampleRates[cfg->sbr_sr_index];
				}
			}
		}
	}
	cfg->audioPL = gf_m4a_get_profile(cfg);
	gf_bs_del(bs);
	return GF_OK;
}

#endif
