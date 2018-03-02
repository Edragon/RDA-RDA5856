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
//#include "../include/mpeg4_odf.h"


/************************************************************
		Object Descriptors Functions
************************************************************/

GF_Err gf_odf_desc_read(int8 *raw_desc, uint32 descSize, GF_Descriptor * *outDesc)
{
	GF_Err e;
	uint32 size;
	GF_BitStream *bs;

	if (!raw_desc || !descSize) return GF_BAD_PARAM;

	bs = gf_bs_new((uint8*)raw_desc, (u64) descSize,(uint8)GF_BITSTREAM_READ);
	if (!bs) return GF_OUT_OF_MEM;

	size = 0;
	e = gf_odf_parse_descriptor(bs, outDesc, &size);
	//the size dosn't have the header in it
	size += gf_odf_size_field_size(size);

	if (size != descSize) {
		//if (*outDesc) gf_odf_delete_descriptor(*outDesc);
		*outDesc = NULL;
		e = GF_ODF_INVALID_DESCRIPTOR;
	}


	gf_bs_del(bs);
	return e;
}

GF_Descriptor *gf_odf_new_esd()
{
	//diag_printf("mmc_MemMalloc .... gf_odf_new_esd \n");
	GF_ESD *newDesc = (GF_ESD *) mmc_MemMalloc(sizeof(GF_ESD));
	if (!newDesc) return NULL;
	memset(newDesc, 0, sizeof(GF_ESD));
	newDesc->tag = GF_ODF_ESD_TAG;
	return (GF_Descriptor *) newDesc;
}

GF_Err AddDescriptorToESD(GF_ESD *esd, GF_Descriptor *desc)
{
	if (!esd || !desc) return GF_BAD_PARAM;

	switch (desc->tag) {
	case GF_ODF_DCD_TAG:
		if (esd->decoderConfig) return GF_ODF_INVALID_DESCRIPTOR;
		esd->decoderConfig = (GF_DecoderConfig *) desc;
		break;

	case GF_ODF_SLC_TAG:
		if (esd->slConfig) return GF_ODF_INVALID_DESCRIPTOR;
		esd->slConfig = (GF_SLConfig *) desc;
		break;


	default:
		if ( (desc->tag >= GF_ODF_EXT_BEGIN_TAG) &&
			(desc->tag <= GF_ODF_EXT_END_TAG) ) {
		;//	return gf_list_add(esd->extensionDescriptors, desc);
		}
		;//gf_odf_delete_descriptor(desc);
		return GF_OK;
	}

	return GF_OK;
}

GF_Err gf_odf_read_esd(GF_BitStream *bs, GF_ESD *esd, uint32 DescSize)
{
	GF_Err e = GF_OK;
	uint32 ocrflag, urlflag, streamdependflag, tmp_size, nbBytes;

	if (! esd) return GF_BAD_PARAM;

	nbBytes = 0;

	esd->ESID = gf_bs_read_int(bs, 16);
	streamdependflag = gf_bs_read_int(bs, 1);
	urlflag = gf_bs_read_int(bs, 1);
	ocrflag = gf_bs_read_int(bs, 1);
	esd->streamPriority = gf_bs_read_int(bs, 5);
	nbBytes += 3;

	while (nbBytes < DescSize) {
		GF_Descriptor *tmp = NULL;
		e = gf_odf_parse_descriptor(bs, &tmp, &tmp_size);
		/*fix for iPod files*/
		if (e==GF_ODF_INVALID_DESCRIPTOR) {
			nbBytes += tmp_size;
			if (nbBytes>DescSize) return e;
			gf_bs_read_int(bs, DescSize-nbBytes);
			return GF_OK;
		}
		if (e) return e;
		if (!tmp) return GF_ODF_INVALID_DESCRIPTOR;
		e = AddDescriptorToESD(esd, tmp);
		if (e) return e;
		nbBytes += tmp_size + gf_odf_size_field_size(tmp_size);
		
		//apple fix
		if (!tmp_size) nbBytes = DescSize;

	}
	if (DescSize != nbBytes) return GF_ODF_INVALID_DESCRIPTOR;
	return e;

}

GF_Err gf_odf_read_dcd(GF_BitStream *bs, GF_DecoderConfig *dcd, uint32 DescSize)
{
	GF_Err e;
	uint32 reserved, tmp_size, nbBytes = 0;
	if (! dcd) return GF_BAD_PARAM;

	dcd->objectTypeIndication = gf_bs_read_int(bs, 8);
	dcd->streamType = gf_bs_read_int(bs, 6);
	dcd->upstream = gf_bs_read_int(bs, 1);
	reserved = gf_bs_read_int(bs, 1);
	dcd->bufferSizeDB = gf_bs_read_int(bs, 24);
	dcd->maxBitrate = gf_bs_read_int(bs, 32);
	dcd->avgBitrate = gf_bs_read_int(bs, 32);
	nbBytes += 13;

	while (nbBytes < DescSize) {
		GF_Descriptor *tmp = NULL;
		e = gf_odf_parse_descriptor(bs, &tmp, &tmp_size);
		if (e) return e;
		if (!tmp) return GF_ODF_INVALID_DESCRIPTOR;
		switch (tmp->tag) {
		case GF_ODF_DSI_TAG:
			if (dcd->decoderSpecificInfo) {
				;//gf_odf_delete_descriptor(tmp);
				return GF_ODF_INVALID_DESCRIPTOR;
			}
			dcd->decoderSpecificInfo = (GF_DefaultDescriptor *) tmp;
			break;
		default:
			;//gf_odf_delete_descriptor(tmp);
			break;
		}
		nbBytes += tmp_size + gf_odf_size_field_size(tmp_size);
	}
	if (DescSize != nbBytes) return GF_ODF_INVALID_DESCRIPTOR;
	return GF_OK;
}

GF_Err gf_odf_read_default(GF_BitStream *bs, GF_DefaultDescriptor *dd, uint32 DescSize)
{
	uint32 nbBytes = 0;
	if (! dd) return GF_BAD_PARAM;

	dd->dataLength = DescSize;
	dd->data = NULL;
	if (DescSize) {
			//diag_printf("mmc_MemMalloc .... gf_odf_read_default \n");
		dd->data = (int8*)mmc_MemMalloc(dd->dataLength);
		if (! dd->data) return GF_OUT_OF_MEM;
		memset(dd->data, 0, dd->dataLength );
		gf_bs_read_data(bs, (uint8*)dd->data, dd->dataLength);
		nbBytes += dd->dataLength;
	}
	if (nbBytes != DescSize) return GF_ODF_INVALID_DESCRIPTOR;
	return GF_OK;
}

GF_Descriptor *gf_odf_new_default()
{
	//diag_printf("mmc_MemMalloc .... gf_odf_new_default \n");
	GF_DefaultDescriptor *newDesc = (GF_DefaultDescriptor *) mmc_MemMalloc(sizeof(GF_DefaultDescriptor));
	if (!newDesc) return NULL;

	newDesc->dataLength = 0;
	newDesc->data = NULL;
	//set it to the Max allowed
	newDesc->tag = GF_ODF_USER_END_TAG;
	return (GF_Descriptor *) newDesc;
}


#endif

