#!/bin/sh

# PROJ_ROOT=/home/lee/project
PROJ_ROOT=$(pwd)

# export PROJ=$1
# export PROJ_WORKDIR=$PROJ_ROOT/$PROJ
export PROJ_WORKDIR=$PROJ_ROOT

# Soft directory location
export SOFT_WORKDIR=$PROJ_WORKDIR

PATH=$SOFT_WORKDIR/env/utils:$PATH

# SVN server database location
export SVN_SOFT_REPOS=http://subversion-server/svn/developing

# Soft environment setup
#if [ -f ${SOFT_WORKDIR}/env/set_env.sh ]; then
#    echo "source setenv.sh"
#	source ${SOFT_WORKDIR}/env/set_env.sh
#fi

export PERL5LIB=${SOFT_WORKDIR}/env/utils/lib/
export PERL_RL="Perl"
export PERL_READLINE_NOWARN=1

export PATH="${SOFT_WORKDIR}/env:$PATH"
export PATH="${SOFT_WORKDIR}/env/utils:$PATH"
export PATH="${SOFT_WORKDIR}/env/compilation:$PATH"
export SOFT_WORKDIR_CLEAN_ENV="${SOFT_WORKDIR}/env/clean_env"

alias cds='cd ${SOFT_WORKDIR}'
alias cde='cd ${SOFT_WORKDIR}/env'
alias cdenv='cd ${SOFT_WORKDIR}/env'
alias cdpal='cd ${SOFT_WORKDIR}/platform/pal'
alias cdp='cd ${SOFT_WORKDIR}/platform/pal'
alias cdphy='cd ${SOFT_WORKDIR}/platform/phy'
alias cdhal='cd ${SOFT_WORKDIR}/platform/chip/hal'
alias cdh='cd ${SOFT_WORKDIR}/platform/chip/hal'
alias cdedrv='cd ${SOFT_WORKDIR}/platform/edrv'
alias cdboot='cd ${SOFT_WORKDIR}/platform/chip/boot'
alias cdrom='cd ${SOFT_WORKDIR}/platform_test/hal/int_rom_programmer'
alias cdspp='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spp'
alias cdspc='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spc'
alias cdspal='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spal'
alias cdrfd='cd ${SOFT_WORKDIR}/platform/edrv/rfd'
alias cdsx='cd ${SOFT_WORKDIR}/platform/base/sx'
alias cdstack='cd ${SOFT_WORKDIR}/platform/stack'
alias cdtest='cd ${SOFT_WORKDIR}/platform_test'
alias cdtst='cd ${SOFT_WORKDIR}/platform_test'
alias cdt='cd ${SOFT_WORKDIR}/platform_test'
alias cdproto='cd ${SOFT_WORKDIR}/platform_test/stack/proto'
alias cdpro='cd ${SOFT_WORKDIR}/platform_test/stack/proto'
alias cdcal='cd ${SOFT_WORKDIR}/platform_test/calib/calib_stub'
alias cdhex='cd ${SOFT_WORKDIR}/hex'
alias cdchip='cd ${SOFT_WORKDIR}/platform/chip'
alias cdbcpu='cd ${SOFT_WORKDIR}/platform/chip/bcpu'

# cdregs
function cdregsFunc {
    asic_name=`make -C ${SOFT_WORKDIR}/platform get_var_CT_ASIC | grep -v "make:"`
    if [ ! -d "${SOFT_WORKDIR}/platform/chip/regs/${asic_name}/include" ]; then
        cd ${SOFT_WORKDIR}/platform/chip/regs;
    else
        cd ${SOFT_WORKDIR}/platform/chip/regs/${asic_name}/include;
    fi
}

function croot()
{
    if [ "${SOFT_WORKDIR}" ]; then
        \cd ${SOFT_WORKDIR};
    else
        echo "Could not locate the root of the tree.";
    fi
}

alias cdregs='cdregsFunc'

alias cddefs='cd ${SOFT_WORKDIR}/platform/chip/defs'
alias cdrom='cd ${SOFT_WORKDIR}/platform/chip/rom'
alias cdfmd='cd ${SOFT_WORKDIR}/platform/edrv/fmd'
alias cdmcd='cd ${SOFT_WORKDIR}/platform/edrv/mcd'
alias cdumss='cd ${SOFT_WORKDIR}/platform/svc/umss'
alias cdsvc='cd ${SOFT_WORKDIR}/platform/svc'
alias cductls='cd ${SOFT_WORKDIR}/platform/svc/uctls'
alias cdmps='cd ${SOFT_WORKDIR}/platform/svc/mps'
alias cdmrs='cd ${SOFT_WORKDIR}/platform/svc/mrs'
alias cdimgs='cd ${SOFT_WORKDIR}/platform/svc/imgs'
alias cdcams='cd ${SOFT_WORKDIR}/platform/svc/cams'
alias cdavps='cd ${SOFT_WORKDIR}/platform/svc/avps'
alias cdavrs='cd ${SOFT_WORKDIR}/platform/svc/avrs'
alias cdcamd='cd ${SOFT_WORKDIR}/platform/edrv/camd'
alias cdtarget='cd ${SOFT_WORKDIR}/target'

alias cdmmi='cd ${SOFT_WORKDIR}/application/coolmmi/main'
alias cdm='cd ${SOFT_WORKDIR}/application/coolmmi/main'

echo "Soft base SOFT_WORKDIR=$SOFT_WORKDIR"
#cd $SOFT_WORKDIR

# echo Project Switched to $PROJ

