
a23音频驱动

源码位置：
1.音频驱动位置:linux-3.4/sound/soc/sunxi/
2.switch耳机检测驱动位置:linux-3.4/drivers/staging/android/switch/

  I2S, PCM 驱动,是接口驱动,适用于所有支持 I2S,PCM 接口的设备
  Audio Codec 是芯片内置硬件驱动
  耳机检测驱动,适用于所有 3 节或 4 节耳机,注意,硬件上的 layout

5个音频设备驱动，分别为audiocodec，hdmiaudio，spdif，pcm(i2s0)，i2s(i2s1)
1个耳机检测驱动switch

设备文件：/dev/snd/
    1.controlC0     用于声卡的控制，例如通道选择，混音，麦克风的控制等 
    2.pcmC0D0c      用于录音的pcm设备
    3.pcmC0D0p      用于播放的pcm设备 
    4.timer         定时器

C0D0代表的是声卡0中的设备0，pcmC0D0c的c代表capture，
pcmC0D0p的p代表playback，这些都是alsa-driver中的命名规则。

-----------------------------------------------------------------------
框架:
  
                         audio_hw
    _________________________|__________ 
   |                                    |
   |         /proc/asound        Tiny_alsa/lib_audio   
   |         /sys/class/sound           |                 
   |                 |__________________|                 user space
---------------------|---------------------------------------------
   |          alsa sound core                             kernel space
   |                 |
   |                 |
   |          alsa-asoc-framework
   |                 | 
   |                 |___________________________
   |                 |              |            |
switch_drv        audiocodec    i2s0/daudio  i2s1/daudio
---|--------------------|-----------|------------|-------------------------
audio_line_hardware  speaker/mic  3g音频        蓝牙      Hardward space


  在音频框架中，audiocodec属于模拟音频部分，hdmiaudio，spdif，pcm，i2s属于
数字音频。其中audiocodec在3g音频通话中支持模拟音频通路和通话录音功能接口。
pcm,i2s都可以配置成pcm和i2s模式，代码中为了区分，一个以pcm命名，一个以i2s
命名。Hdmiaudio，spdif支持raw data模式。耳机检测驱动switch支持android标准的
耳机检测接口。

  在模拟音频驱动audiocodec中，支持ADC录音，DAC播放，模拟音频通路。支持四路
音频输入（mic1，mic2/mic3，phone in，line in），四路音频输出（phone out，
headphone，earphone，Speaker）。在硬件设计中，mic2用于耳机mic，mic3用于降噪
，mic2跟mic3不能同时使用.



