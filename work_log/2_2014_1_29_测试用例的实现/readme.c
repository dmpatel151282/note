
2014-1-29

Audio模块:  共12个测试用例
    1. audio_codec_play_high_samplerate
        初步完成。
        
        后续：查看codec_play_stereo_mono_48k_192k 程序源码，注意其返回值
        
    2. audio_codec_play_10minutes
        初步完成。
        
        后续：查看codec_play_10min 程序源码，注意返回值

    3. audio_codec_play_0_data
        初步完成。

        后续：查看codec_play_0data 程序源码，注意返回值

    4. audio_codec_judge_left_channel
       audio_codec_judge_right_channel
       初步完成。

        后续：查看codec_judge_channel_left_right 程序源码，注意返回值

CPU模块:  共27个测试用例

  已完成:
    1. CPU_STRESS_FREQ_SWITCH_ORDER_0.05_100000
    2. CPU_STRESS_FREQ_SWITCH_RANDOM_0.05_100000
    3. CPU_STRESS_FREQ_SWITCH_RANDOM_1_10000
    4. CPU_STRESS_FREQ_SWITCH_ORDER_1_2000
    5. CPU_STRESS_CPU_HOTPLUG_0.5_5000

  下一个:
    6. CPU_STRESS_CPU1_STATUS_SWITCH_0.05_10000
    7. CPU_STRESS_CPU1_STATUS_SWITCH_1_2000

ion: 共9个测试用例
    已完成 9 个

    后续：查看 ion_test 程序源码

rtc:
  
issue:
    1. sine_detector/audio_check_wav 工具未找到?
    解决：找刘刚拿到源码
    2. 开关cpu1时，与自动调频有冲突，测试时是否能关掉自动调频?
    解决：固定开关核，写入1或2 > cpufreq/fantasys/hotplug_lock

--------------------------------------------------------------------------

2014-2-10

Audio模块:
    5. audio_codec_capture 测试用例
        修改audio_codec_capture_open源码，增加时间参数，以此为录音时间

    6. 

issue:
    1. 运行audio_codec_capture失败，audio_check_wav 时出现 NOt A SINE signal
    

--------------------------------------------------------------------------

2014-2-12
    初步完成，audio模块功能测试用例
      data区空间不足，将测试中的资源文件改放到sdcard中，修改所有脚本
