#!/bin/bash

echo "Switching back to Beijing Subversion server..."
svn switch --relocate svn://atlas/SVN_MIRROR/dev_mirror http://subversion-server/svn/developing .

echo "Done!"
