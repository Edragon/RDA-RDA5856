#!/bin/bash

cd $SOFT_WORKDIR
echo "--- " > toolpool/map/ChipStd/traceDb.yaml
grep -R 'TSTR(.*,[ \t]*0x[0-9a-zA-Z]*)' platform/base/sx/ platform/stack/ --exclude-dir=.svn --include="*.[ch]" | sed 's/.*TSTR(\(.*\),[ \t]*\(0x[0-9a-zA-Z]*\)).*/\2: \1/' >> toolpool/map/ChipStd/traceDb.yaml

