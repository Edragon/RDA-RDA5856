##############################################################################
# Include customvars.mk if it exists
##############################################################################
ifneq "$(IGNORE_CUSTOM_VARS)" "1"
ifneq "$(wildcard $(SOFT_WORKDIR)/env/compilation/customvars.mk)" ""
include ${SOFT_WORKDIR}/env/compilation/customvars.mk
endif
endif

#############################################################################
# Check if CT_TARGET is correctly defined and inlcude corresponding target.def
#############################################################################
ifeq "$(wildcard $(SOFT_WORKDIR)/target/$(CT_TARGET)/target.def)" ""
    $(error "!!!! CT_TARGET not valid - CT_TARGET=${CT_TARGET} is not supported or not well defined !!!!")
else
# Include the Target definition
include ${SOFT_WORKDIR}/target/${CT_TARGET}/target.def
endif

#############################################################################
# Check if CT_ASIC is correctly defined and include corresponding ${CT_ASIC}.def
#############################################################################
ifeq "$(wildcard $(SOFT_WORKDIR)/platform/chip/$(CT_ASIC)/defs/$(CT_ASIC)_$(CT_ASIC_CFG).def)" ""
    $(error "!!!! No ASIC named: CT_ASIC=$(CT_ASIC)_$(CT_ASIC_CFG) !!!!")
else
# Include the ASIC definition
include ${SOFT_WORKDIR}/platform/chip/$(CT_ASIC)/defs/${CT_ASIC}_${CT_ASIC_CFG}.def
endif

#############################################################################
# Include the file which defines which modules must be used as lib.
#############################################################################
include ${SOFT_WORKDIR}/env/compilation/teamvars.mk
GLOBAL_BINARY_LIBS += ${TEAM_BINARY_LIBS}

#############################################################################
# Include the file which defines some properties for the platform
#############################################################################
ifneq "$(wildcard $(SOFT_WORKDIR)/platform/platform.def)" ""
include $(SOFT_WORKDIR)/platform/platform.def
endif

#############################################################################
# Compile all the lists of flags (except for the local list).
#############################################################################
ALL_EXPORT_FLAG := ${GLOBAL_EXPORT_FLAG} ${TARGET_EXPORT_FLAG} ${CHIP_EXPORT_FLAG}

COMPILEVARS_DEFINED:=yes
