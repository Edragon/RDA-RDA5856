#!/bin/bash

## This script will switch the Subversion working copy to the
## Modem2G trunk.

echo "Switching to the Modem2G trunk..."
cd $SOFT_WORKDIR
svn switch http://subversion-server/svn/developing/modem2G/trunk .

echo "Done!"
