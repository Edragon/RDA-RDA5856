.SILENT:

ECHO = @echo
SHELL = /bin/bash

all: help

help:
	$(ECHO) "-------------------------------------------------------"
	$(ECHO) "This makefile allows you to update your project workdir"
	$(ECHO) "-------------------------------------------------------"
	$(ECHO) "To use it you must be in the soft/env directory"
	$(ECHO) "make platform       update your platform directory"
	$(ECHO) "make platform_test  update your platform directory"
	$(ECHO) "make application    update your application directory"
	$(ECHO) "make target         update your target directory"
	$(ECHO) "make env         	 update your env directory"



platform:
	svn update ../platform

platform_test:
	svn update -N ../platform_test

application:
	svn update ../application

target:
	svn update ../target

env:
	svn update ../env


