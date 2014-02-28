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


echo ""
echo "********* config vim  ***********"
cp ${DOC_PATH}/vim/vim  ${USER}/.vim -frv
cp ${DOC_PATH}/vim/vimrc ${USER}/.vimrc -v
echo "alias vi=vim " >> ${USER}/.bashrc
. ${USER}/.bashrc
echo "********* vim 配置完成 ***********"


echo ""
echo "********* config git  ***********"

echo "********* git 配置完成 ***********"


echo ""
echo "********* install jdk ***********"
jdk_tar=`find ${DOC_PATH} -name jdk*.tar.gz`
tar zvxf ${jdk_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/jdk/
echo  "\n#for java\nJAVA_HOME=${install_dir}/jdk"  >> ${USER}/.bashrc
echo  "CLASSPATH=\$JAVA_HOME/lib:\$JAVA_HOME/jre/lib" >> ${USER}/.bashrc
echo  "export JAVA_HOME CLASSPATH\nexport PATH=\$PATH:\$JAVA_HOME/bin\n" >> ${USER}/.bashrc
. ${USER}/.bashrc
echo "********* jdk 安装完成 ***********"


echo ""
echo "********** install ant ***********"
ant_tar=`find ${DOC_PATH} -name apache-ant*.tar.gz`
tar zvxf ${ant_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/ant/
echo "\n#for ant\nexport ANT_HOME=${install_dir}/ant\nexport PATH=\$PATH:\$ANT_HOME/bin\n" >> ${USER}/.bashrc
. ${USER}/.bashrc

find ${DOC_PATH} -name "jsch-0.1.50.jar" | xargs -t -i cp {} -v ${install_dir}/ant/lib/ || \
{
    echo "ssh 功能将不能使用"
}
echo "********** ant 安装成功 **********"


echo ""
echo "********* install android sdk ***********"
sdk_tar=`find ${DOC_PATH} -name android-sdk*.tgz`
tar zvxf ${sdk_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/android-sdk/
echo "\n#for android-sdk\nexport PATH=\$PATH:${install_dir}/android-sdk/tools" >> ${USER}/.bashrc
. ${USER}/.bashrc
echo "********* 安装android sdk 成功 *************"


echo ""
echo "********* install android ndk ***********"
ndk_tar=`find ${DOC_PATH} -name android-ndk*.tar.bz2`
tar jvxf ${ndk_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/android-ndk/
echo "\n#for android-sdk\nexport PATH=\$PATH:${install_dir}/android-ndk/" >> ${USER}/.bashrc
. ${USER}/.bashrc
echo "********* 安装android ndk 成功 *************"


####################### allwinnertech ########################
#echo ""
#echo "********* install Livesuit *******************"
#apt-get install dkms --force-yes -y
#chmod +x ${DOC_PATH}/livesuit/LiveSuit.run || \
#{
#    echo "安装LiveSuit失败"
#    exit 
#}
#${DOC_PATH}/livesuit/LiveSuit.run
#echo "SUBSYSTEM!=\"usb_device\", ACTION!=\"add\", GOTO=\"objdev_rules_end\"
##USBasp
#ATTRS{idVendor}==\"1f3a\", ATTRS{idProduct}==\"efe8\", GROUP=\"liushui\",
#MODE=\"0666\"
#LABEL=\"objdev_rules_end\"" > /etc/udev/rules.d/10-local.rules
#sudo service udev restart
#cp ${DOC_PATH}/livesuit/LiveSuit_unformat ${USER}/Bin/LiveSuit/bin/LiveSuit -v || \
#{
#    echo "修改默认不格式化刷机失败"
#}
#chmod 755 ${USER}/Bin/LiveSuit/bin/LiveSuit
#
#echo "*********** install LiveSuit successful! *************"
