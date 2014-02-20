#!/bin/bash

DOC_PATH=`dirname $0`
install_dir="/home/liushui/usr"

function do_install () {
    read -p "是否安装 tomcat 到\"${install_dir}\" ? [Y/n]" check
    if [ $check = "Y" -o $check = "y" ]; then
        echo "********** install tomcat ***********"
        tarname=`find ${DOC_PATH} -name apache-tomcat*` || \
        {
            echo "未找到tar.gz包" 
            echo "安装失败"
            exit 1
        }
        tar zvxf $tarname -C ${install_dir} || \
        {
            echo "安装失败"
            exit 1
        }
        tarname=`basename $tarname`
        mv ${install_dir}/${tarname%.tar.gz} ${install_dir}/tomcat/
        
        sh ${install_dir}/tomcat/bin/startup.sh
        
        echo ""
        echo "********** install jenkins.war ***********"
        cp ${DOC_PATH}/jenkins.war ${install_dir}/tomcat/webapps || \
        {
            echo "未找到 jenkins.war" 
            echo "安装失败"
            exit 1
        }
        sh ${install_dir}/tomcat/bin/shutdown.sh
        sh ${install_dir}/tomcat/bin/startup.sh
        echo "********* install jenkins successful! ***********"
    else
        echo "安装终止"
        exit 1
    fi
}

function do_uninstall () {
    echo "********* uninstall tomcat and jenkins **********"
    sh ${install_dir}/tomcat/bin/shutdown.sh
    rm ${install_dir}/tomcat/ -rf
    echo "********* uninstall successful! ***********"
}

if [ "$1" = "-u" ]; then
    do_uninstall
else
    do_install
fi

