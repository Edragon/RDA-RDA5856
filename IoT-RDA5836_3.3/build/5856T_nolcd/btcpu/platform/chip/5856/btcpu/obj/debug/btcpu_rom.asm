# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S"
# 23 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 24 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S" 2

# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/global_macros.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S" 2
# 36 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_rom.S"
.extern btcpu_stack_base
.extern btcpu_main_entry
.extern btcpu_int_entry

.text
.set at



.globl __btcpu_start
__btcpu_start:
    nop




    cache 0,0
    nop

 li $2,0
    li $3,0
    li $6,0
    li $10,0
    li $12,0
    li $13,0
    li $14,0
    li $15,0
    li $16,0
    li $17,0
    li $18,0
    li $19,0
    li $20,0
    li $21,0
    li $22,0
    li $23,0
    li $24,0
    li $25,0

    li $27,0
    li $28,0
    li $29,0
    li $30,0
    li $31,0




    lw $29, __btcpu_stack_base


    la $2, __btcpu_init
    jr $2






.globl __btcpu_init
__btcpu_init:



    li $8, 0x00400000 | 0xff00 | 0x0001
    mtc0 $0, $13
    nop
    mtc0 $8, $12

    lw $31, __btcpu_int_handler
    lw $2, __btcpu_main_entry
    j $2


__btcpu_stack_base: .word _bt_sram_non_romed_top
__btcpu_main_entry: .word _btflash_start
__btcpu_int_entry: .word _btrom_irq_start



    .globl btcpu_GetSp
    .globl btcpu_GetSp
    .ent btcpu_GetSp
btcpu_GetSp:
btcpu_GetSp:
    .frame $29, 0, $31

    move $2,$29

    j $31

    .type btcpu_GetSp,@function
    .size btcpu_GetSp,.-btcpu_GetSp

    .end btcpu_GetSp




. = (__btcpu_start | 0x180)




.globl __btcpu_int_handler
__btcpu_int_handler:



    lw $26, __btcpu_int_entry
    j $26
