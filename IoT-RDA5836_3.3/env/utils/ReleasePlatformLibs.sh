#!/bin/bash

## This script will build all the .

echo "Creating the libraries..."

#TODO: maybe better with a foreach in the ${BINARY_LIBS}
#for module in ${BINARY_LIBS}
#do
#
#done

cd $SOFT_WORKDIR/platform/calib
make clean binlib

cd $SOFT_WORKDIR/platform/base/sx
make clean binlib

#cd $SOFT_WORKDIR/platform/chip/packed
cd $SOFT_WORKDIR/platform/chip
make clean binlib
mv $SOFT_WORKDIR/platform/chip/lib $SOFT_WORKDIR/platform/chip/packed/${CT_ASIC}

cd $SOFT_WORKDIR/platform/csw
make clean binlib

#cd $SOFT_WORKDIR/platform/edrv/packed
cd $SOFT_WORKDIR/platform/edrv
make clean binlib
mv $SOFT_WORKDIR/platform/edrv/lib $SOFT_WORKDIR/platform/edrv/packed/${CT_TARGET}

cd $SOFT_WORKDIR/platform/pal
make clean binlib

cd $SOFT_WORKDIR/platform/stack
make clean binlib

cd $SOFT_WORKDIR/platform/svc
make clean binlib

cd $SOFT_WORKDIR/platform/vpp/amjp
make clean binlib

cd $SOFT_WORKDIR/platform/vpp/speech/std
make clean binlib

echo "Done!"
