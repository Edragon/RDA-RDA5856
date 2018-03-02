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

#ifndef _GF_ISOMEDIA_H_
#define _GF_ISOMEDIA_H_


#include "mpeg4_odf.h"

enum
{
	/*reserved, internal use in the lib. Indicates the track complies to MPEG-4 system
	specification, and the usual OD framework tools may be used*/
	GF_ISOM_SUBTYPE_MPEG4		= GF_FOUR_CHAR_INT( 'M', 'P', 'E', 'G' ),
	
	/*reserved, internal use in the lib. Indicates the track is of GF_ISOM_SUBTYPE_MPEG4
	but it is encrypted.*/
	GF_ISOM_SUBTYPE_MPEG4_CRYP	= GF_FOUR_CHAR_INT( 'E', 'N', 'C', 'M' ),

	/*AVC/H264 media type - not listed as an MPEG-4 type, ALTHOUGH this library automatically remaps
	GF_AVCConfig to MPEG-4 ESD*/
	GF_ISOM_SUBTYPE_AVC_H264		= GF_FOUR_CHAR_INT( 'a', 'v', 'c', '1' ),

	/*3GPP(2) extension subtypes*/
	GF_ISOM_SUBTYPE_3GP_H263		= GF_FOUR_CHAR_INT( 's', '2', '6', '3' ),
	GF_ISOM_SUBTYPE_3GP_AMR		= GF_FOUR_CHAR_INT( 's', 'a', 'm', 'r' ),
	GF_ISOM_SUBTYPE_3GP_AMR_WB	= GF_FOUR_CHAR_INT( 's', 'a', 'w', 'b' ),
	GF_ISOM_SUBTYPE_3GP_EVRC		= GF_FOUR_CHAR_INT( 's', 'e', 'v', 'c' ),
	GF_ISOM_SUBTYPE_3GP_QCELP	= GF_FOUR_CHAR_INT( 's', 'q', 'c', 'p' ),
	GF_ISOM_SUBTYPE_3GP_SMV		= GF_FOUR_CHAR_INT( 's', 's', 'm', 'v' )
};

#endif
#endif

