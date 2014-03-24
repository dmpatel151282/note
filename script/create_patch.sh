#!/bin/bash

function create_patch() {
    branch=$1
    cat .repo/manifest.xml | grep project | while read line
    do
        tmp=$(echo $line | awk '{print $2}')
        tmp=$(echo $tmp | cut -b7-) 
        project_path=$(echo $tmp | cut -c1-$((${#tmp}-1))) 

        echo "project: $project_path"
        cd $project_path 

        git checkout $branch  || \
        {
            echo "switch branch failed" 
            cd -
            continue
        }

        git_diff_output=$(git diff remotes/exdroid/kitkat-a23)
        
        if [ -n "${git_diff_output}" ]; then
            rm 0*.patch
            git format-patch remotes/exdroid/kitkat-a23 
            cur_path=`pwd`
            path=$(echo $cur_path | cut -d / -f 1-4)
            path=${path}"/patch/"$(echo $cur_path | cut -d / -f 6-)
            mv 0*.patch $path   
        fi
        cd -
    done
}

if [ $# -ne 1 ]; then
    echo -e "  Parameters are not available.\n  usage: you need put the branch name to switch.\n  eg: $0 branch_name"
    exit 1
fi

CUR_FILE_PATH=`pwd`
CUR_FILE_DIR=`basename ${CUR_FILE_PATH}`
if [ "x$CUR_FILE_DIR" != "xandroid" ]; then
    echo "this script must run in \"android\" dir."
    exit 1
fi

create_patch $1
