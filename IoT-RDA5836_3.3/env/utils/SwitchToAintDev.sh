#!/bin/bash

## This script will switch the Subversion working copy to the
## Modem2G ASIC Integration Team development branch (aint_dev).

echo "Switching to the Modem2G branches/aint_dev..."
cd $SOFT_WORKDIR
svn switch http://subversion-server/svn/developing/modem2G/branches/aint_dev .

echo "Done!"
