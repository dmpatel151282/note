
在嵌入式SoC系统中集成的独立的外设控制器、挂接在SoC内存空间的外设等不依附于
PCI、USB、I2C、SPI等总线. 如, LCD、RTC、WDT等都被当作平台设备来处理(本身是字符型设备).

Linux 发明了一种虚拟的总线, 称为platform 总线
----------------------------------------------------------------------------------------------------
platform 总线下驱动的开发步骤是:
    1. 设备注册: 
        1）初始化 resource 结构体数组
        2）初始化 platform_device 结构变量
        3）向系统注册设备 platform_device_register()  module_init()
    2. 驱动注册: 
        1）初始化 platrorm_driver 结构变量
        2）向系统注册设备 platform_driver_register()  module_init()
        3）实现 xxx_probe() xxx_remove() 等函数

注意: 先设备注册再驱动注册. 

xxx_init() -> probe()

在内核初始化时:
do_basic_setup() -> driver_init() -> platform_bus_init() -> device_register()
                                                            bus_register()
设备注册的时候: 
platform_device_register() -> device_initialize()
                              platform_device_add()  
驱动注册时:
platform_driver_register() -> driver_register() -> bus_add_driver() -> driver_attach() -> bus_for_each_dev()
-> __driver_attach() -> driver_probe_device() -> drv->bus->match() -> platform_match() 
-> strncmp(pdev->name, drv->name, BUS_ID_SIZE), 如果相符就调用platform_drv_probe()->driver->probe(), 如果probe成功则绑定该设备到该驱动. 

-------------------------------------------------------------------------------------------------------------
1. 数据结构
    platform_device
    resource
    platform_driver

struct platform_device {
	const char	* name;        //平台设备名
	int		id;                //设备id
	struct device	dev;       //父类
	u32		num_resources;     //资源数, = ARRAY_SIZE(arr), struct resource arr[]; 
	struct resource	* resource;//资源指针, = arr
};

struct resource {
	resource_size_t start;//资源起始值
	resource_size_t end;  //资源结束值
	const char *name;     //资源名
	unsigned long flags;  //资源类型, 如IORESOURCE_MEM(内存),IORESOURCE_IRQ(中断)
    ....
};

struct platform_driver {
	int (*probe)(struct platform_device *); //探测函数
	int (*remove)(struct platform_device *);//删除函数
	struct device_driver driver;            //父类
    ....
};

2. 函数接口
#include <linux/platform_device.h>
int platform_device_register(struct platform_device *pdev);
int platform_driver_register(struct platform_driver *drv);
void platform_device_unregister(struct platform_device *pdev);
void platform_driver_unregister(struct platform_driver *drv);

/* 获取资源; type, 资源类型; num, 某种类型中的资源编号 */
struct resource *platform_get_resource(struct platform_device *dev, unsigned int type, unsigned int num);
struct resource *platform_get_resource_byname(struct platform_device *dev, unsigned int type, char *name);

int platform_get_irq(struct platform_device *dev, unsigned int num);

/* 宏, pdev->dev->driver_data = data */
void platform_set_drvdata(struct platform_device *pdev, void *data);


ltp-pan.c   实现ltp-pan的主文件, 含main()

zoolib.c    (zoo文件在testcases/bin目录下)
    A Zoo is a file used to record what test tags are running at the moment.
If the system crashes, we should be able to look at the zoo file to find out
what was currently running. This is especially helpful when running multiple
tests at the same time.

zoo file format:
  80 characters per line, ending with a \n
  available lines start with '#'
  expected line fromat: pid_t,tag,cmdline
  
splitstr.c 分割字符串
const char **splitstr(const char *str, const char *separator, int *argcount)
str: 要分割的字符串
separator: 以此为分割条件
argcount: 分割后的字符串个数
成功返回，分割后的字符串数组；失败返回NULL
void splitstr_free(const char **p_return)

-------------------------------------------------------------------------
ltp-pan可执行文件的参数：
    -q   quiet_mode, =>quiet_mode=1 
    -e   track_exit_stats, exit non-zero if any test exists non-zero
    -S   run tests sequentially, =>sequential=1  
    -p   formatted printing,  =>fmt_printf=1
    -O   [test_out_dir]     output buffering directory   
    -t   [run_time]     s|m|h|d 结尾, =>timed=1
    -a   [zooname]  name of the zoo file to use
    -n   [panname]  tag given to pan
    -f   [filename] filename to read test tags from
    -l   [logfilename]
    -o   [outputfile]
    -C   [failcmdfilename] the file where all failed commands will be
    -A   
    -d   [Debug] debug-level, 默认为0, 0xf00 0xf0 0xf 0xff 0xfff ...
    -h
    -r   [reporttype] reporting type: none, rts
    -s   [starts] number of tags to run, 默认-1
    -x   [keep_active] number of tags to keep running, 默认1
-------------------------------------------------------------------------
ltp-pan 分析

/* One entry in the command line collection.  */
struct coll_entry {
    char *name;     /* tag name */
    char *cmdline;      /* command line */
    char *pcnt_f;       /* location of %f in the command line args, flag */
    struct coll_entry *next;
};

