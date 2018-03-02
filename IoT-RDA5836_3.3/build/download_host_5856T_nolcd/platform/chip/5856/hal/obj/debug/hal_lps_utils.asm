# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S"
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 28 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/global_macros.h" 1
# 29 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/globals_asm.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 2
# 30 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2


# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/halp_gdb_stub.h" 1
# 33 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/halp_irq_handler.h" 1
# 34 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/comn/include/chip_id.h" 1
# 35 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S" 2
# 43 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S"
.extern sxr_IrqStack_pointer
.extern hal_LpsDeepSleep
# 79 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_lps_utils.S"
    .globl hal_LpsDeepSleepWrapper
    .ent hal_LpsDeepSleepWrapper
hal_LpsDeepSleepWrapper:
    .frame $29, 32, $31



    mfc0 $9, $12
    li $8, ~1
    and $8, $9
    mtc0 $8, $12
    .set noat
    li $at, 0x00001000
    or $27, $27, $at
    .set at

    lw $8, sxr_IrqStack_pointer
    add $8, $8, -32
    sw $9, 24($8)
    sw $29, 20($8)
    sw $31, 16($8)
    move $29, $8

    jal hal_LpsDeepSleep

    .set noat
    li $at, ~0x00001000
    and $27, $27, $at
    .set at
    lw $9, 24($29)
    lw $31, 16($29)
    lw $29, 20($29)
    mtc0 $9, $12
    jr $31

    .type hal_LpsDeepSleepWrapper,@function
    .size hal_LpsDeepSleepWrapper,.-hal_LpsDeepSleepWrapper
    .end hal_LpsDeepSleepWrapper
