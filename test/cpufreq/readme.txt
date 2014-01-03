
一、目录结构
cpufreq_table_detail.txt   -- cpu频点细表，pll1实际可调的频点
cpufreq_table_vf.txt       -- cpu v-f表中频点，1个电压对应1个该电压下最高频
                              率，用于单个频点的长时间老化测试
cpufreq_test.sh            -- 频点测试脚本

二、使用说明
1. 修改cpufreq_table_detail.txt、cpufreq_table_vf.txt文件
1.1 cpufreq_table_detail.txt    -- 随机频点切换测试使用该表中频点
    通过如下命令可以查看所有pll1实际可调频点
        $ cat /sys/devices/system/cpu/cpu0/cpufreq/stats/time_in_state
    或者查看源码lichee/linux-3.4/drivers/cpufreq/sunxi-cpufreq.c中
        struct cpufreq_frequency_table sunxi_freq_tbl[]

1.2 cpufreq_table_vf.txt        -- 单个频点长时间老化测试使用该表中频点
    在sys_config.fex中[dvfs_table]条目内定义

2. 将cpufreq_table_detail.txt、cpufreq_table_vf.txt、cpufreq_test.sh放入/system/bin/
  adb remount
  adb push cpufreq_table_detail.txt /system/bin/
  adb push cpufreq_table_vf.txt /system/bin/
  adb push cpufreq_test.sh /system/bin/

3. 执行脚本
串口中执行脚本
  $ su
  $ cd /system/bin/
  $ sh cpufreq_test.sh      // 在前台执行，脚本未执行完，串口不能输入
  或者
  $ sh cpufreq_test.sh &    // 在后台执行，好处是脚本未执行完，串口仍可输入

三、主要测试项目
1. 以0.5s间隔随机频点切换10W次
    读取cpufreq_table_detail.txt中频点，并随机选择，以0.5秒间隔切换
2. 以0.5s间隔开/关核1W次
3. 单个频点固定老化6h
    读取cpufreq_table_vf.txt中频点，并顺序选择，以6h间隔切换

四、注意事项
1. 修改cpufreq_table_detail.txt、cpufreq_table_vf.txt文件时注意字符编码保持
   utf-8格式，若转换为DOS格式会导致脚本不能正常识别

