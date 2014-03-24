#!/bin/bash

function am_patch() {

    find ./ -type f | grep .patch | while read line
    do
        echo "liushui $line"
        tmp=$(dirname $line | cut -b 2-)
        project_path=${1}${tmp}

        echo "project: $project_path"
        cp $line $project_path

        cd $project_path || \
        {
            echo "cd $project_path failed!"  
            cd -
        }
        
        patch=`basename $line`
        echo "  patch: $patch"
        git am --ignore-whitespace --3way  $patch
        cd -
        
    done
}

android_root="/home/zhengjinyong/a23_android4.4/android"
CUR_FILE_PATH=`pwd`
CUR_FILE_DIR=`basename ${CUR_FILE_PATH}`
if [ "x$CUR_FILE_DIR" != "xpatch" ]; then
    echo "this script must run in \"patch\" dir."
    exit 1
fi

am_patch $android_root
