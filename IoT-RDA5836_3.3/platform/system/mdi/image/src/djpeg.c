/*
 * djpeg.c
 *
 * Copyright (C) 1991-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a command-line user interface for the JPEG decompressor.
 * It should work on any system with Unix- or MS-DOS-style command lines.
 *
 * Two different command line styles are permitted, depending on the
 * compile-time switch TWO_FILE_COMMANDLINE:
 *	djpeg [options]  inputfile outputfile
 *	djpeg [options]  [inputfile]
 * In the second style, output is always to standard output, which you'd
 * normally redirect to a file or pipe to some other program.  Input is
 * either from a named file or from standard input (typically redirected).
 * The second style is convenient on Unix but is unhelpful on systems that
 * don't support pipes.  Also, you MUST use the first style if your system
 * doesn't do binary I/O to stdin/stdout.
 * To simplify script writing, the "-outfile" switch is provided.  The syntax
 *	djpeg [options]  -outfile outputfile  inputfile
 * works regardless of which command line style is used.
 */

#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */
#include <ctype.h>		/* to declare isprint() */
#include "mmi_jpeg_dec.h"
#ifndef SHEEN_VC_DEBUG
//#include "mmi_trace.h"
//#include "debuginitdef.h"
#else
#include "voc2_library.h"
#include "voc2_define.h"
#include "voc2_simulator.h"
#include "MAP_ADDR.h"
#include "jpegdec2.h"
#endif
#include "setjmp.h"
#include "jdct.h"

#if defined(MEDIA_VOCVID_SUPPORT)
#include "hal_voc.h"
#include "mdi_alg_common.h"
#include "mmc.h"
#include "mcip_debug.h"
#include "vpp_audiojpeg_dec.h"
#include "sxr_tls.h"
#undef TRUE
#define TRUE 1
#define VOC_DO
#ifndef SHEEN_VC_DEBUG
VocWorkStruct vocWorkState;
#endif
extern void jinit_voc_decoder(j_decompress_ptr cinfo);
#endif

/*
 * The main program.
 */
jmp_buf						Jpeg_jmpbuf;
BOOL						Jpeg_jmpbuf_is_used=FALSE;
void (*callBackInJpgDec)();//for video player. sheen
EXTERN(long) jdiv_round_up JPP((long a, long b));

GLOBAL(void)
jpeg_mem_term (void);



#ifdef SHEEN_VC_DEBUG
ERROR_TYPE MMF_JpegDecodePicture(STREAM input_file ,char* outbuf,unsigned int outbuf_len,
								 short width,short height,
								 short start_x,short start_y,
								 short end_x,short end_y,
								 DEC_UNIT* decUnit,short kind,void (*callBackFun)(void),
								 UINT16 img_id);

int MMI_FS_Read(FS_HANDLE FileHandle, void *DataPtr, INT32 Lenth, INT32*ReadLen)
{
	*ReadLen=fread(DataPtr, Lenth, 1, FileHandle);
}

#pragma pack(1)
/*BMP文件头*/
typedef struct
{
	U16 bfType;          //文件类型，2Bytes，值为ox4D42(字符BM)
	UINT32 bfSize;         // 4Bytes,整个文件的大小，单位为字节
	U16 bfReserved1;     //保留，2Bytes，置位为0
	U16 bfReserved2;     //保留，2Bytes，置位为0
	UINT32 bfOffBits;      //从文件头开始到实际图像数据之间的字节偏移量，4Bytes
} BITMAPFILEHEADER;


