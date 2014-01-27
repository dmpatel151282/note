#!/bin/bash

CUR_FILE_PATH=`dirname $0`

# bash 
adb remount || \
{
    exit 1
}
adb push $CUR_FILE_PATH/doc/bash /system/bin/ || \
{
    exit 2
}
adb shell chmod 755 /system/bin/bash
adb shell mkdir /bin || \
{
    echo "mkdir /bin failed"
    exit 3
}
adb shell ln -s /system/bin/bash /bin/sh
adb shell ln -s /system/bin/bash /bin/bash

# busybox
adb push $CUR_FILE_PATH/doc/busybox /data/ || \
{
    exit 2
}
adb shell chmod 777 /data/busybox

cmd=(ls dirname basename rm mkdir mktemp cut awk sed grep find free head ldd fdformat touch tr tar printf expr uname zcat depmod which tail) 

for i in ${cmd[@]} 
do 
    echo "adb shell rm /system/bin/$i"
    adb shell rm /system/bin/$i
    echo "adb shell ln -s /data/busybox /system/bin/$i"
    adb shell ln -s /data/busybox /system/bin/$i
done

# arm-none-linux-gnueabi lib
adb push $CUR_FILE_PATH/doc/lib/ /system/lib/ || \
{
    exit 2
}

# install ltp-ddt
adb push $CUR_FILE_PATH/ltp/ltp/ /data/ltp/ || \
{
    exit 2
}

# exec install-recovery.sh
adb shell sh /data/ltp/install-recovery.sh || \
{
    exit 3
}
