

ltp-ddt 的使用方法

安装
    执行 ltp-ddt.sh 脚本

1.  sh runltp [option]...

        sh runltp -P a23-evm -f ddt/audio_codec -t 24h

选项
    //测试什么
    -P PLATFORM     Platform to run tests on. Used to filter device driver tests (ddt)

    -f CMDFILES     Execute user defined list of testcases (separate with ',')
                    不指定默认执行，scenario_groups/default-ddt

    -S SKIPFILE     Skip tests specified in SKIPFILE

    // 测试结果的输出
    -l LOGFILE      Log results of test in a logfile.   测试结果 
    -o OUTPUTFILE   Redirect test output to a file.     测试过程打印
    -K DMESG_LOG_DIR
                    Log Kernel messages generated for each test cases inside this directory 

    通过create_dmesg_entries_for_each_test.awk
    在每条测试用例的命令前加上：dmesg -c 1>/dev/null 2>&1
                    命令后加上：dmesg > <testcasename>.dmesg.log

    -C FAILCMDFILE  Command file with all failed test cases.
    -d TMPDIR       Directory where temporary files will be created. 默认为/tmp
    -p              Human readable format logfiles. 

    // 常用
    -t:             指定测试的持续时,Define duration of the test in s,m,h,d.
    -q              Print less verbose output to screen. 安静模式
    -h              Help. Prints all available options.

    // 测试时，增加负载
    1. cpu 负载      

    -c NUM_PROCS    Run LTP under additional background CPU load 
                    [NUM_PROCS = no. of processes creating the CPU Load by spinning over sqrt()]

    genload -cpu NUM_PROCS &

    2. Secondary Storage 负载 
    -D <options>    Run LTP under additional background Load on Secondary Storage (Seperate by comma)

    NUM_PROCS,NUM_FILES,NUM_BYTES,CLEAN_FLAG
    [NUM_PROCS   = no. of processes creating Storage Load by spinning over write()]
    [NUM_FILES   = Write() to these many files (Defaults to 1 when value 0 or undefined)]
    [NUM_BYTES   = write these many bytes (defaults to 1GB, when value 0 or undefined)]
    [CLEAN_FLAG  = unlink file to which random data written, when value 1]

    genload --hdd $NUM_PROCS --hdd-files $NUM_FILES --hdd-bytes $NUM_BYTES [--hdd-noclean] &

    3. memory 负载
    -m <options>    Run LTP under additional background Load on Main memory (Seperate by comma)

    NUM_PROCS,CHUNKS,BYTES,HANGUP_FLAG
    [NUM_PROCS   = no. of processes creating main Memory Load by spinning over malloc()]
    [CHUNKS      = malloc these many chunks (default is 1 when value 0 or undefined)]
    [BYTES       = malloc CHUNKS of BYTES bytes (default is 256MB when value 0 or undefined) ]
    [HANGUP_FLAG = hang in a sleep loop after memory allocated, when value 1]

    genload --vm $NUM_PROCS --vm-chunks $CHUNKS --vm-bytes $BYTES [--vm-hang] &
    
    4. IO 负载

    -i NUM_PROCS    Run LTP under additional background Load on IO Bus

    [NUM_PROCS   = no. of processes creating IO Bus Load by spinning over sync()]

    genload --io $NUM_PROCS  &

    5. network traffic 负载
    -n              Run LTP with network traffic in background.
    
    netpipe.sh

    // 提高

    -M CHECK_TYPE    内存泄露检查
    [CHECK_TYPE=1 => Full Memory Leak Check tracing children as well]
    [CHECK_TYPE=2 => Thread Concurrency Check tracing children as well]
    [CHECK_TYPE=3 => Full Memory Leak & Thread Concurrency Check tracing children as well]

    通过create_valgrind_check.awk脚本

------------------------------------------------------------------------------------
    -a EMAIL_TO     EMAIL all your Reports to this E-mail Address   需安装mutt
    -g HTMLFILE     Create an additional HTML output format
    -e              Prints the date of the current LTP release
    -F LOOPS,PERCENTAGE Induce PERCENTAGE Fault in the Kernel Subsystems, and, run each test for LOOPS loop
    -N              Run all the networking tests. 
    -r LTPROOT      Fully qualified path where testsuite is installed.
    -s PATTERN      Only run test cases which match PATTERN.
    -T REPETITION   Execute the testsuite for REPETITION no. of times
    -w CMDFILEADDR  Uses wget to get the user's list of testcases.
    -x INSTANCES    Run multiple instances of this testsuite.
    -b DEVICE       Some tests require an unmounted block device to run correctly.
    -B DEVICE_FS_TYPE The file system of test block devices.
---------------------------------------------------------------------------
issue:

  1.无法执行ltp-pan，/system/bin/sh: ./ltp-pan: No such file or directory

原因：用readelf查看ltp-pan，它包含3个动态库。
有INTERP段，它指明了程序在加载过程中需要使用的动态链接加载器，如果指定
的这个加载器未找到，那么就会提示“ No such file or directory”

解决：ltp-ddt源码，include/mk/config.mk中，CFLAGS=-static，LDFLAGS=-static
再重新编译；make clean; make ....

  2.运行runltp出现： 
        ./runltp: line 989: /data/ltp/IDcheck.sh: not found
        WARNING: required users and groups not present
        WARNING: some test cases may fail
        ./runltp: line 989: /data/ltp/ver_linux: not found
        WARNING: unable to display versions of software installed

原因：在android shell环境下无法直接执行IDcheck.sh和ver_linux脚本.
解决：修改Ver_linux和IDcheck.sh：
        #!/bin/sh  ->   #!/system/bin/sh
      更好的方法是：在开发板中：
                        mkdir /bin; cd /bin; ln -s /data/busybox sh

  3.测试时出现：testcases/bin/ddt/common/common.sh: line 284: 
                syntax error: bad substitution

原因： busybox默认的shell为ash，有些语法是/bin/bash才有的
解决： 编译或下载可执行文件bash，
       adb push bash /system/bin/; 
       ln -s /bin/sh /system/bin/bash;
       ln -s /bin/bash /system/bin/bash;
       
  4. 测试时出现：Config Option and Module name not found.  

原因：在get_modular_config_names.sh中，
解决：在get_modular_config_names.sh中，加入
        *rtc0)
          rtc="CONFIG_RTC_DRV_SUNXI:rtc0";;

  5. 测试时出现：FATAL: error during prcessing alltests file by sed

原因：部分testscenarios没有通过check_feature_support

  6. 测试时出现：/proc/config.gz: cannot execute binary file
                 FATAL: Could not find CONFIG_RTC_HCTOSYS in config or 
                 the option was not set to m or y

原因：zcat命令不存在
解决：ln -s /data/busybox /system/bin/zcat

  7. 重启板子，/bin/目录、/usr目录消失; 

解决：在板子的/system/etc/下创建install-recovery.sh; chmod 755 ..

  8.  测试音频时出现：aplay: pcm_write:1939: write error: Input/output error

解决：不用alsa_test_suite 测音频
