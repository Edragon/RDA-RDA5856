# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S"
# 23 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S"
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/regdef.h" 1
# 24 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S" 2
# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/cp0.h" 1
# 25 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S" 2

# 1 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/regs/include/global_macros.h" 1
# 27 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S" 2
# 36 "D:/rda5981/code/IoT-RDA5836_3.3/platform/chip/5856/btcpu/src/btcpu_flash.S"
.extern _res_data_flash_start_location
.extern _bt_flash_end


.text
.set at



.globl _flash_btcpu_start
_flash_btcpu_start:
    flash_valid_flag: .word 0xffffffff
    flash_testmode_flag: .word 0xffffffff
    flash_cdacdelta_value: .word 0xffffffff
    flash_resource_addr: .word _res_data_flash_start_location
    j _bt_flash_end
    nop

    flash_xcpu_start_addr: .word _bt_flash_end
