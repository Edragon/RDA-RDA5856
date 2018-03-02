# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
# 24 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb_stub.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S" 2






# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/global_macros.h" 1
# 34 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/globals_asm.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 2
# 35 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/page_spy_asm.h" 1
# 36 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S" 2
# 194 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
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

    .globl btcpu_GdbGBreak
btcpu_GdbGBreak:
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreakSoftBreakPoint
    .word btcpu_GdbBreakRaise
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreakBreakPoint
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreakDivBy0
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreaks
    .word btcpu_GdbBreakPrint
# 244 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
  .text
  .set nomips16
    .globl _bt_gdb
    .ent _bt_gdb
_bt_gdb:
    .frame $29, 0, $31
# 258 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
    subu $29, $29, ((38 +3)*4)


    mfc0 $24, $14
    sw $9, 0x90($29)
    sw $24, 0x94($29)



    lw $8, (20)+52($16)
    sw $0, 0x00($29)
    .set noat
    sw $8, 0x04($29)
    .set at
    sw $2, 0x08($29)
    sw $3, 0x0c($29)
    sw $4, 0x10($29)
    sw $5, 0x14($29)
    sw $6, 0x18($29)
    sw $7, 0x1c($29)

    lw $8, (20)+28($16)
    sw $8, 0x20($29)
    lw $8, (20)+32($16)
    sw $8, 0x24($29)

    sw $10, 0x28($29)
    sw $11, 0x2c($29)
    sw $12, 0x30($29)
    sw $13, 0x34($29)
    sw $14, 0x38($29)
    sw $15, 0x3c($29)

    lw $8, 0($16)
    sw $17, 0x44($29)
    sw $8, 0x40($29)
    sw $18, 0x48($29)
    sw $19, 0x4c($29)
    sw $20, 0x50($29)
    sw $21, 0x54($29)
    sw $22, 0x58($29)
    sw $23, 0x5c($29)

    lw $8, (20)+40($16)
    sw $8, 0x60($29)

    sw $25, 0x64($29)

    sw $0, 0x68($29)
    addi $8,$27,-1
    sw $8, 0x6c($29)
    sw $28, 0x70($29)

    addu $16, $16, (20)+(56)
    sw $16, 0x74($29)
    sw $30, 0x78($29)
    sw $31, 0x7c($29)

    .set noat
    mflo $8
    mfhi $at
    sw $8, 0x84($29)
    sw $at, 0x88($29)
    .set at

    .set noat
    mfc0 $8, $12
    mfc0 $at, $8
    sw $8, 0x80($29)
    sw $at, 0x8c($29)
    .set at


    la $8, btcpu_sp_context
    sw $29, 0($8)


    subu $29, $29, 16







    li $8, (0xff00 | 0x0004 | 0x00400000)
    mtc0 $8, $12


    li $4, 0

    andi $8, $9, 0x003C
    bne $8, ((9)<<2), $L_gdb_skip_break


    andi $8, $24, 1
    beqz $8, $L_isa32_bp
$L_isa16_bp:
    lhu $8, -1($24)
    srl $8, 3
    j $L_isa_common
$L_isa32_bp:

    lw $8, 0($24)
    srl $8, 14
$L_isa_common:
    la $24, btcpu_GdbGBreak
    andi $8, $8, 0x3c
    addu $8, $8, $24
    lw $8, 0($8)
    jalr $8
    j $L_gdb_skip_irq
$L_gdb_skip_break:
    beqz $8, $L_gdb_skip_exception
    move $5, $9


    jal btcpu_GdbException
    j $L_gdb_skip_irq
$L_gdb_skip_exception:
# 389 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_gdb.S"
    andi $8, $9, ((0x400) << (5))
    beqz $8, $L_skip_host

    j $L_gdb_skip_irq
$L_skip_host:



    jal btcpu_GdbIrqDebug
$L_gdb_skip_irq:


    addu $29, $29, 16






    la $8, btcpu_sp_context
    sw $0, 0($8)



    move $26, $29

    move $29, $16

    lw $8, 0x84($26)
    lw $9, 0x88($26)
    mtlo $8
    mthi $9

    lw $8, 0x80($26)
    mtc0 $8, $12

    lw $31, 0x7c($26)
    lw $30, 0x78($26)







    lw $25, 0x64($26)
    lw $24, 0x60($26)
    lw $23, 0x5c($26)
    lw $22, 0x58($26)
    lw $21, 0x54($26)
    lw $20, 0x50($26)
    lw $19, 0x4c($26)
    lw $18, 0x48($26)
    lw $17, 0x44($26)
    lw $16, 0x40($26)
    lw $15, 0x3c($26)
    lw $14, 0x38($26)
    lw $13, 0x34($26)
    lw $12, 0x30($26)
    lw $11, 0x2c($26)
    lw $10, 0x28($26)
    lw $9, 0x24($26)
    lw $8, 0x20($26)
    lw $7, 0x1c($26)
    lw $6, 0x18($26)
    lw $5, 0x14($26)
    lw $4, 0x10($26)
    lw $3, 0x0c($26)
    lw $2, 0x08($26)
    .set noat
    lw $1, 0x04($26)
    .set at





    lw $26, 0x94($26)


    addi $27,$27,-1

    .set noreorder
    j $26
    rfe
    .set reorder




    .type _bt_gdb,@function
    .size _bt_gdb,.-_bt_gdb
    .end _bt_gdb
