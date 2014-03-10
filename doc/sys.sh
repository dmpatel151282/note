#!/bin/bash

echo ""
apt-add-repository ppa:tualatrix/ppa        #for ubuntu-tweak
add-apt-repository ppa:phablet-team/tools   #for android-tools-adb
apt-get update

echo""
echo "********* 解决gedit乱码 *********"
gsettings set org.gnome.gedit.preferences.encodings auto-detected "['UTF-8','GB18030','GB2312','GBK','BIG5','CURRENT','UTF-16']"
echo "********* 乱码解决完毕 *********"

echo ""
echo "******** 删除一些没用的软件 ***********"
apt-get remove totem totem-gstreamer totem-mozilla --force-yes  -y
apt-get remove rhythmbox evolution bittorrent empathy --force-yes  -y
echo "******** 删除软件完成 ***********"

echo ""
echo "*********** 安装必要软件 *************"
apt-get install vim --force-yes -y
apt-get install git --force-yes -y
apt-get install minicom --force-yes -y
apt-get install libncurse --force-yes -y
apt-get install bison flex autoconf automake --force-yes -y
apt-get install tree wget curl ubuntu-tweak --force-yes -y
apt-get install zip unzip rar unrar --force-yes -y
apt-get install ksnapshot --force-yes -y
echo "********* 安装完成 ***********"
