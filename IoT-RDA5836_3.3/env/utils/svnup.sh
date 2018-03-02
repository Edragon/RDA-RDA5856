#!/bin/bash

REVISION="$1"

if [[ $REVISION = "" ]]
then
  REVISION="HEAD"
fi

echo svn up application    -r ${REVISION}
svn up application    -r ${REVISION}
echo svn up env            -r ${REVISION}
svn up env            -r ${REVISION}
echo svn up platform       -r ${REVISION}
svn up platform       -r ${REVISION}
echo svn up platform/base  -r ${REVISION}
svn up platform/base  -r ${REVISION}
echo svn up platform/chip  -r ${REVISION}
svn up platform/chip  -r ${REVISION}
echo svn up platform/csw   -r ${REVISION}
svn up platform/csw   -r ${REVISION}
echo svn up platform/edrv  -r ${REVISION}
svn up platform/edrv  -r ${REVISION}
echo svn up platform/mdi   -r ${REVISION}
svn up platform/mdi   -r ${REVISION}
echo svn up platform/stack -r ${REVISION}
svn up platform/stack -r ${REVISION}
echo svn up platform/svc   -r ${REVISION}
svn up platform/svc   -r ${REVISION}
echo svn up platform/vpp   -r ${REVISION}
svn up platform/vpp   -r ${REVISION}
echo svn up platform_test  -r ${REVISION}
svn up platform_test  -r ${REVISION}
echo svn up target         -r ${REVISION}
svn up target         -r ${REVISION}
echo svn up toolpool       -r ${REVISION}
svn up toolpool       -r ${REVISION}
