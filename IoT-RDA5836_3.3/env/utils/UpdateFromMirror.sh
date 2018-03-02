#!/bin/bash

## This script will switch to the Subversion soft mirror,
## do a "svn update" and switch back to the Beijing Subversion
## server.

## After that you can run a "svn update" to get the very latest
## (less than one hour) updates from the Beijing Subversion server.

echo "Switching to the Subversion mirror..."
svn switch --relocate http://subversion-server/svn/developing svn://atlas/SVN_MIRROR/dev_mirror .

echo "Updating from the Subversion mirror..."
svn update

echo "Switching back to Beijing Subversion server..."
svn switch --relocate svn://atlas/SVN_MIRROR/dev_mirror http://subversion-server/svn/developing .

echo "Done!"
