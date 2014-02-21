#!/bin/bash

DOC_PATH=
USER="/home/liushui"
install_dir="/home/liushui/usr"

echo ""
echo "********* 升级系统 ************"
apt-add-repository ppa:tualatrix/ppa  #for ubuntu-tweak
apt-get upgrade

echo""
echo "********* 解决gedit乱码 *********"
gsettings set org.gnome.gedit.preferences.encodings auto-detected "['UTF-8','GB18030','GB2312','GBK','BIG5','CURRENT','UTF-16']"
echo "********* 乱码解决完毕 *********"

echo ""
echo "******** 删除一些没用的软件 ***********"
apt-get remove totem totem-gstreamer totem-mozilla --force-yes  -y
apt-get remove rhythmbox evolution bittorrent empathy --force-yes  -y


echo ""
echo "*********** 安装必要软件 *************"
software=(vim git libncurse bison flex curl wget lynx unrar unzip chmsee ksnapshot ubuntu-tweak)
function do_apt_get_install () {
    soft=$1
    echo ""
    echo "********* install ${soft} ***********"
    apt-get install ${soft} --force-yes -y
    echo "********* install ${soft} successful! ***********"
}
for s in ${software[@]}
do
    do_apt_get_install $s
done 

echo ""
echo "********* config vim  ***********"
cp ${DOC_PATH}/vim/vim  ${USER}/.vim -frv
cp ${DOC_PATH}/vim/.vimrc ${USER}/.vimrc -v
echo "alias vi=vim " >> ${USER}/.bashrc
source ${USER}/.bashrc

echo ""
echo "********* config git  ***********"


echo ""
echo "********* install android sdk ***********"
sdk=`find ${DOC_PATH} -name android-sdk*.tgz`
tar zxf ${sdk} -C ${install_dir}
mv ${install_dir}/android-sdk-linux/ ${install_dir}/android-sdk/
echo -e "\n#for android-sdk\nexport PATH=$PATH:${install_dir}/android-sdk/tools" >> ${USER}/.bashrc

echo ""
echo "********* install eclipse ***********"
ecl=`find ${DOC_PATH} -name eclipse*.tar.gz`
tar zxf ${ecl} -C ${install_dir}
echo -e "\n#for eclipse\nexport PATH=$PATH:${install_dir}/eclipse" >> ${USER}/.bashrc

echo ""
echo "********* install arm-none-linux-gnueabi- **********"

###################### allwinnertech ########################
echo ""
echo "********* install Livesuit *******************"
apt-get install dkms --force-yes -y
chmod +x ${DOC_PATH}/livesuit/LiveSuit.run || \
{
    echo "安装LiveSuit失败"
    exit 
}
${DOC_PATH}/livesuit/LiveSuit.run
echo "SUBSYSTEM!=\"usb_device\", ACTION!=\"add\", GOTO=\"objdev_rules_end\"
#USBasp
ATTRS{idVendor}==\"1f3a\", ATTRS{idProduct}==\"efe8\", GROUP=\"liushui\",
MODE=\"0666\"
LABEL=\"objdev_rules_end\"" > /etc/udev/rules.d/10-local.rules
sudo service udev restart
cp ${DOC_PATH}/livesuit/LiveSuit_unformat ${USER}/Bin/LiveSuit/bin/LiveSuit -v || \
{
    echo "修改默认不格式化刷机失败"
}
chmod 755 ${USER}/Bin/LiveSuit/bin/LiveSuit

echo "*********** install LiveSuit successful! *************"

