#!/usr/bin/bash -e

USAGE="
Usage:
`basename $0` <target> [ binlib ]
    <target> - The CT_TARGET value
    binlib   - Force to remake binary libraries
"

error_info()
{
    echo "$USAGE"
    echo "$*"
    exit 1
}

TARGET=$1
BINLIB=0

if [ "x$1" = "x" ]; then
    error_info "Please input target"
fi

if [ ! -d target/${TARGET} ]; then
    error_info "Invalid target: ${TARGET}"
fi

if [ "x$2" = "xbinlib" ]; then
    BINLIB=1
elif [ "x$2" != "x" ]; then
    error_info "Unknown argument: $2"
fi

echo ============ Update ALL libs ============
echo ======== with WAPMMS and DualSim ========

cd ${SOFT_WORKDIR}

if [ "x${BINLIB}" = "x1" ]; then
    rm -f -r ./build/${TARGET}/platform
    cd ${SOFT_WORKDIR}/platform

    echo make config: CT_TARGET=${TARGET}
    ctmake CT_TARGET=${TARGET} CT_RELEASE=debug CT_USER=ADMIN AUTO_XMD2H=no AUTO_XCP2H=no NO_EDRV=1 clean #1>/dev/null
    ctmake CT_TARGET=${TARGET} CT_RELEASE=debug CT_USER=ADMIN AUTO_XMD2H=no AUTO_XCP2H=no NO_EDRV=1 binlib #1>/dev/null
    ctmake CT_TARGET=${TARGET} CT_RELEASE=release CT_USER=ADMIN AUTO_XMD2H=no AUTO_XCP2H=no NO_EDRV=1 clean #1>/dev/null
    ctmake CT_TARGET=${TARGET} CT_RELEASE=release CT_USER=ADMIN AUTO_XMD2H=no AUTO_XCP2H=no NO_EDRV=1 binlib #1>/dev/null
fi

CT_ASIC=`make CT_TARGET=${TARGET} CT_RELEASE=debug CT_USER=ADMIN get_var_CT_ASIC`

# Move lib
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/base/lib/libbase_debug.a ${SOFT_WORKDIR}/platform/base/packed/base_dualsim_gprs/lib/libbase_dualsim_gprs_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/base/lib/libbase_release.a ${SOFT_WORKDIR}/platform/base/packed/base_dualsim_gprs/lib/libbase_dualsim_gprs_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/chip/lib/libchip_debug.a ${SOFT_WORKDIR}/platform/chip/packed/chip_${CT_ASIC}_chip/chip_dualsim_gprs/lib/libchip_dualsim_gprs_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/chip/lib/libchip_release.a ${SOFT_WORKDIR}/platform/chip/packed/chip_${CT_ASIC}_chip/chip_dualsim_gprs/lib/libchip_dualsim_gprs_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/csw/lib/libcsw_debug.a ${SOFT_WORKDIR}/platform/csw/packed/csw_${CT_ASIC}/csw_dualsim_gprs/lib/libcsw_dualsim_gprs_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/csw/lib/libcsw_release.a ${SOFT_WORKDIR}/platform/csw/packed/csw_${CT_ASIC}/csw_dualsim_gprs/lib/libcsw_dualsim_gprs_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/mdi/lib/libmdi_debug.a ${SOFT_WORKDIR}/platform/mdi/packed/mdi_${CT_ASIC}/lib/libmdi_${CT_ASIC}_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/mdi/lib/libmdi_release.a ${SOFT_WORKDIR}/platform/mdi/packed/mdi_${CT_ASIC}/lib/libmdi_${CT_ASIC}_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/stack/lib/libstack_debug.a ${SOFT_WORKDIR}/platform/stack/packed/stack_${CT_ASIC}/stack_dualsim_gprs/lib/libstack_dualsim_gprs_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/stack/lib/libstack_release.a ${SOFT_WORKDIR}/platform/stack/packed/stack_${CT_ASIC}/stack_dualsim_gprs/lib/libstack_dualsim_gprs_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/svc/lib/libsvc_debug.a ${SOFT_WORKDIR}/platform/svc/packed/svc_${CT_ASIC}/lib/libsvc_${CT_ASIC}_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/svc/lib/libsvc_release.a ${SOFT_WORKDIR}/platform/svc/packed/svc_${CT_ASIC}/lib/libsvc_${CT_ASIC}_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/vpp/lib/libvpp_debug.a ${SOFT_WORKDIR}/platform/vpp/packed/vpp_${CT_ASIC}/lib/libvpp_${CT_ASIC}_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/vpp/lib/libvpp_release.a ${SOFT_WORKDIR}/platform/vpp/packed/vpp_${CT_ASIC}/lib/libvpp_${CT_ASIC}_release.a

mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/edrv/btd/rdaCommon/lib/librdaCommon_debug.a ${SOFT_WORKDIR}/platform/edrv/btd/rdaCommon/lib/librdaCommon_debug.a
mv -f ${SOFT_WORKDIR}/build/${TARGET}/platform/edrv/btd/rdaCommon/lib/librdaCommon_release.a ${SOFT_WORKDIR}/platform/edrv/btd/rdaCommon/lib/librdaCommon_release.a

echo
echo "===================================="
echo "Updating finished"

