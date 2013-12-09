
声卡驱动架构：
    OSS     开放声音系统           商业
    ALSA    先进Linux声音架构      开源 GPL

---------------------------------------------------------
典型的数字音频系统:

    嵌入式微控制器/DSP ：集成了PCM、IIS 或 AC97 音频接口
        |
        |
        |PCM、IIS 或 AC97接口
        |
        |
    音频编解码器codec
      |     |
      |     |
   麦克风  喇叭 

-----------------------------------------------------------
codec
1.采样频率(横坐标)    
2.量化精度(纵坐标)

-----------------------------------------------------------
PCM 接口 ：脉冲编码调制

  1.钟脉冲(BCLK)
  2.帧同步信号(FS)
  3.接收数据(DR)
  4.发送数据(DX)

每个音频通道获得一个独立的数据队列

应用：
-----------------------------------------------------------
IIS 接口

Inter-IC Sound 在 20 世纪 80 年代首先被 PHILIPS 用于消费音频产品

LRCLK(Left/Right CLOCK)的信号机制中经过多路转换,将两路音频信号变成单一的数据队列。
    LRCLK 为高时,左声道数据被传输;
    LRCLK 为低时,右声道数据被传输;

应用：一般 CD、MD、MP3 随身听
------------------------------------------------------------
AC97 接口

Audio Codec 1997 是以 Intel 为首的 5 个 PC 厂商 Intel、Creative Labs、
NS、Analog Device 与 Yamaha 共同提出的规格标准。

AC-Link   与外部的编解码器相连,
  AC-Link接口：
    1.位时钟(BITCLK)
    2.同步信号校正(SYNC)
    3.从编码到处理器及从处理器中解码(SDATDIN 与 SDATAOUT)的数据队列

应用：具有音频功能的 PDA
-------------------------------------------------------------
Linux ALSA 音频设备驱动

特点：
  1.支持多种声卡设备
  2.模块化的内核驱动程序
  3.支持 SMP 和多线程
  4.提供应用开发函数库(alsa-lib)以简化应用程序开发
  5.支持 OSS API，兼容 OSS 应用程序

ALSA 系统：
必需1.alsa-driver       驱动包                  内核驱动程序
    2.alsa-libs         开发包                  用户空间的函数库
    3.alsa-libplugins   开发包插件 
    4.alsa-utils        设置管理工具包          aplay、arecord
    5.alsa-tools        相关处理小程序包 
    6.alsa-firmware     特殊音频固件支持包 
    7.alsa-oss          OSS接口兼容模拟层工具  

ALSA框架：

hardware
    |
    |
alsa-driver
    |
    | 信息接口(Information Interface,/proc/asound);
    | 控制接口(Control Interface,/dev/snd/contro|CX);
    | 混音器接口(Mixer Interface,/dev/snd/mixerCXDX);
    | PCM 接口(PCM Interface,/dev/snd/pcmCXDX);
    | Raw 迷笛接口(Raw MIDI Interface,/dev/snd/midiCXDX);
    | 音序器接口(Sequencer Interface,/dev/snd/seq);
    | 定时器接口(Timer Interface,/dev/snd/timer);
    |
alsa-soc ：对alsa-driver的封装，针对嵌入式设备提供了一些增强的功能
    |
    |
alsa-libs
    |
    |
 应用程序

---------------------------------------------------------------------
声卡
struct snd_card

snd_card可以说是整个ALSA音频驱动最顶层的一个结构，整个声卡的软件逻辑结构
开始于该结构，几乎所有与声音相关的逻辑设备都是在snd_card的管理之下，
声卡驱动的第一个动作通常就是创建一个snd_card结构体。

include/sound/core.h
struct snd_card {  
    int number;             /* number of soundcard (index to snd_cards) */
    char id[16];            /* id string of this card */  
    char mixername[80];     /* mixer name */  
    char components[128];   /* card components delimited with space */  
    struct module *module;  /* top-level module */  
    struct list_head devices;           /* devices */  
    struct list_head controls;          /* all controls for this card */  
    
    ...
};  

声卡的创建
int snd_card_create(index, id, THIS_MODULE, 0, &card);

index         int, 该声卡的编号
id            char, 声卡的标识符
参数四        int, 额外分配的私有数据的大小
card          struct snd_card, 返回所创建的snd_card实例的指针 

声卡的注册
int snd_card_register(struct snd_card *card)

-------------------------------------------------------------------
功能部件（逻辑设备）

每一种部件的创建最终会调用snd_device_new()来生成一个snd_device实例，
并把该实例链接到snd_card的devices链表中。

PCM     --    snd_pcm_new()
CONTROL --    snd_ctl_create()
TIMER   --    snd_timer_new()
RAWMIDI --    snd_rawmidi_new()
INFO    --    snd_card_proc_new()
JACK    --    snd_jack_new()

-------------------------------------------------------------------
pcm 设备

一个pcm实例由一个playback stream和一个capture stream组成，
这两个stream又分别有一个或多个substreams组成。

