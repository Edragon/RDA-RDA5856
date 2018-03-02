        .fast_cpu_memory_overlay_0
         {
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .fast_cpu_memory_overlay_1
         {
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .fast_cpu_memory_overlay_2
         {
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .fast_cpu_memory_overlay_3
         {
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .fast_cpu_memory_overlay_4
         {
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         .fast_cpu_memory_overlay_5
         {
             #if (USE_BINUTILS_2_19 == 1) 
             /* FIXME: LD 2.19 doesn't like empty overlays... */
             LONG(0)
             #endif 
             . = ALIGN(0x10);
         }

         #define FLASH_SIZE_OF_FAST_CPU_MEMORY_OVERLAY 0 + SIZEOF(.fast_cpu_memory_overlay_0) + SIZEOF(.fast_cpu_memory_overlay_1) + SIZEOF(.fast_cpu_memory_overlay_2) + SIZEOF(.fast_cpu_memory_overlay_3) + SIZEOF(.fast_cpu_memory_overlay_4) + SIZEOF(.fast_cpu_memory_overlay_5)
         #define MAX_SIZE_OF_FAST_CPU_MEMORY_OVERLAY MAX(SIZEOF(.fast_cpu_memory_overlay_5), MAX(SIZEOF(.fast_cpu_memory_overlay_4), MAX(SIZEOF(.fast_cpu_memory_overlay_3), MAX(SIZEOF(.fast_cpu_memory_overlay_2), MAX(SIZEOF(.fast_cpu_memory_overlay_1), MAX(SIZEOF(.fast_cpu_memory_overlay_0), 0))))))

