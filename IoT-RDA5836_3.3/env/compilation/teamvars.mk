#############################################################################
# Depending on the team, selects which module must be used as lib.
#############################################################################

ifeq "${CT_USER}" "ADMIN"
 TEAM_BINARY_LIBS := 
else
ifeq "${CT_USER}" "CMUX"
    TEAM_BINARY_LIBS := platform/base/packed
    TEAM_BINARY_LIBS += platform/stack/packed
    TEAM_BINARY_LIBS += platform/svc/packed
    TEAM_BINARY_LIBS += platform/vpp/packed
    TEAM_BINARY_LIBS += platform/service/packed
    TEAM_BINARY_LIBS += platform/system/mdi/packed
else
   # Only stack is used as a lib
   TEAM_BINARY_LIBS := platform/stack/packed  
endif   
endif

