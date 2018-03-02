#!/bin/bash

#script executable
find ${SOFT_WORKDIR}/application/mmi_customer -name *.sh | xargs chmod +x
find ${SOFT_WORKDIR}/application/target_res -name *.sh | xargs chmod +x

echo "Cleaning all target..."
rm -rf ${SOFT_WORKDIR}/build
echo "Clean done."

LOGGINGDIR=${SOFT_WORKDIR}/env/utils/logging
echo "Create logging file"
rm -rf ${LOGGINGDIR}
mkdir -p ${LOGGINGDIR}
touch ${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
echo "Create logging file done."

###################TARGET 1#########################
TARGET=8853_wqvga_pda
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8853_wqvga_pda CT_RELEASE=debug CT_RESGEN=yes  CT_PRODUCT=bv5 clean lod

###################TARGET 2#########################
TARGET=8853_qvga_3264
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8853_qvga_3264 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 3#########################
TARGET=8853_qvga_6432
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8853_qvga_6432 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 4#########################
TARGET=8852_qcif_3232
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8852_qcif_3232 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 5#########################
TARGET=8852_qcif_3216
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8852_qcif_3216 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 6#########################
TARGET=8852_qcif_3264
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8852_qcif_3264 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 7#########################
TARGET=gallite_r440_3232
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=gallite_r440_3232 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 8#########################
TARGET=8853_qvga_3216
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8853_qvga_3216 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 9#########################
TARGET=8853_qvga_3232
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8853_qvga_3232 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

###################TARGET 10#########################
TARGET=8852_320x240tp_6432
echo "Building Target:${TARGET}"
echo "#######Start building Target:${TARGET}#######" >>${LOGGINGDIR}/log.$(date +"%Y%m%d")txt
make CT_USER=FAE CT_TARGET=8852_320x240tp_6432 CT_RELEASE=debug CT_RESGEN=yes CT_PRODUCT=bv5 clean lod

echo "Build all targets done!!!"

