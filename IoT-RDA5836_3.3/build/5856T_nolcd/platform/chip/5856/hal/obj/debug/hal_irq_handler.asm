# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 28 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2

# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/global_macros.h" 1
# 30 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/globals_asm.h" 1
# 26 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/sys_irq_asm.h" 2
# 31 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/page_spy_asm.h" 1
# 32 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2


# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/halp_gdb_stub.h" 1
# 35 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/halp_irq_handler.h" 1
# 36 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S" 2
# 52 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
.extern initial_stack_top_var
# 67 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    .text

    .set reorder
# 79 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    .section .irqtext, "awx", @progbits
    .balign 0x10

    .globl _sxr_Irq_handler
    .ent _sxr_Irq_handler
    .frame $29, 0, $31
_sxr_Irq_handler:
# 108 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    subu $29, $29, (56)


    .set noat
    sw $at,52($29)
    .set at
    sw $8,28($29)
# 163 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
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
# 204 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    sw $4,8($29) # save $4


    mfc0 $8,$14

    li $9, 0xffff8000 | 1

    sw $8,12($29) # save Pc


    blt $27, 0x01000000, $L_skip_save_job_flag
    or $9, $9, (0x10)
$L_skip_save_job_flag:

    sw $9,16($29) # save CTX Type (>=0 is from function - <0 is From IT)
# 228 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    move $16,$29 # $29 where the context has been saved stored in $16

    beqz $27,$L_comming_from_task
        blt $27, 0x01000000, $L_comming_from_irq
            .set noat
            li $at, ~0x01000000
            and $27, $27, $at
            .set at
$L_comming_from_task:

        lw $8, sxr_IrqStack_pointer
        nop
        move $29, $8

$L_comming_from_irq:




    mfc0 $9,$13
# 260 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    addi $27,$27,1







    andi $8,$9,(((0x400) << (4))|((0x400) << (5))|((0x400) << (3))| 0x003C)


    bnez $8,_go_to_gdb






    subu $29, $29, 24
    sw $28, 20($29)

    move $28, $16
# 295 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    li $8, ( ((((0xa0000000 | 0x01A01000) & 0xffff8000) + (0x01A01000 & 0x8000))) | 0xa0000000 )
    lw $0, (((0x01A01000) & 0x7fff) - (0x01A01000 & 0x8000)) + 0x00000014($8)





    li $8, (0xff00 | 0x0001 | 0)

.set noreorder
    jal hal_IrqDispatch
    mtc0 $8, $12
.set reorder




    li $8, (0xff00 | 0x0004 | 0)
    mtc0 $8, $12



    move $2, $28

    lw $28, 20($29)
# 335 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
_srx_irq_handler_end_dont_step_in_here:
# 345 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    lw $16,0($2) # restore $16
    lw $17,4($2) # restore $17
# 356 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    lw $4,8($2) # restore task param
    lw $26,12($2) # restore PC
    lw $9,16($2) # restore CTX type

    and $8,$9,(0x10)
    beqz $8,$L_skip_restore_job_flag
    or $27,$27,0x01000000
$L_skip_restore_job_flag:



    li $8, ( ((((0xa0000000 | 0x01A01000) & 0xffff8000) + (0x01A01000 & 0x8000))) | 0xa0000000 )

    sw $9, (((0x01A01000) & 0x7fff) - (0x01A01000 & 0x8000)) + 0x00000014($8)
# 379 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
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

$L_CTx_restored :
    .set noreorder
    jr $26
    rfe
    .set reorder
# 471 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
_go_to_gdb:







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


    la $8, xcpu_sp_context
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
    la $24, hal_GdbGBreak
    andi $8, $8, 0x3c
    addu $8, $8, $24
    lw $8, 0($8)
    jalr $8
    j $L_gdb_skip_irq
$L_gdb_skip_break:
    beqz $8, $L_gdb_skip_exception
    move $5, $9


    jal hal_GdbException
    j $L_gdb_skip_irq
$L_gdb_skip_exception:


    andi $8, $9, ((0x400) << (3))
    beqz $8, $L_skip_page
    jal hal_GdbIrqPage
    j $L_gdb_skip_irq
$L_skip_page:




    andi $8, $9, ((0x400) << (5))
    beqz $8, $L_skip_host
    jal hal_HstRamClientIrq
    j $L_gdb_skip_irq
$L_skip_host:



    jal hal_GdbIrqDebug
$L_gdb_skip_irq:


    addu $29, $29, 16






    la $8, xcpu_sp_context
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
# 827 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    .type _sxr_Irq_handler,@function
    .size _sxr_Irq_handler,.-_sxr_Irq_handler
    .end _sxr_Irq_handler
# 851 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/hal_irq_handler.S"
    .section .sramdata, "aw", @progbits

.globl sxr_IrqStack_pointer
.globl g_SpBufferPtr
.globl g_SpBufferTop
.globl g_SpBufferStamp

g_SpBufferPtr: .word 0
g_SpBufferTop: .word 0
g_SpBufferStamp: .word 0