/*BMP文件信息头*/
typedef struct  
{
	UINT32 biSize;             //BMP信息头大小(字节数)，4Bytes
	INT32 biWidth;             //图像的宽度，以像素为单位，4Bytes
	INT32 biHeight;            //图像的高度，以像素为单位，4Bytes
	U16 biPlanes;            //说明目标设备位面数，固定值为1，2Bytes
	U16 biBitCount;          //每像素比特数，2Bytes，其值为：1、4、8、16、24、32
	UINT32 biCompression;      //图像数据压缩方法，4Bytes
	UINT32 biSizeImage;        //图像的大小(字节数)，4Bytes，该数必须是4的整数倍，当使用BI_RGB格式时，可置为0
	INT32 biXPelsPerMeter;     //水平分辨率，4Bytes，单位：像素/米
	INT32 biYPelsPerMeter;     //垂直分辨率，4Bytes，单位：像素/米
	UINT32 biClrUsed;          //实际使用的色表中的颜色索引数,4Bytes
	UINT32 biClrImportant;     //对图像显示有重要影响的颜色索引的数目,4Bytes
} BITMAPINFOHEADER;
#pragma pack()
unsigned char RGB565Pal[16] = {0x00, 0xf8, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//rgb565 需要写此16字节


//COMMON CODE
INT32 G_VppCommonDecCode[] = {
#include "E:\projects\gallite_hx_simu\platform_simu\voc\simus\audiojpegH263\work\audiojpeg_dec_common.tab"
};

//OTHER CODE
INT32 G_VppJPEGDec2Code[] = {
#include "E:\projects\gallite_hx_simu\platform_simu\voc\simus\audiojpegH263\work\jpegdec2.tab"
};
//const table
INT32 G_VppJPEGDec2Consty[] = {
#include "E:\projects\gallite_hx_simu\platform_simu\voc\simus\audiojpegH263\work\vpp_jpeg_dec2_const_y.tab"
};

void VoC_BCPU_IRQ_handler(void) 
{}

int voc_loop_flag;//1 work 0 not work
void VoC_XCPU_IRQ_handler(void) 
{
	//sign voc loop continue or not.
	if(voc_loop_flag==1)
		VoC_set_event(CFG_WAKEUP_SOF0);
	voc_loop_flag=1-voc_loop_flag;
	//printf("XCPU IRQ\n");
}

void voc_init(void) //for debug
{
	HAL_ERR_T status;
	HAL_VOC_CFG_T cfg;
	
	cfg.vocCode              = G_VppCommonDecCode;//will be voc code tab
	cfg.vocCodeSize          = sizeof(G_VppCommonDecCode);
	cfg.pcVal                = 0;
	cfg.vocIrqHandler        = NULL;
	cfg.eventMask.wakeupIfc0 = 0;
	cfg.eventMask.wakeupIfc1 = 0;
	cfg.eventMask.wakeupDmae = 0;
	cfg.eventMask.wakeupDmai = 0;
	cfg.eventMask.wakeupSof0 = 0;
	cfg.eventMask.wakeupSof1 = 0;
	
	status = hal_VocOpen(&cfg);//load common code.
	
	//VoC_set_mask( (short) (CFG_WAKEUP_SOF0 | CFG_WAKEUP_DMAI));
	
	if (HAL_ERR_RESOURCE_BUSY == status)
	{
		return HAL_ERR_RESOURCE_BUSY;
	}
	else if (HAL_ERR_RESOURCE_RESET == status)
	{
		INT32 *pCode = hal_VocGetPointer(H263_Zoom_Code_ExternalAddr_addr);
		INT32 *pTable = hal_VocGetPointer(H263_Zoom_ConstX_ExternalAddr_addr);
		INT32 tabsize=sizeof(G_VppJPEGDec2Consty);
		
		*pCode    = hal_VocGetDmaiPointer((INT32*)G_VppJPEGDec2Code, HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
		*pTable   = hal_VocGetDmaiPointer((INT32*)G_VppJPEGDec2Consty, HAL_VOC_DMA_READ,HAL_VOC_DMA_BURST,HAL_VOC_DMA_B2S_NO);
	}
	
	//vpp_AudioJpegDecSetCurMode(-1, -1);
	{
		INT16 *pMode;
		pMode=hal_VocGetPointer(Current_Audio_Mode_addr);
		*pMode=-1;
		pMode+=1;
		*pMode=-1;
	}
	
	//vpp_AudioJpegDecSetInputMode(MMC_MPEG4_DECODE);
	{
		INT16 *pMode;
		pMode=hal_VocGetPointer(Input_mode_addr);
		*pMode = 18;
		pMode=hal_VocGetPointer(Flag_need_bluetooth_ADDR);
		*pMode=0;
	}
	
	reg_SP16 = RAM_X_MAX-1;
	reg_SP32 = RAM_Y_MAX-2;
	VoC_lw16i(SP16,SP_16_ADDR);
	VoC_lw16i(SP32,SP_32_ADDR);
}



int main (int argc, char **argv)
{
	DEC_UNIT decUnit;
	short kind = 2;//kind  =0   jpeg in buf 
                   //kind  =1   jpeg in file
                   //kind  =2   jpeg in buf and output yuv 
	short dest_width=320, dest_height=240;
	short start_x=0, start_y=0;
	short end_x=dest_width-1, end_y=dest_height-1;
	int bufsize=(end_x- start_x+1)*(end_y- start_y+1)*2;
	char* outbuffer= (char*)malloc(bufsize);
	
	STREAM output_file;
	STREAM input_file;
    char*  inbuffer;
	BITMAPFILEHEADER *mypbmfh;
	BITMAPINFOHEADER *mypbmih;
	int res;
	int i;

	voc_loop_flag=1;
	voc_init();

	if (kind==0)
	{
		unsigned int len;
		
		
		if ((input_file = fopen("F:\\测试文件\\jpg\\vidrec.jpg", READ_BINARY)) == NULL) {
			printf(stderr,"can't open \n");
			exit(EXIT_FAILURE);
		}
		
		fseek(input_file,0,SEEK_END);
		len = ftell(input_file);
		fseek(input_file,0,SEEK_SET);
		inbuffer = malloc(len);
		if (inbuffer==NULL)
		{
			exit(EXIT_FAILURE);
		}
		fread(inbuffer,1,len,input_file);
		//fclose(input_file);
	}
	else if (kind==2)
	{
		unsigned int len;
		//avi_sof0_422.jpg
		if ((input_file = fopen("F:\\测试文件\\jpg\\news_sof0_411.jpg", READ_BINARY)) == NULL) {
			printf(stderr,"can't open \n");
			exit(EXIT_FAILURE);
		}
		fseek(input_file,0,SEEK_END);
		res = ftell(input_file);
		fseek(input_file,0,SEEK_SET);
		
		inbuffer = malloc(100*1024);
		if (inbuffer==NULL)
		{
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		inbuffer = "F:\\测试文件\\jpg\\avi.jpg";
	}

	
	  /* Open the output file. */
    if ((output_file = fopen("F:\\测试文件\\jpg\\test1.yuv", WRITE_BINARY)) == NULL) {
      fprintf(stderr, "can't open %s\n", argv[2]);
      exit(EXIT_FAILURE);
    }

	i=0;
	while(i<1)
	{
		if (kind==2)
		{
			//fread(&res,1,4,input_file);
			fread(inbuffer,1,res,input_file);
		}

		res=MMF_JpegDecodePicture(inbuffer,outbuffer,bufsize,
			dest_width,dest_height,
			start_x,start_y,
			end_x,end_y,
			&decUnit,kind,0);
		if(res!=FUNC_OK)
			printf("ERROR! res=%d\n", res);
		printf("kind = %d , dst_w=%d ,dst_h=%d \n",kind,decUnit.width,decUnit.height);
		
		VoC_counter_print(1);

	if(kind!=2)
	{
		//bmp header
		mypbmfh = ( BITMAPFILEHEADER *)malloc(14);
		mypbmfh->bfType = 0x4d42;
		mypbmfh->bfReserved1 = 0;
		mypbmfh->bfReserved2 = 0;
		mypbmfh->bfSize = 70 + decUnit.width*decUnit.height*2;
		mypbmfh->bfOffBits = 70;
		
		mypbmih = (BITMAPINFOHEADER *)malloc(40);
		mypbmih->biSize = 40;
		mypbmih->biWidth =decUnit.width;
		mypbmih->biHeight =-((long)decUnit.height);//加-号可以使bmp图像上下倒转显示方式（原本bmp存储时上下倒转）
		mypbmih->biPlanes = 1;
		mypbmih->biBitCount = 16;
		mypbmih->biCompression = 3;//rgb565 写0颜色不正确。
		mypbmih->biSizeImage= decUnit.width*decUnit.height*2;
		mypbmih->biXPelsPerMeter= 0;
		mypbmih->biYPelsPerMeter= 0;
		mypbmih->biClrUsed= 0;
		mypbmih->biClrImportant= 0;
		
		fwrite(mypbmfh, 14, 1, output_file);
		fwrite(mypbmih, 40, 1, output_file);
		fwrite(&RGB565Pal, 16, 1, output_file);//rgb565 需要写此16字节
		//bmp header end
	}

		if(kind!=2)
			fwrite(outbuffer,decUnit.width*decUnit.height*2,1,output_file);
		else
			fwrite(outbuffer,decUnit.width*decUnit.height*3/2,1,output_file);

		printf("frame %d\n",i);
		i++;
	}
	free(outbuffer);
	fclose(output_file);
	fclose(input_file);

	if (kind!=1)
	{
		free(inbuffer);
	}

	return 0;
}
#endif


#define JPEG_IMAGE_BUFFER_SIZE      5

typedef struct jpg_img_tag {
    UINT16 width;
    UINT16 height;
    UINT16 image_id;
    UINT16 pixcel[1];
} jpeg_image_t;

PRIVATE jpeg_image_t *jpeg_buffer[JPEG_IMAGE_BUFFER_SIZE];

/***************************************************************************************
sheen  2011.6.13

input_file:		set input file handle or buffer address
outbuf:			set output buffer address
outbuf_len:		output buffer size, not less than display area.
width:			set scaled width of the full output image.
height:			set scaled height of the full output image.
start_x:		set x start position of the display area in scaled image.(0==first)
end_x:			set x end position of the display area in scaled image.
start_y:		set y start position of the display area in scaled image.(0==first)
end_y:			set y end position of the display area in scaled image.
kind:			set kind of the input&output format.(default=0 input jpeg in buf & output RGB565)
callBackFun:	set callback function for video player.(default NULL)
decUnit->width:	output display image width
decUnit->height: output display image height
decUnit->len:	output display image data size.

!!! NOTE:
	1. input width/height will be adjust inside with aspect ratio also limit within original width/height.
	2. start_x/end_x/start_y/end_y sign the real output area also be adjust with width/height.
	3. set output area within LCD width/height.

****************************************************************************************/
ERROR_TYPE MMF_JpegDecodePicture(STREAM input_file ,char* outbuf,unsigned int outbuf_len,
								 short width,short height,
								 short start_x,short start_y,
								 short end_x,short end_y,
								 DEC_UNIT* decUnit,short kind,void (*callBackFun)(void),
								 UINT16 img_id)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  djpeg_dest_ptr dest_mgr = NULL;
  ERROR_TYPE ret;
  UINT i;
#ifdef VOC_DO
  UINT32 cs_status;
#endif
  //FILE * output_file;
  //JDIMENSION num_scanlines;
  int jmpret; 
  callBackInJpgDec=callBackFun;//sheen
   
	mmi_trace(1,"MMF_JpegDecodePicture");

	if(kind!=2)
	{
		mmi_trace(1,"input: \n input_file=0x%x outbuf=0x%x outbuf_len=%d width =%d height = %d\n",
			input_file, outbuf, outbuf_len, width,height);
		mmi_trace(1,"start_x=%d start_y=%d end_x=%d end_y =%d kind = %d\n",
			start_x, start_y, end_x, end_y,kind);

	}

    //if(0 || (1&&(0||0||0||0||1||1||0))||(0
	if (!outbuf ||
		(kind !=2 && 
		(start_x< 0 || start_y<0 || 
		start_x > end_x || start_y > end_y ||
		//end_x+1 > width || end_y+1 >height ||
		(end_x-start_x+1)*(end_y-start_y+1) > outbuf_len))
		||(kind==2 && outbuf_len< (width*height*3)>>1))
	{
		mmi_trace(1,"Error input\n");
        hal_HstSendEvent(SYS_EVENT, outbuf);
        hal_HstSendEvent(SYS_EVENT, kind);
        hal_HstSendEvent(SYS_EVENT, start_x);
        hal_HstSendEvent(SYS_EVENT, start_y);
        hal_HstSendEvent(SYS_EVENT, end_x);
        hal_HstSendEvent(SYS_EVENT, end_y);
        hal_HstSendEvent(SYS_EVENT, width);
        hal_HstSendEvent(SYS_EVENT, height);
        hal_HstSendEvent(SYS_EVENT, outbuf_len);
        
        hal_HstSendEvent(SYS_EVENT, 0x12280101);
        while(1);
		return ERROR_WRONG_SIZE;
	}

    if(img_id != 0)
    {
        for(i=0; i<JPEG_IMAGE_BUFFER_SIZE; i++)
        {
            if(jpeg_buffer[i] != 0 && jpeg_buffer[i]->image_id == img_id)
            {
                jpeg_image_t *img = jpeg_buffer[i];
                memcpy(outbuf, img->pixcel+start_y*width, width*height*2);
                // move this image to first position
                while(i>0)
                {
                    jpeg_buffer[i] = jpeg_buffer[i-1];
                    i --;
                };
                jpeg_buffer[0] = img;
                return FUNC_OK;
            }
        }
    }

	memset(&cinfo,0,sizeof(struct jpeg_decompress_struct));
  /* Initialize the JPEG decompression object with default error handling. */
  //JPEG_TRY
	                            
	MMI_ASSERT(!Jpeg_jmpbuf_is_used);         
	Jpeg_jmpbuf_is_used = TRUE;               
	jmpret = setjmp (Jpeg_jmpbuf);            
	if(jmpret == 0 )                         
	{
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		/* Add some application-specific error messages (from cderror.h) */
		//  jerr.addon_message_table = cdjpeg_message_table;
		//  jerr.first_addon_message = JMSG_FIRSTADDONCODE;
		//  jerr.last_addon_message = JMSG_LASTADDONCODE;

		/* Insert custom marker processor for COM and APP12.
		* APP12 is used by some digital camera makers for textual info,
		* so we provide the ability to display it as text.
		* If you like, additional APPn marker types can be selected for display,
		* but don't try to override APP0 or APP14 this way (see libjpeg.doc).
		*/
		jpeg_set_marker_processor(&cinfo, JPEG_COM);
		jpeg_set_marker_processor(&cinfo, JPEG_APP0+12);


		/* Scan command line to find file names. */
		/* It is convenient to use just one switch-parsing routine, but the switch
		* values read here are ignored; we will rescan the switches after opening
		* the input file.
		* (Exception: tracing level set here controls verbosity for COM markers
		* found during jpeg_read_header...)
		*/
		//input parameters:
		cinfo.is_yuv_format = kind;//kind  =0   jpeg in buf 
		     //kind  =1   jpeg in file
		     //kind  =2   jpeg in buf and output yuv 
		cinfo.dest_width  = width;
		cinfo.dest_height = height;

		if (outbuf==NULL)
		{
			ERREXIT(&cinfo, JERR_BMP_COLORSPACE);
		}
		cinfo.dest_buffer = outbuf;
		if(kind==2)
		{
			cinfo.dest_bufferU= cinfo.dest_buffer + width*height;
			cinfo.dest_bufferV = cinfo.dest_bufferU + (width*height>>2);
		}
		cinfo.dct_method = JDCT_IFAST;
		decUnit->width = 0;
		decUnit->height= 0;
		decUnit->len   = 0;

		if (kind==1)
		{
			//input_file = MMI_FS_Open((U8*)filename, FS_READ_ONLY);
			if(input_file<0)
			{
				//Jpeg_jmpbuf_is_used = FALSE;
				//return ERROR_OPENFIAL;
				ERREXIT(&cinfo, 0);
			}

			/* Specify data source for decompression */
			jpeg_stdio_src(&cinfo, input_file);
		}
		else
		{
			/* Specify data source for decompression */
			jpeg_stdio_src(&cinfo, input_file);
		}




		//mmi_trace(1,"MMF_JpegDecodePicture begin to decode:");
		/* Read file header, set default decompression parameters */
		ret=jpeg_read_header(&cinfo, TRUE);
		if(ret!=JPEG_HEADER_OK)
		{
			mmi_trace(1,"jpeg_read_header err= %d",ret);
			ERREXIT(&cinfo, 0);
		}
		/* sheen
		decUnit->width  = cinfo.dest_width;
		decUnit->height = cinfo.dest_height;
		decUnit->len    = cinfo.dest_width*cinfo.dest_height<<1;//byets in rgb565 format
		*/
		//mmi_trace(1,"dst_w=%d,dst_h=%d",decUnit->width,decUnit->height);


		/* Adjust default decompression parameters by re-parsing the options */


		/* Initialize the output module now to let it override any crucial
		* option settings (for instance, GIF wants to force color quantization).
		*/
		dest_mgr = jinit_write_bmp(&cinfo);
		if(!dest_mgr)
		{
			mmi_trace(1,"jinit_write_bmp err");
			ERREXIT(&cinfo, 0);
		}
		//dest_mgr->output_file = output_file;
		if(img_id != 0)
        {      
    		cinfo.dest_width = cinfo.image_width;
            cinfo.dest_height = cinfo.image_height;
            // allocate new image to save image data
            if(jpeg_buffer[JPEG_IMAGE_BUFFER_SIZE-1] != 0)
                COS_Free(jpeg_buffer[JPEG_IMAGE_BUFFER_SIZE-1]);
            for(i=JPEG_IMAGE_BUFFER_SIZE-1;i>0;i--)
                jpeg_buffer[i] = jpeg_buffer[i-1];
            jpeg_buffer[0] = (jpeg_image_t*)COS_Malloc(sizeof(jpeg_image_t)+cinfo.image_width*cinfo.image_height*2);
            jpeg_buffer[0]->image_id = img_id;
            jpeg_buffer[0]->width = cinfo.image_width;
            jpeg_buffer[0]->height = cinfo.image_height;
            cinfo.dest_buffer = jpeg_buffer[0]->pixcel;
        }
		if(kind==2 && (cinfo.dest_width!= cinfo.output_width || cinfo.dest_height!= cinfo.output_height))//sheen
		{
			mmi_trace(1,"dest_width !=output_width %d %d or dest_height != output_height %d %d",cinfo.dest_width,cinfo.output_width,cinfo.dest_height, cinfo.output_height);
			ERREXIT(&cinfo, 0);
		}

		/* Start decompressor */
		ret=jpeg_start_decompress(&cinfo); 
		if(ret!=TRUE)
		{
			mmi_trace(1,"jpeg_start_decompress err");
			ERREXIT(&cinfo, 0);
		}

		/* Process data */
		//  while (cinfo.output_scanline < cinfo.output_height) {
		//    num_scanlines = jpeg_read_scanlines(&cinfo, dest_mgr->buffer,//imcu row :IDCT  YUV->RBG 
		//					dest_mgr->buffer_height);
		//    (*dest_mgr->put_pixel_rows) (&cinfo, dest_mgr, num_scanlines);//output to dest buffer through tmp buffer
		//  }

		mmi_trace(1,"original width =%d height = %d\n",
				cinfo.image_width, cinfo.image_height);

		//for cut area,sheen
		if(img_id == 0 && kind!=2)
		{
			int tmp;
			//keep cut area within dest
			if(end_y-start_y+1 <= cinfo.dest_height && end_y < cinfo.dest_height)
			{
				cinfo.smp_cut_row_start=start_y;
				cinfo.smp_cut_row_end=end_y;
			}
			else
			{
				short len=end_y-start_y+1;

				cinfo.smp_cut_row_end=cinfo.dest_height-1;
				cinfo.smp_cut_row_start=cinfo.dest_height- len;
				if(cinfo.smp_cut_row_start<0)cinfo.smp_cut_row_start=0;
			}

			if(end_x-start_x+1 <= cinfo.dest_width && end_x < cinfo.dest_width)
			{
				cinfo.smp_cut_col_start=start_x;
				cinfo.smp_cut_col_end=end_x;
			}
			else
			{
				short len=end_x-start_x+1;

				cinfo.smp_cut_col_end=cinfo.dest_width-1;
				cinfo.smp_cut_col_start=cinfo.dest_width- len;
				if(cinfo.smp_cut_col_start<0)cinfo.smp_cut_col_start=0;
			}
			
			if(cinfo.dest_height<1 ||
				cinfo.dest_width<1||
				cinfo.max_v_samp_factor<1||
				cinfo.max_h_samp_factor<1)
			{
				mmi_trace(1,"bad header %d %d %d %d", cinfo.dest_height,
				cinfo.dest_width,
				cinfo.max_v_samp_factor,
				cinfo.max_h_samp_factor);
				ERREXIT(&cinfo, 0);
			}

			//calculate MCU position in idct scaled output
			if(cinfo.image_height > DCTSIZE && cinfo.dest_height>1)
			{
				cinfo.MCU_cut_row_start= 
					(cinfo.smp_cut_row_start* cinfo.image_height/cinfo.dest_height)/(cinfo.max_v_samp_factor*DCTSIZE);
				tmp=jdiv_round_up((long)(cinfo.smp_cut_row_end* (cinfo.image_height-1)), (long)(cinfo.dest_height-1));
				if(tmp>=cinfo.image_height)
					tmp=cinfo.image_height-1;
				cinfo.MCU_cut_row_end=
					jdiv_round_up((long)tmp, (long) (cinfo.max_v_samp_factor*DCTSIZE));
				if(cinfo.MCU_cut_row_end+1 > cinfo.MCU_rows_in_scan)cinfo.MCU_cut_row_end=cinfo.MCU_rows_in_scan-1;
			}
			else
			{
				cinfo.MCU_cut_row_start=0;
				cinfo.MCU_cut_row_end=0;
			}

			if(cinfo.image_width > DCTSIZE && cinfo.dest_width>1)
			{
				cinfo.MCU_cut_col_start=
					(cinfo.smp_cut_col_start* cinfo.image_width/cinfo.dest_width)/(cinfo.max_h_samp_factor*DCTSIZE);
				tmp=jdiv_round_up((long)(cinfo.smp_cut_col_end* (cinfo.image_width-1)), (long)(cinfo.dest_width-1));
				if(tmp>=cinfo.image_width)
					tmp=cinfo.image_width-1;
				cinfo.MCU_cut_col_end=
					jdiv_round_up((long)tmp, (long) (cinfo.max_h_samp_factor*DCTSIZE));
				if(cinfo.MCU_cut_col_end+1 > cinfo.MCUs_per_row)cinfo.MCU_cut_col_end=cinfo.MCUs_per_row-1;
			}
			else
			{
				cinfo.MCU_cut_col_start=0;
				cinfo.MCU_cut_col_end=0;
			}
			mmi_trace(1,"adjust dest_width =%d dest_height = %d\n",
				cinfo.dest_width, cinfo.dest_height);
			mmi_trace(1,"start_x=%d start_y=%d end_x=%d end_y =%d \n",
				cinfo.smp_cut_col_start, cinfo.smp_cut_row_start, cinfo.smp_cut_col_end, cinfo.smp_cut_row_end);

		}
		else
		{
			cinfo.smp_cut_row_start=0;
			cinfo.smp_cut_row_end=cinfo.dest_height-1;
			cinfo.smp_cut_col_start=0;
			cinfo.smp_cut_col_end=cinfo.dest_width-1;

			cinfo.MCU_cut_row_start=0;
			cinfo.MCU_cut_row_end=cinfo.MCU_rows_in_scan-1;
			cinfo.MCU_cut_col_start=0;
			cinfo.MCU_cut_col_end=cinfo.MCUs_per_row-1;
		}
		decUnit->width  = cinfo.smp_cut_col_end -cinfo.smp_cut_col_start +1;
		decUnit->height = cinfo.smp_cut_row_end -cinfo.smp_cut_row_start +1;
		decUnit->len    = decUnit->width*decUnit->height<<1;//byets in rgb565 format
		//for cut area,end


		/* Finish decompression and release memory.
		* I must do it in this order because output module has allocated memory
		* of lifespan JPOOL_IMAGE; it needs to finish before releasing memory.
		*/
		if(cinfo.is_yuv_format!=2)
			(*dest_mgr->finish_output) (&cinfo, dest_mgr);//write to file
		else
		{
#ifndef VOC_DO
		(*dest_mgr->finish_output) (&cinfo, dest_mgr);//write to file
#else
//voc
		jinit_voc_decoder(&cinfo);

		//if pmain->buffer > 15360
		//{ big than voc malloc!!!		}
#ifndef SHEEN_VC_DEBUG
		{
			//int tk1,tk2;
			while(*(vocWorkState.pVOC_WORK) ==1/*audio*/ ||(*(vocWorkState.pVOC_WORK) ==2/*video*/ && *(vocWorkState.pVOC_VID) !=0))
			{//wait last voc vid done
				//diag_printf("1 %d %d ",*(vocWorkState.pVOC_WORK), *(vocWorkState.pVOC_VID) );
				//sxr_Sleep(1);
			}

			cs_status=hal_SysEnterCriticalSection();

			*(vocWorkState.pVOC_WORK)=2;//video
			*(vocWorkState.pVOC_VID)=2;//more voc work to do

			hal_SysExitCriticalSection(cs_status);

			//tk1=hal_TimGetUpTime();

			vpp_AVDecSetMode(MMC_JPEG_DECODE, 0, 0);
			hal_VocWakeup(HAL_VOC_WAKEUP_EVENT_0);

			//sendVocReq =0;
			while(*(vocWorkState.pVOC_VID) !=0)
			{//wait last voc vid done
				//audio decode first
				if(vocWorkState.aud_nframe_dec)
					vocWorkState.aud_nframe_dec();
				//display second
				if(vocWorkState.vid_display)
				{
					vocWorkState.vid_display();
					vocWorkState.vid_display= NULL;
					COS_SleepByTick(1);
				}
			}


				//tk2=hal_TimGetUpTime();
				//diag_printf("jpeg voc %d ms, include dis time", (tk2-tk1)*1000/16384);

			vocWorkState.aud_nframe_dec=NULL;
			//aac will cover these ram below.!!!!!!!!!!!!!

			//free voc for audio
			cs_status=hal_SysEnterCriticalSection();
			
			*(vocWorkState.pVOC_WORK)=0;//video
			
			hal_SysExitCriticalSection(cs_status);
		}

#else
		
		VoC_set_event(CFG_WAKEUP_SOF0);//=(1<<4)
#ifdef BINARY_SIM
		VoC_execute(0);//offset 0x00 =Coolsand_AudioDecode_MAIN
#else
		Coolsand_AudioDecode_MAIN();
#endif
#endif
//voc
#endif
		}
		(void) jpeg_finish_decompress(&cinfo);

		//jpeg_mem_term();
		jpeg_destroy_decompress(&cinfo);
		jpeg_mem_term();

        if(img_id != 0)
        {
            memcpy(outbuf, jpeg_buffer[0]->pixcel+start_y*width, width*height*2);
        }
		ret = FUNC_OK;

	}
    //JPEG_CATCH_BEGIN
      else                                  
      {                                     
		ret = WRONG_DECOMPOSE;
		jpeg_destroy_decompress(&cinfo);
      }                          
    //JPEG_CATCH_END
    
    Jpeg_jmpbuf_is_used = FALSE;

  /* Close files, if we opened them */
  //if (output_file != stdout)
  //  fclose(output_file);


  /* All done. */
  // exit(jerr.num_warnings ? EXIT_WARNING : EXIT_SUCCESS);
  return ret;			/* suppress no-return-value warnings */
}
