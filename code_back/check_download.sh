#!/bin/sh
set -e

MySelf=$0
MySelf=${MySelf##*/}
#echo "MySelf=${MySelf}"
FileName=`ls | grep -v ${MySelf}`
echo "1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "-->FileName=${FileName}s"
echo "2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

#for CheckName in ${FileName}
#do
#	echo ${CheckName}
#done
#exit 0