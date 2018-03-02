# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S"
# 24 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/halp_gdb_stub.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S" 2
# 37 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_gdb.S"
    .set nomips16
    .globl hal_DbgGdbPrintf
    .globl dbg_GdbPrint
    .ent hal_DbgGdbPrintf
    .frame $29, 0, $31
hal_DbgGdbPrintf:
dbg_GdbPrint:
    break 15
    jr $31
    nop
    .type hal_DbgGdbPrintf,@function
    .size hal_DbgGdbPrintf,.-hal_DbgGdbPrintf
    .end hal_DbgGdbPrintf






    .data

    .globl hal_GdbGBreak
hal_GdbGBreak:
    .word hal_GdbBreaks
    .word hal_GdbBreakSoftBreakPoint
    .word hal_GdbBreakRaise
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreakBreakPoint
    .word hal_GdbBreaks
    .word hal_GdbBreakDivBy0
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreaks
    .word hal_GdbBreakPrint
