# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S"

# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 3 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 4 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S" 2
# 12 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S"
    .text
    .set reorder
# 45 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S"
    .globl setjmp
    .ent setjmp
setjmp:
    .frame $29, 0, $31


    sw $16,0($4) # save $16 ($16 -$23)
    sw $17,4($4) # save $17
    sw $18,8($4) # save $18
    sw $19,12($4) # save $19
    sw $20,16($4) # save $20
    sw $21,20($4) # save $21
    sw $22,24($4) # save $22
    sw $23,28($4) # save $23
    sw $30,32($4) # save $30 ($30)


    sw $29,36($4) # save $29 ($29)
    sw $31,40($4) # save $31 ($31)

    move $2,$0

    j $31
    .type setjmp,@function
    .size setjmp,.-setjmp

    .end setjmp
# 105 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/hal/../../comn/hal/src/setjmp.S"
    .globl longjmp
    .ent longjmp
longjmp:
    .frame $29, 0, $31


    lw $16,0($4) # Recover $16 ($16 -$23)
    lw $17,4($4) # Recover $17
    lw $18,8($4) # Recover $18
    lw $19,12($4) # Recover $19
    lw $20,16($4) # Recover $20
    lw $21,20($4) # Recover $21
    lw $22,24($4) # Recover $22
    lw $23,28($4) # Recover $23
    lw $30,32($4) # Recover $30 ($30)

    lw $29,36($4) # Recover $29 ($29)
    lw $31,40($4) # Recover $31 ($31)

    bne $5,$0,1f
    li $5,1
1:
    move $2,$5
    move $4,$5

    j $31

    .type longjmp,@function
    .size longjmp,.-longjmp
    .end longjmp
