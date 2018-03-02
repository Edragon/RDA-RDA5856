        .sys_sram_overlay_0
         {
             *h263_idct_mips.o(.text .text.* .sramtext)
             *getbits.o(.text .text.* .sramtext)
             *getblk.o(.text .text.* .sramtext)
             *getvlc.o(.text .text.* .sramtext)
             *recon.o(.text .text.* .sramtext)
             *h263_idct_mips.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *getbits.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *getblk.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *getvlc.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *recon.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *h263_idct_mips.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *getbits.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *getblk.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *getvlc.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *recon.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .sys_sram_overlay_1
         {
             *mpeg4_idct_mips.o(.text .text.* .sramtext)
             *mpeg4_decoder.o(.text .text.* .sramtext)
             *interpolate8x8.o(.text .text.* .sramtext)
             *mbcoding.o(.text .text.* .sramtext)
             *mbprediction.o(.text .text.* .sramtext)
             *mpeg4_bitstream.o(.text .text.* .sramtext)
             *quant_h263.o(.text .text.* .sramtext)
             *mpeg4_idct_mips.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *mpeg4_decoder.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *interpolate8x8.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *mbcoding.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *mbprediction.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *mpeg4_bitstream.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *quant_h263.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *mpeg4_idct_mips.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *mpeg4_decoder.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *interpolate8x8.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *mbcoding.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *mbprediction.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *mpeg4_bitstream.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *quant_h263.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .sys_sram_overlay_2
         {
             *mmc_audio_overlay.o(.text .text.* .sramtext)
             *mmc_audio_overlay.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *mmc_audio_overlay.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .sys_sram_overlay_3
         {
             *zyvc_idct.o(.text .text.* .sramtext)
             *yuv420_to_rgb16.o(.text .text.* .sramtext)
             *zyvc_idct.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *yuv420_to_rgb16.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *zyvc_idct.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *yuv420_to_rgb16.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .sys_sram_overlay_4
         {
             *rv_overlay_set.o(.text .text.* .sramtext)
             *deblock.o(.text .text.* .sramtext)
             *rv_bs.o(.text .text.* .sramtext)
             *rv_overlay_set.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *deblock.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *rv_bs.o(.rdata .rodata .rodata.* .gnu.linkonce.t.* .stub .data .data.* .gnu.linkonce.d.* .data1 .sdata .sdata.* .gnu.linkonce.s.* .sdata2 .sdata2.* .gnu.linkonce.s2.*)
             *rv_overlay_set.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *deblock.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             *rv_bs.o(.bss .bss.* .srambss COMMON .scommon .sbss .sbss.* .sbss2 .sbss2.*)
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .sys_sram_overlay_5
         {
             *vpp_speech_asm.o(.sram_overlay)
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         #define FLASH_SIZE_OF_SYS_SRAM_OVERLAY 0 + SIZEOF(.sys_sram_overlay_0) + SIZEOF(.sys_sram_overlay_1) + SIZEOF(.sys_sram_overlay_2) + SIZEOF(.sys_sram_overlay_3) + SIZEOF(.sys_sram_overlay_4) + SIZEOF(.sys_sram_overlay_5)
         #define MAX_SIZE_OF_SYS_SRAM_OVERLAY MAX(SIZEOF(.sys_sram_overlay_5), MAX(SIZEOF(.sys_sram_overlay_4), MAX(SIZEOF(.sys_sram_overlay_3), MAX(SIZEOF(.sys_sram_overlay_2), MAX(SIZEOF(.sys_sram_overlay_1), MAX(SIZEOF(.sys_sram_overlay_0), 18432))))))

