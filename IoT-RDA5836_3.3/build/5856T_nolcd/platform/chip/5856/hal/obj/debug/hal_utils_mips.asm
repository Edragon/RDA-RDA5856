# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_utils_mips.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_utils_mips.S"
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_utils_mips.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_utils_mips.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_utils_mips.S" 2






    .set nomips16
    .text
    .set reorder
# 75 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_utils_mips.S"
    .globl hal_SysGetSp
    .globl hal_SysGetSp
    .ent hal_SysGetSp
hal_SysGetSp:
hal_SysGetSp:
    .frame $29, 0, $31

    move $2,$29

    j $31

    .type hal_SysGetSp,@function
    .size hal_SysGetSp,.-hal_SysGetSp

    .end hal_SysGetSp
