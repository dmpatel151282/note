#!/system/bin/sh

mkdir /bin/
ln -s /system/bin/bash /bin/sh
ln -s /system/bin/bash /bin/bash
mkdir /lib/
cp /system/lib/ld-linux.so.3 /lib/ld-linux.so.3
