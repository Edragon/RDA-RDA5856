#!/usr/bin/bash 
echo ======== delete build =========
rm -f -r ./build
cdm
echo ======== start compile platform for cool_profile lib ========== 
WITHOUT_WERROR=1 CT_USER=ADMIN make 1>com.log
echo ======== start compile platform for debug lib ========== 
WITHOUT_WERROR=1 CT_USER=ADMIN CT_RELEASE=debug make 1>com.log
echo ======== start compile platform for release lib ========== 
WITHOUT_WERROR=1 CT_USER=ADMIN CT_RELEASE=release make 1>com.log
cds
echo ======== start copy lib ===========
mv -f ./build/platform/base/lib/libbase_cool_profile.a       platform/base/lib/libbase_cool_profile.a  
mv -f ./build/platform/base/lib/libbase_debug.a              platform/base/lib/libbase_debug.a         
mv -f ./build/platform/base/lib/libbase_release.a            platform/base/lib/libbase_release.a         

mv -f ./build/platform/csw/lib/libcsw_cool_profile.a         platform/csw/lib/libcsw_cool_profile.a    
mv -f ./build/platform/csw/lib/libcsw_debug.a                platform/csw/lib/libcsw_debug.a           
mv -f ./build/platform/csw/lib/libcsw_release.a              platform/csw/lib/libcsw_release.a           

mv -f ./build/platform/pal/lib/libpal_cool_profile.a         platform/pal/lib/libpal_cool_profile.a    
mv -f ./build/platform/pal/lib/libpal_debug.a                platform/pal/lib/libpal_debug.a           
mv -f ./build/platform/pal/lib/libpal_release.a              platform/pal/lib/libpal_release.a           

mv -f ./build/platform/stack/lib/libstack_cool_profile.a     platform/stack/lib/libstack_cool_profile.a
mv -f ./build/platform/stack/lib/libstack_debug.a            platform/stack/lib/libstack_debug.a       
mv -f ./build/platform/stack/lib/libstack_release.a          platform/stack/lib/libstack_release.a       

mv -f ./build/platform/svc/lib/libsvc_cool_profile.a         platform/svc/lib/libsvc_cool_profile.a    
mv -f ./build/platform/svc/lib/libsvc_debug.a                platform/svc/lib/libsvc_debug.a           
mv -f ./build/platform/svc/lib/libsvc_release.a              platform/svc/lib/libsvc_release.a           

mv -f ./build/platform/vpp/lib/libvpp_cool_profile.a         platform/vpp/lib/libvpp_cool_profile.a    
mv -f ./build/platform/vpp/lib/libvpp_debug.a                platform/vpp/lib/libvpp_debug.a           
mv -f ./build/platform/vpp/lib/libvpp_release.a              platform/vpp/lib/libvpp_release.a           

mv -f ./build/platform/chip/lib/libchip_cool_profile.a       platform/chip/packed/chip_greenstone_chip/lib/libchip_greenstone_chip_cool_profile.a
mv -f ./build/platform/chip/lib/libchip_debug.a              platform/chip/packed/chip_greenstone_chip/lib/libchip_greenstone_chip_debug.a
mv -f ./build/platform/chip/lib/libchip_release.a            platform/chip/packed/chip_greenstone_chip/lib/libchip_greenstone_chip_release.a

mv -f ./build/mdi/lib/libmdi_cool_profile.a                  mdi/lib/libmdi_cool_profile.a  
mv -f ./build/mdi/lib/libmdi_debug.a                         mdi/lib/libmdi_debug.a         
mv -f ./build/mdi/lib/libmdi_release.a                       mdi/lib/libmdi_release.a       
echo ======== compile platform finish =========

