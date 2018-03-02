## Libs that we generally use only as binary
## For example, with the following line, you can avoid compiling the stack.
#GLOBAL_BINARY_LIBS += platform/stack

## Extra flags exported to all makefiles.
GLOBAL_EXPORT_FLAG += 

## This will define the default target for which you will compile,
## if you don't specify a CT_TARGET on the command line.
CT_TARGET := greenstone_eng_ct1010

## This defines the team you're in.
## Used to determine which modules are used as libraries. Available values:
## Empty or CSW, STACK, SVC, MEDIA, CHIP, EDRV, MMI or ADMIN.
CT_USER := CSW

## This is useful to compile your code as a Ramrun.
#CT_RAMRUN := 1

## We can force to use a Romuator here (normally this is defined by the target).
#CT_ROMULATOR := yes

## This defines the release mode in which you compile.
CT_RELEASE := cool_profile

## Define VERBOSE to get more information during the make process.
#VERBOSE := 1

## To use the ccache tool (accelerate the compilation).
#CCACHE_ENABLE := 1

WITH_PERL ?= 1
WITH_SVN  ?= 1

