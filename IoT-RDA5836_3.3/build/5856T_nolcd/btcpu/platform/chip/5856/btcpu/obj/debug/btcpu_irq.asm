# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 28 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S" 2

# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/global_macros.h" 1
# 30 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/globals_asm.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 2
# 31 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/page_spy_asm.h" 1
# 32 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S" 2
# 183 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
.extern bt_irq
.extern _bt_gdb
# 193 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    .text

    .set reorder
# 206 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    .balign 0x10

    .globl _bt_Irq_handler
    .ent _bt_Irq_handler
    .frame $29, 0, $31
_bt_Irq_handler:
# 222 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    subu $29, $29, (56)


    .set noat
    sw $at,52($29)
    .set at
    sw $8,28($29)
# 241 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    sw $2,8($29) # save $2
    sw $3,12($29) # save $3
    sw $5,16($29) # save $5
    sw $6,20($29) # save $6
    sw $7,24($29) # save $7
    sw $9,32($29) # save $9
    sw $10,36($29) # save $10







    sw $24,40($29) # save $24
    sw $25,44($29) # save $25
    sw $31,48($29) # save $31


    mflo $8
    mfhi $9
    sw $8, 0($29)
    sw $9, 4($29)






    subu $29, $29, (20)
    sw $16,0($29) # save $16
    sw $17,4($29) # save $17
# 282 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    sw $4,8($29) # save $4

    mfc0 $8,$14
    li $9, 0xffff8000 | 1
    sw $8,12($29) # save Pc







    sw $9,16($29) # save CTX Type (>=0 is from function - <0 is From IT)
# 304 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    move $16,$29 # $29 where the context has been saved stored in $16
# 315 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
$L_comming_from_irq:




    mfc0 $9,$13

    addi $27,$27,1




    andi $8,$9,(((0x400) << (4))|((0x400) << (5))|0x003C)





    mfc0 $4,$14
 move $5, $9
 andi $6, $9, 0x003C
 srl $6, $6, 2
 move $7, $27


    bnez $8,_go_to_gdb







    subu $29, $29, 24
    sw $28, 20($29)

    move $28, $16
# 361 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    li $8, (0xff00 | 0x0004 | 0x00400000)







    mfc0 $4,$14
    mfc0 $5,$13
.set noreorder




    la $9, SYSirq_Handle
    jalr $9
    mtc0 $8, $12
.set reorder




    li $8, (0xff00 | 0x0004 | 0x00400000)
    mtc0 $8, $12
# 394 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    move $2, $28

    lw $28, 20($29)






_srx_irq_handler_end_dont_step_in_here:
# 413 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    lw $16,0($2) # restore $16
    lw $17,4($2) # restore $17
# 424 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    lw $4,8($2) # restore task param
    lw $26,12($2) # restore PC
    lw $9,16($2) # restore CTX type
# 447 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    addi $27,$27,-1

    addi $29, $2, (20)



    bgez $9, $L_CTx_restored





    lw $8, 0($29)
    lw $9, 4($29)
    mtlo $8
    mthi $9

    lw $2,8($29)
    lw $3,12($29)
    lw $5,16($29)
    lw $6,20($29)
    lw $7,24($29)
    lw $10,36($29)







    lw $31,48($29)
    lw $24,40($29)
    lw $25,44($29)
    .set noat
    lw $at,52($29)
    .set at






    lw $8,28($29)
    lw $9,32($29)


    addu $29, $29, (56)



$L_CTx_restored:
    .set noreorder
    jr $26
    rfe
    .set reorder

_go_to_gdb:
  jal _bt_gdb


    .type _bt_Irq_handler,@function
    .size _bt_Irq_handler,.-_bt_Irq_handler
    .end _bt_Irq_handler
# 533 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    .globl bt_IrqInit
    .ent bt_IrqInit
bt_IrqInit:
    .frame $29, 0, $31






    li $9, ((0xFFFFFFFF<<0) &~ (1<<15))



    li $8, (((0xa0000000 | 0x01A01000) & 0xffff8000) + (0x01A01000 & 0x8000))
    lw $0, (((0x01A01000) & 0x7fff) - (0x01A01000 & 0x8000)) + 0x00000014($8)
    sw $9, (((0x01A01000) & 0x7fff) - (0x01A01000 & 0x8000)) + 0x00000008($8)


    mtc0 $0, $13
    li $24, (0xff00 | 0x0001 | 0)
    mtc0 $24, $12

    li $10,1


    li $27, 1
# 568 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_irq.S"
    jr $31

    .type bt_IrqInit,@function
    .size bt_IrqInit,.-bt_IrqInit
    .end bt_IrqInit