结构体
pcm_struct.png

  snd_pcm是挂在snd_card下面的一个snd_device
  snd_pcm中的字段：streams[2]，元素指向snd_pcm_str结构，
                               分别代表playback stream和capture stream
  snd_pcm_str中的substream字段，指向snd_pcm_substream结构
  snd_pcm_substream是pcm中间层的核心，绝大部分任务都是在substream中处理，
  尤其是他的ops（snd_pcm_ops）字段，许多user空间的应用程序通过alsa-lib对
  驱动程序的请求都是由该结构中的函数处理。它的runtime字段则
  指向snd_pcm_runtime结构，snd_pcm_runtime记录这substream的一些重要的软件
  和硬件运行环境和参数。


时序图：create_pcm_seq.png

1.snd_card_create
2.snd_pcm_new
3.建立playback stream，相应的substream也同时建立
4.建立capture stream，相应的substream也同时建立
5.调用snd_device_new()把该pcm挂到声卡中，参数ops中的dev_register字段
  指向了函数snd_pcm_dev_register，这个回调函数会在声卡的注册阶段被调用。
6.snd_pcm_set_ops     设置操作该pcm的控制/操作接口函数
7.snd_card_register   注册声卡，在这个阶段会遍历声卡下的所有逻辑设备，
  并且调用各设备的注册回调函数，对于pcm，就是snd_pcm_dev_register函数，
  该回调函数建立了和用户空间应用程序（alsa-lib）通信所用的设备文件节点:
  /dev/snd/pcmCxxDxxp和/dev/snd/pcmCxxDxxc

int snd_pcm_new(struct snd_card *card, const char *id, int device, 
    int playback_count, int capture_count, struct snd_pcm ** rpcm);

void snd_pcm_set_ops(struct snd_pcm *pcm, int direction, 
    struct snd_pcm_ops *ops);


设备文件节点的建立
struct snd_minor
每个snd_minor结构体保存了声卡下某个逻辑设备的上下文信息，他在逻辑设备建立
阶段被填充，在逻辑设备被使用时就可以从该结构体中得到相应的信息。

include/sound/core.h
struct snd_minor {  
    int type;           /* SNDRV_DEVICE_TYPE_XXX */  
    int card;           /* card number */  
    int device;         /* device number */  
    const struct file_operations *f_ops;    /* file operations */  
    void *private_data;     /* private data for f_ops->open */  
    struct device *dev;     /* device for sysfs */  
}; 

在声卡的注册阶段，会调用pcm的回调函数snd_pcm_dev_register()，这个函数里会
调用函数snd_register_device_for_dev()
  1.分配并初始化一个snd_minor结构中的各字段
  2.根据type，card和pcm的编号，确定数组的索引值minor，
    minor也作为pcm设备的c主设备号 
  3.把该snd_minor结构的地址放入全局数组snd_minors[minor]中
  4.最后，调用device_create创建设备节点

snd_pcm_f_ops是file_operations结构数组，在sound/core/pcm_native.c中：
const struct file_operations snd_pcm_f_ops[2] = {  
    {  
        .owner =        THIS_MODULE,  
        .write =        snd_pcm_write,  
        .aio_write =    snd_pcm_aio_write,  
        .open =         snd_pcm_playback_open,  
        .release =      snd_pcm_release,  
        .llseek =       no_llseek,  
        .poll =         snd_pcm_playback_poll,  
        .unlocked_ioctl =   snd_pcm_playback_ioctl,  
        .compat_ioctl =     snd_pcm_ioctl_compat,  
        .mmap =         snd_pcm_mmap,  
        .fasync =       snd_pcm_fasync,  
        .get_unmapped_area =    snd_pcm_get_unmapped_area,  
    },  
    {  
        .owner =        THIS_MODULE,  
        .read =         snd_pcm_read,  
        .aio_read =     snd_pcm_aio_read,  
        .open =         snd_pcm_capture_open,  
        .release =      snd_pcm_release,  
        .llseek =       no_llseek,  
        .poll =         snd_pcm_capture_poll,  
        .unlocked_ioctl =   snd_pcm_capture_ioctl,  
        .compat_ioctl =     snd_pcm_ioctl_compat,  
        .mmap =         snd_pcm_mmap,  
        .fasync =       snd_pcm_fasync,  
        .get_unmapped_area =    snd_pcm_get_unmapped_area,  
    }  
};  

应用程序如何最终调用到snd_pcm_f_ops结构中的回调函数:
app_to_pcm.png

open一个pcm设备时，将会调用snd_fops的open回调函数，跟入snd_open函数，
它首先从inode中取出此设备号，然后以次设备号为索引，从snd_minors全局数组
中取出当初注册pcm设备时填充的snd_minor结构，然后从snd_minor结构中取出
pcm设备的f_ops，并且把file->f_op替换为pcm设备的f_ops，紧接着直接调用pcm
设备的f_ops->open()，然后返回。因为file->f_op已经被替换，以后，应用程序的
所有read/write/ioctl调用都会进入pcm设备自己的回调函数中，也就是
snd_pcm_f_ops结构中定义的回调。

-------------------------------------------------------------------------
Control接口
主要让用户空间的应用程序（alsa-lib）可以访问和控制音频codec芯片中的多路开关
，滑动控件等。对于Mixer（混音）来说，Control接口显得尤为重要，从ALSA 0.9.x
版本开始，所有的mixer工作都是通过control接口的API来实现的。

<sound/control.h>定义了所有的Control API

