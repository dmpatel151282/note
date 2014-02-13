
runltp 处理参数 -> bin/ltp-pan 
                    1.读取runtest中的文件
                    2.执行testcase中的脚本和可执行文件   
-------------------------------------------------------------------------
runltp -P a23-evm -f ddt/rtc_alarm

CMDFILES=ddt/rtc_alarm
PLATFORM=a23-evm

1.根据a23-evm，
  ARCH=
  SOC=
  MACHINE=
  DRIVERS=       //逗号相隔
在${TMP}/platform/下创建文件夹

2.将-f指定的所有文件内容 >> 到${TMP}/alltests

3.${LTPROOT}/IDcheck.sh

4.${LTPROOT}/ver_linux

5.过滤需要块设备的test case
    sed -i "/DEVICE/d"  ${TMP}/alltests

6.执行ltp-pan
    /data/ltp/bin/ltp-pan  -e -S   -a 2183     -n 2183  -p  -f /tmp/ltp-XXXXLl1IIc/alltests -l /data/ltp/results/LTP_RUN_ON-2013_Dec_31-10h_58m_07s_2183.log  -C /data/ltp/output/LTP_RUN_ON-2013_Dec_31-10h_58m_07s_2183.failed
    
7.处理测试结果


例：rtc_alarm

1. source 'common.sh'; 
2. do_cmd install_modules.sh 'rtc' ; 
    1) get_modular_config_names.sh
         获取模块名和配置项
    2) zcat /proc/config.gz 确认是否编译到内核
    3) 如果是模块，则执行insmod.sh脚本，插入模块
  
3. DEV_NODE=`get_devnode.sh "rtc"` || die "error getting devnode for rtc"; 
    1) case 列表中找[rtc]，/dev/rtc0
    2）未找到，调用get_blk_device_node.sh "rtc"

4. do_cmd rtc_tests -device $DEV_NODE  -ioctltest alarm -ioctltestarg 30 
     调用测试程序rtc_test测试；