struct collection {
    int cnt;
    struct coll_entry **ary;
};

//正在运行的tag进程组
struct tag_pgrp {
    int pgrp;         
    int stopping;
    time_t mystime;
    struct coll_entry *cmd;
    char output[PATH_MAX];
};

//孤儿进程组
struct orphan_pgrp {
    int pgrp;
    struct orphan_pgrp *next;
};

---------------------------------------------------------------------------
main函数中，
    1.处理参数, getopt()
       向logfile中写入：Test Start Time:  等
    2.get_collection函数
        get_collection(filename, optind, argc, argv);
        2.1 buf = slurp(file);
          2.1.1 fopen() 打开文件(alltests)
          2.1.2 fstat()取得文件状态 
          2.1.3 分配相应大小的buf，read() 将文件内容读取到该buf上
        2.2 分配 struct collection, 将alltests文件中的每一行分别存在
            struct coll_entry结构体中
        2.3 最终返回 struct collection* 
    3. 分配并置0, 结构体数组running, struct tag_pgrp，个数keep_active 
    4. 分配并置0, 结构体 struct orphan_pgrp
    5. 产生一个48位种子随机数, arand48()
    6. 处理参数-t和-s, 给starts赋值, starts = coll->cnt
    7. 处理-O 且没有-x 
    8. 处理-o参数，freopen()
    9. 处理-C参数，fopen()
    10. 处理-a参数, zoo_open()
                    zoo_mark_args()
    11. Allocate N spaces for max-arg commands.
          对每个[keep_active]执行 zoo_mark_cmdline()
          对每个[keep_active]执行 zoo_clear()
    12. sigemptyset(&sa.sa_mask); 绑定信号处理函数wait_handler, sigaction()
        SIGALRM SIGINT SIGTERM SIGHUP SIGUSR1 SIGUSR2
    13. while(1), 
      13.1 while, 对每个[keep_active] run_child()
      13.2 如果之前接收过 SIGUSR1 信号，...
           如果接收过 SIGUSR2 信号，propagate_signal()
      13.3 check_pids()
    14. while(1), 等待孤儿进程组
    15. zoo_clear(zoofile, getpid())
    16. 打印测试结果统计到[logfile]

--------------------------------------------------------------------------
13.1 run_child(coll->ary[c],running + i,quiet_mode,&failcnt,fmt_print,logfile)
1.初始化 struct tag_pgrp；创建管道，pipe(errpipe)
2.write_test_start()
3.创建子进程，fork()
子进程中：
 1.设置文件描述符标记，fcntl(errpipe[1], F_SETFD, 1);
 2.setpgrp() <==> setpgid(0, 0)
 3.若无-O，dup2(fileno(stdout), fileno(stderr))
 4.如果cmdline含"\"';|<>$\\"，在shell下执行cmd
    execlp("sh", "sh", "-c", c_cmdline, (char *)0);
   否则直接执行，execvp(arg_v[0], arg_v);
   分别将错误信息写入errbuf字符串中
 5.将errbuf字符串的长度和内容写入管道errpipe[1]
父进程中：
 1.从errpipe[0]中读取, read(errpipe[0], &errlen, sizeof(errlen))
 2.read(errpipe[0], errbuf, errlen); close(errpipe[0]);
 3.等待子进程的状态，waitpid(cpid, &status, 0);
 4.将结果写入logfile中
 5.write_test_end()
 6.zoo_mark_cmdline()
-------------------------------------------------------------------
static void propagate_signal(struct tag_pgrp *running, int keep_active,
         struct orphan_pgrp *orphans)
1. 对于每个[keep_active], 向tag_pgrp中的进程组发送信号, 并置stopping为1
  kill(-running[i].pgrp, send_signal)
2. check_orphans(orphans, send_signal);
 2.1 跳过进程组id pgrp为0的项
 2.2 kill(-(orph->pgrp), sig) 向孤儿进程组发送send_signal信号
3. rec_signal = send_signal = 0;
-------------------------------------------------------------------
static int check_pids(struct tag_pgrp *running, int *num_active, 
    int keep_active, FILE * logfile, FILE * failcmdfile, 
    struct orphan_pgrp *orphans,int fmt_print, int *failcnt, int quiet_mode)

1. check_orphans(orphans, send_signal);
 1.1 跳过进程组id pgrp为0的项
 1.2 kill(-(orph->pgrp), sig) 向孤儿进程组发送send_signal信号

2. 

-------------------------------------------------------------------
void wait_handler(int sig)
{
    static int lastsent = 0;
    if (sig == 0) {
        lastsent = 0;
    } else {
        rec_signal = sig;
        if (sig == SIGUSR2)
            return;
        if (lastsent == 0)
            send_signal = sig;
        else if (lastsent == SIGUSR1)
            send_signal = SIGINT;
        else if (lastsent == sig)
            send_signal = SIGTERM;
        else if (lastsent == SIGTERM)
            send_signal = SIGHUP;
        else if (lastsent == SIGHUP)
            send_signal = SIGKILL;
        lastsent = send_signal;
    }
}



