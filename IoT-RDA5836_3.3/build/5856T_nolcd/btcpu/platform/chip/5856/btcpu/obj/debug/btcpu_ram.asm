# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_ram.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_ram.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 2 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_ram.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 3 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_ram.S" 2




.data

.globl btcpu_error_code
.globl btcpu_error_status
.globl btcpu_sp_context


btcpu_error_code: .word 0xc0020000
btcpu_error_status: .word 0xc002c002
btcpu_sp_context: .word 0x0
