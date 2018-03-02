#!/bin/bash

echo "Switching to the Subversion mirror..."
svn switch --relocate http://subversion-server/svn/developing svn://atlas/SVN_MIRROR/dev_mirror .

echo "Done!"
