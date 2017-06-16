#!/bin/bash


svn_ver=`svn up|grep revision|awk -F' ' '{ print $3 }'|awk -F'.' '{print $1}'`
#svn_ver=`svn info | grep Revision | awk '{print $2}'`  
Time=\"$(date '+%Y-%m-%d %H:%M:%S')\"  
Output="#define BUILD_TIMER     ${Time}"     
echo "/* This is generate by the shell*/" > version_svn_bulid.h
echo "#ifndef __SVN_VERSION__"   >> version_svn_bulid.h
echo "#define __SVN_VERSION__    1"   >> version_svn_bulid.h
echo "#define BUILD_SVN_VERSION    ${svn_ver}" >> version_svn_bulid.h
echo "${Output}" >> version_svn_bulid.h  
echo "#endif" >> version_svn_bulid.h


make