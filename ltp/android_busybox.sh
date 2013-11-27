#!/bin/sh
adb remount
adb push busybox /data/
adb shell chmod 777 /data/busybox
adb shell mount -o remount,rw /dev/block/nandd /system

adb shell ln -s /data/busybox /system/bin/sh
adb shell rm /system/bin/ls
adb shell ln -s /data/busybox /system/bin/ls
adb shell rm /system/bin/dirname
adb shell ln -s /data/busybox /system/bin/dirname
adb shell rm /system/bin/basename
adb shell ln -s /data/busybox /system/bin/basename
adb shell rm /system/bin/rm
adb shell ln -s /data/busybox /system/bin/rm
adb shell rm /system/bin/mkdir
adb shell ln -s /data/busybox /system/bin/mkdir
adb shell rm /system/bin/mktemp
adb shell ln -s /data/busybox /system/bin/mktemp
adb shell rm /system/bin/awk
adb shell ln -s /data/busybox /system/bin/awk
adb shell rm /system/bin/grep
adb shell ln -s /data/busybox /system/bin/grep
adb shell rm /system/bin/sed
adb shell ln -s /data/busybox /system/bin/sed
adb shell rm /system/bin/cut
adb shell ln -s /data/busybox /system/bin/cut
adb shell rm /system/bin/free
adb shell ln -s /data/busybox /system/bin/free
adb shell rm /system/bin/head
adb shell ln -s /data/busybox /system/bin/head
adb shell rm /system/bin/ldd
adb shell ln -s /data/busybox /system/bin/ldd
adb shell rm /system/bin/fdformat
adb shell ln -s /data/busybox /system/bin/fdformat
adb shell rm /system/bin/touch
adb shell ln -s /data/busybox /system/bin/touch
adb shell rm /system/bin/tr
adb shell ln -s /data/busybox /system/bin/tr
adb shell rm /system/bin/tar
adb shell ln -s /data/busybox /system/bin/tar