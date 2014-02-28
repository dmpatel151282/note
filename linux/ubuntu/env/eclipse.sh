#!/bin/bash

DOC_PATH="/home/liushui/github/doc"
USER="/home/liushui"
install_dir="/home/liushui/usr"

echo ""
echo "********* install eclipse ***********"
eclipse_tar=`find ${DOC_PATH} -name eclipse*.tar.gz`
tar zxf ${eclipse_tar} -C ${install_dir}
echo "\n#for eclipse\nexport PATH=\$PATH:${install_dir}/eclipse" >> ${USER}/.bashrc
. ${USER}/.bashrc
echo "********* 安装eclipse 成功 *************"

