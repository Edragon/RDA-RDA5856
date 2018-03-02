#!/bin/sh
# must be sourced with bash

# the SOFT_WORKDIR_CLEAN_ENV variable is set to a script responsible to cleanup some stuff before setting a new env (mainly the path)
if [[ -n "$SOFT_WORKDIR_CLEAN_ENV" ]]; then
    test -f $SOFT_WORKDIR_CLEAN_ENV.sh && source $SOFT_WORKDIR_CLEAN_ENV.sh
fi

## Set perl environment variables
export PERL5LIB=${SOFT_WORKDIR}/env/utils/lib/
export PERL_RL="Perl"
export PERL_READLINE_NOWARN=1

if [[ $(uname | grep -c CYGWIN) > 0 ]]; then  
    export PATH="${SOFT_WORKDIR}/env/win32:${PATH}"
fi

export PATH="${SOFT_WORKDIR}/env:$PATH"
export PATH="${SOFT_WORKDIR}/env/utils:$PATH"
export PATH="${SOFT_WORKDIR}/env/compilation:$PATH"
export PATH="${SOFT_WORKDIR}/env/deliv:$PATH"
export SOFT_WORKDIR_CLEAN_ENV="${SOFT_WORKDIR}/env/clean_env"

# use the OBJECT_DIR stuff on helios or vulcain
# also create the directory if needed
#if [ "$HOSTNAME" == "helios" ] || [ "$HOSTNAME" == "vulcain" ] || [ "$HOSTNAME" == "petrus" ] || [ "$HOSTNAME" == "jupiter" ] || [ "$HOSTNAME" == "pluton" ] || [ "$HOSTNAME" == "sancerre" ] || [ "$HOSTNAME" == "medoc" ] || [ "$HOSTNAME" == "bergerac" ] || [ "$HOSTNAME" == "cadillac" ]; then
#    export OBJECT_DIR=/tmp/${USER}_obj/${PROJ}/`echo ${SOFT_WORKDIR/$PROJ_WORKDIR\//} | tr "/" "_"`
#    if [ ! -d $OBJECT_DIR ]; then
#        mkdir -p $OBJECT_DIR
#    fi
#fi

