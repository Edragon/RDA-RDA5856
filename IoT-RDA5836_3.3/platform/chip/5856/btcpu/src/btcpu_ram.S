#include "regdef.h"
#include "cp0.h"

//#################################################################################
// Global variables in RAM
//#################################################################################
.data
// Btcpu variables
.globl btcpu_error_code
.globl btcpu_error_status
.globl btcpu_sp_context


btcpu_error_code:     .word 0xc0020000
btcpu_error_status:   .word 0xc002c002
btcpu_sp_context:     .word 0x0

