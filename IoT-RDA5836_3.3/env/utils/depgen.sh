#!/bin/bash
# modify the windows path to linux path in depends file 
echo $2 | sed 's/\//\\\//g' > deppath
DEPPATH=`cat deppath`
rm -rf deppath
#echo $DEPPATH
#echo cat $1  sed 's/\\/\//g'  sed 's/'$3'/'$2/$3'/g' 
cat $1 | sed 's/\\/\//g' | sed 's/^'$3'/'$DEPPATH'\/'$3'/g' > $1