alias cdapp='cd ${SOFT_WORKDIR}/application'
alias cdadp='cd ${SOFT_WORKDIR}/application/adaptation'
alias cdcomm='cd ${SOFT_WORKDIR}/application/adaptation/communication'
alias cdmmi='cd ${SOFT_WORKDIR}/application/coolmmi'
alias cdm='cd ${SOFT_WORKDIR}/application/coolmmi'
alias cdres='cd ${SOFT_WORKDIR}/application/target_res'
alias cdbuild='cd ${SOFT_WORKDIR}/build'
alias cdbase='cd ${SOFT_WORKDIR}/platform/base'
alias cdsap='cd ${SOFT_WORKDIR}/platform/base/sap'
alias cdsx='cd ${SOFT_WORKDIR}/platform/base/sx'
alias cdchip='cd ${SOFT_WORKDIR}/platform/chip'
alias cdbcpu='cd ${SOFT_WORKDIR}/platform/chip/bcpu'
alias cdspal='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spal'
alias cdspc='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spc'
alias cdspp='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spp'
alias cdboot='cd ${SOFT_WORKDIR}/platform/chip/boot'
alias cddefs='cd ${SOFT_WORKDIR}/platform/chip/defs'
alias cdhal='cd ${SOFT_WORKDIR}/platform/chip/hal'
alias cdh='cd ${SOFT_WORKDIR}/platform/chip/hal'
alias cdpal='cd ${SOFT_WORKDIR}/platform/chip/pal'
alias cdp='cd ${SOFT_WORKDIR}/platform/chip/pal'
alias cdrom='cd ${SOFT_WORKDIR}/platform/chip/rom'
alias cdedrv='cd ${SOFT_WORKDIR}/platform/edrv'
alias cdcamd='cd ${SOFT_WORKDIR}/platform/edrv/camdold'
alias cdfmd='cd ${SOFT_WORKDIR}/platform/edrv/fmd'
alias cdmcd='cd ${SOFT_WORKDIR}/platform/edrv/mcd'
alias cdpmd='cd ${SOFT_WORKDIR}/platform/edrv/pmd'
alias cdrfd='cd ${SOFT_WORKDIR}/platform/edrv/rfd'
alias cdenv='cd ${SOFT_WORKDIR}/env'
alias cde='cd ${SOFT_WORKDIR}/env'
alias cdhex='cd ${SOFT_WORKDIR}/hex'
alias cdmdi='cd ${SOFT_WORKDIR}/platform/mdi'
alias cdalg='cd ${SOFT_WORKDIR}/platform/mdi/alg'
alias cdmmc='cd ${SOFT_WORKDIR}/platform/mdi/mmc'
alias cdstack='cd ${SOFT_WORKDIR}/platform/stack'
alias cdcc='cd ${SOFT_WORKDIR}/platform/stack/cc'
alias cdl1='cd ${SOFT_WORKDIR}/platform/stack/l1'
alias cdmm='cd ${SOFT_WORKDIR}/platform/stack/mm'
alias cdrr='cd ${SOFT_WORKDIR}/platform/stack/rr'
alias cdsvc='cd ${SOFT_WORKDIR}/platform/svc'
alias cdavps='cd ${SOFT_WORKDIR}/platform/svc/avps'
alias cdavrs='cd ${SOFT_WORKDIR}/platform/svc/avrs'
alias cdcams='cd ${SOFT_WORKDIR}/platform/svc/cams'
alias cdimgs='cd ${SOFT_WORKDIR}/platform/svc/imgs'
alias cdmps='cd ${SOFT_WORKDIR}/platform/svc/mps'
alias cdmrs='cd ${SOFT_WORKDIR}/platform/svc/mrs'
alias cductls='cd ${SOFT_WORKDIR}/platform/svc/uctls'
alias cdumss='cd ${SOFT_WORKDIR}/platform/svc/umss'
alias cdtarget='cd ${SOFT_WORKDIR}/target'
alias cdtest='cd ${SOFT_WORKDIR}/platform_test'
alias cdtst='cd ${SOFT_WORKDIR}/platform_test'
alias cdt='cd ${SOFT_WORKDIR}/platform_test'
alias cdtcal='cd ${SOFT_WORKDIR}/platform_test/calib/calib_stub'
alias cdtrom='cd ${SOFT_WORKDIR}/platform_test/rom/int_rom_programmer'
alias cdproto='cd ${SOFT_WORKDIR}/platform_test/stack/proto'
alias cdpro='cd ${SOFT_WORKDIR}/platform_test/stack/proto'
alias cdfp='cd ${SOFT_WORKDIR}/toolpool/flash_programmer'
alias cdtfp='cd ${SOFT_WORKDIR}/toolpool/t_flash_programmer'

# cdregs
function cdregsFunc {
    asic_name=`make -C ${SOFT_WORKDIR}/platform get_var_CT_ASIC | grep -v "make:"`
    if [ ! -d "${SOFT_WORKDIR}/platform/chip/regs/${asic_name}/include" ]; then
        cd ${SOFT_WORKDIR}/platform/chip/regs;
    else
        cd ${SOFT_WORKDIR}/platform/chip/regs/${asic_name}/include;
    fi
}

alias cdregs='cdregsFunc'

# cds
function cds {
    if [ "x$1" = "x" ]; then
        cd ${SOFT_WORKDIR}
        return
    fi
    DIR_TO_BE_CHECKED=". env platform platform/base platform/chip platform/csw \
        platform/edrv platform/mdi platform/stack platform/svc platform/vpp \
        platform_test/hal target toolpool application application/adaptation \
        application/coolmmi application/coolmmi/media application/coolmmi/mmi \
        application/coolmmi/mmi_csdapp application/thirdpartylibs \
        platform/chip/bcpu platform/csw/base platform/csw/cfw \
        platform/edrv/camdold platform/edrv/lcdd platform/edrv/memd \
        platform/edrv/pmd platform/edrv/rfd \
        platform/mdi/alg \
        "
    for dir in ${DIR_TO_BE_CHECKED}; do
        dst="${SOFT_WORKDIR}/${dir}/$1"
        if [ -d "${dst}" ]; then
            cd "${dst}"
            return
        fi
    done
    echo "Unknown directory: $1"
}

# usefull git function
#alias gitk='gitk --all'

echo "Soft base SOFT_WORKDIR=$SOFT_WORKDIR"

