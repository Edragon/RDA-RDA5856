# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb_stub_mips.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb_stub_mips.S"
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb_stub_mips.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb_stub_mips.S" 2
.text

.globl hal_GdbFlushCache
.ent hal_GdbFlushCache
.frame $29,0,$31
hal_GdbFlushCache:
    .set noreorder
    .align 4
    cache 0,0
    nop
    nop
    nop
    jr $31
    nop
    .set reorder
.end hal_GdbFlushCache

.globl hal_GdbFlushCacheData
.ent hal_GdbFlushCacheData
.frame $29,0,$31
hal_GdbFlushCacheData:
    .set noreorder
    .align 4
    cache 2,0
    nop
    nop
    nop
    jr $31
    nop
    .set reorder
.end hal_GdbFlushCacheData
