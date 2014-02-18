Input子系统

1. 数据结构
    input_dev   
    input_event 事件格式

struct input_dev {
	const char *name;//设备名
	const char *phys;
	const char *uniq;
	struct input_id id;

    //设备支持的事件类型，如EV_KEY,EV_REL 
	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];
	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];//如KEY_A, KEY_UP
	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];
	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];//具体事件，如ABS_X,ABS_Y
	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];
	unsigned long sndbit[BITS_TO_LONGS(SND_CNT)];
	unsigned long ffbit[BITS_TO_LONGS(FF_CNT)];
	unsigned long swbit[BITS_TO_LONGS(SW_CNT)];

	struct timer_list timer;
    ....
};

struct input_event {
	struct timeval time;//事件时间戳
	__u16 type;         //事件类型
	__u16 code;         //事件编码
	__s32 value;        //事件值
};


2. 函数接口
struct input_dev *input_allocate_device(void);
void input_free_device(struct input_dev *dev);

int input_register_device(struct input_dev *dev);
void input_unregister_device(struct input_dev *dev);

/* 时钟使能 */
struct clk *clk_get(struct device *dev, const char *id);
int clk_enable(struct clk *clk);
void clk_disable(struct clk *clk);
void clk_put(struct clk *clk);

/*  code, 事件编码; value, 事件对应值, 0 松开, 1 按下 */
void input_report_key(struct input_dev *dev, unsigned int code, int value);
void input_report_abs(struct input_dev *dev, unsigned int code, int value)
void input_report_rel(struct input_dev *dev, unsigned int code, int value)
void input_sync(struct input_dev *dev)

void input_set_abs_params(struct input_dev *dev, int axis, int min, int max, int fuzz, int flat);

----------------------------------------------------------------------------------
调试
    getevent sendevent 是android系统下的一个工具，可以模拟多种按键和触屏操作i
  产生的是raw event，raw event经过event hub处理产生最终的gesture事件。
  
    源码：system/core/toolbox/ sendevent.c getevent.c
  
  getevent
    查看所有/dev/input/event* 及对应驱动名字
    监控当前的事件(鼠标事件,按键事件,拖动滑动等)
    
    type 键码 最后一个根据type不同而不同

  sendevent

-----------------------------------------------------------------------------------------------------------
触摸平驱动
linux-2.6.28/drivers/input/touchscreen/s3c-ts.c
linux-2.6.28/arch/arm/plat-s3c/dev-ts.c
linux-2.6.28/arch/arm/mach-s3c6410/mach-smdk6410.c

驱动配置
 Device Drivers  ---> 
    Input device support  --->
        [*]   Touchscreens  --->
             < >   S3C touchscreen driver
                
tslib移植
# ./autogen-clean.sh
# ./autogen.sh 
# ./configure --host=arm-linux --prefix=/nfsroot
# make
    ts_test.c:(.text+0x1d4): undefined reference to `rpl_malloc'
修改config.h
    加入：#define malloc rpl_malloc
# make
# make install
# vim /etc/ts.conf
    打开module_raw input，去掉前边的空格，不然段错误

配置环境变量：
export TSLIB_TSDEVICE=/dev/input/event1
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_PLUGINDIR=/lib/ts
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export LD_LIBRARY_PATH=/lib

# ts_calibrate
    生成校准参数/etc/pointercal
