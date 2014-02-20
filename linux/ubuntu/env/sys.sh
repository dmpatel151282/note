#!/bin/bash

CUR_FILE_PATH=`dirname $0`

software=(vim git)

function do_apt_get_install () {
    soft=$1

    read -p "是否安装 ${soft} ? [Y/n]" check
    if [ $check = "Y" -o $check = "y" ]; then
        echo ""
        echo "********* install ${soft} ***********"
        apt-get install ${soft}
        echo "********* install ${soft} successful! ***********"
    
    else 
        echo "${soft} 安装终止。"
    fi
}

for s in ${software[@]}
do
    do_apt_get_install $s
done 
