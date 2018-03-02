#!/bin/bash

echo ======== build target=========
mkdir -p ./platform/stack/packed/stack_dualsim/lib
mkdir -p ./platform/stack/packed/stack_threesim/lib
mkdir -p ./platform/stack/packed/stack_foursim/lib
mkdir -p ./platform/stack/packed/stack_dualsim_gprs/lib
mkdir -p ./platform/stack/packed/stack_threesim_gprs/lib
mkdir -p ./platform/stack/packed/stack_foursim_gprs/lib
echo "-------Dual SIM GSM only--------"
rm -rf ./build/8853_gh021/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_dualsim/lib/libstack_dualsim_debug.a

rm -rf ./build/8853_gh021/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_dualsim/lib/libstack_dualsim_release.a

echo "-------Three SIM GSM only--------"
rm -rf ./build/8853_gh021/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_threesim/lib/libstack_threesim_debug.a

rm -rf ./build/8853_gh021/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_threesim/lib/libstack_threesim_release.a

echo "-------Four SIM GSM only--------"
rm -rf ./build/8853_gh021/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=4	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_foursim/lib/libstack_foursim_debug.a

rm -rf ./build/8853_gh021/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_gh021 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=4	platform/stack 
mv -f ./build/8853_gh021/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_foursim/lib/libstack_foursim_release.a

echo "-------Dual SIM GPRS--------"
rm -rf ./build/8853_hk603/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_dualsim_gprs/lib/libstack_dualsim_gprs_debug.a

rm -rf ./build/8853_hk603/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=2	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_dualsim_gprs/lib/libstack_dualsim_gprs_release.a

echo "-------Three SIM GPRS--------"
rm -rf ./build/8853_hk603/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_threesim_gprs/lib/libstack_threesim_gprs_debug.a

rm -rf ./build/8853_hk603/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=3	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_threesim_gprs/lib/libstack_threesim_gprs_release.a

echo "-------Four SIM GPRS--------"
rm -rf ./build/8853_hk603/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=debug CT_RESGEN=no NUMBER_OF_SIM=4	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_debug.a     platform/stack/packed/stack_foursim_gprs/lib/libstack_foursim_gprs_debug.a

rm -rf ./build/8853_hk603/platform/stack
ctmake CT_USER=ADMIN CT_TARGET=8853_hk603 CT_RELEASE=release CT_RESGEN=no NUMBER_OF_SIM=4	platform/stack 
mv -f ./build/8853_hk603/platform/stack/lib/libstack_release.a     platform/stack/packed/stack_foursim_gprs/lib/libstack_foursim_gprs_release.a

echo ======== compile platform finish =========
echo "Done!!"

