中断

/proc/interrupts 记录了中断详细信息.
/proc/stat       记录了一些系统活动的底层统计信息.
    intr 开头的行, 代表中断信息.

调用request_irq的正确位置应该时在设备第一次打开、硬件被告知产生中断之前. open()
调用free_irp的位置是最后一次关闭设备、硬件被告知不再用中断处理器之后.

-------------------------------------------------------------------------------------------

int request_irq(unsigned int irq, irq_handler_t handler,
		unsigned long irqflags, const char *devname, void *dev_id)
参数:
   irq, 中断编号, 如, S2按键IRQ_EINT(0), 看门狗IRQ_WDT

   handler, 中断处理函数, 类型为irqreturn_t (*handler)(int, void *);
                                    int, 中断编号
                                    void *, dev_id 共享的中断信号线
   irqflags, 中断标识        
    IRQF_TRIGGER_NONE	缺省的
    IRQF_TRIGGER_RISING, 上升沿触发
    IRQF_TRIGGER_FALLING, 下降沿触发
    IRQF_TRIGGER_HIGH, 高电平触发
    IRQF_TRIGGER_LOW, 低电平触发
    IRQF_SHARED, 共享中断(多个设备使用一个中断), dev_id不能为空
    IRQF_DISABLED, 关全部中断, 非irq对应中断
   
   devname, 设备名
   dev_id, 任意指针
返回值:
    成功返回0, 失败返回负数。

注意:
    1. 中断执行时, 本中断是关闭的
    2. 中断上下文不能睡眠(可能导致睡眠的函数不能调用)

void free_irq(unsigned int irq, void *dev_id)
参数:
   irq, 中断编号
   dev_id, 任意指针

/* 使能和屏蔽中断 */
void enable_irq(int irq);
void disable_irq(int irq);
void disable_irq_nosync(int irq); //等待目前的中断处理完成, 再屏蔽
/* 将屏蔽本 CPU 内的所有中断 */
void local_irq_enable(void);
void local_irq_disable(void);
void local_irq_save(unsigned long flags); //注意flags被直接传递, 而不是通过指针传递
void local_irq_restore(unsigned long flags);
void local_bh_enable(void);  //使能中断的底半部
void local_bh_disable(void); //禁止中断的底半部

---------------------------------------------------------------------------------------------------------------
顶半部和底半部:

顶半部完成尽可能少的比较紧急的功能. 顶半部不可被中断.
底半部完成中断事件的绝大多数任务, 而且可以被新的中断打断.

/*中断处理顶半部*/
irqreturn_t xxx_interrupt(int irq, void *dev_id)
{
    ...
    /*调度 tasklet 执行*/
    tasklet_schedule(&xxx_tasklet); // static inline void tasklet_schedule(struct tasklet_struct *t)
            或
    /*调度工作队列执行*/
    schedule_work(xxx_wq);   
    ...
}

底半部实现:
软中断 和 tasklet 仍然运行于中断上下文, 而工作队列则运行于进程上下文.

1.tasklet(小任务), 所有的tasklet代码必须是原子的. 基于软中断实现.
struct tasklet_struct
{
	struct tasklet_struct *next;
	unsigned long state;
	atomic_t count;
	void (*func)(unsigned long);
	unsigned long data;
};

定义tasklet:
    1）DECLARE_TASKLET(name, func, data);
    2）struct tasklet_struct *task;
       task = kmalloc
       void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long), unsigned long data);

 /*定义tasklet和底半部函数, 并关联*/
struct tasklet_struct xxx_tasklet;
void xxx_do_tasklet(unsigned long);
DECLARE_TASKLET(xxx_tasklet, xxx_do_tasklet, 0);
/*中断处理底半部*/
void xxx_do_tasklet(unsigned long)
{
    ...
}

2. workqueue(工作队列) , 更高的延迟, 但允许休眠

/*定义一个工作队列和处理函数, 并关联*/
struct work_struct *xxx_wq; 
void xxx_do_work(unsigned long); 
INIT_WORK(xxx_wq, (void (*)(void *)) xxx_do_work);
/*中断处理底半部*/
void xxx_do_work(unsigned long)
{
    ...
}

3. timer_list(内核定时器)

#include <linux/timer.h>
struct timer_list {
    ... 
    unsigned long expires;
    void (*function)(unsigned long);
    unsigned long data;
};

定义并初始化
/* function,超时调用的函数; expires, 将来某个时间,如, jiffies+HZ*N; data, 传给function的参数 */
1）DEFINE_TIMER(name, function, expires, data);

2）struct timer_list *timer;
   timer = kmalloc
   init_timer(timer);
   timer->function = 
   timer->expires = 
   timer->data = 

void init_timer(struct timer_list *timer);
void add_timer(struct timer_list *timer);/* 不能重复添加 */
int mod_timer(struct timer_list *timer, unsigned long expires);/* 只能修改没有超时的定时器 */
int del_timer(struct timer_list *timer);/* 只能修改没有超时的定时器 */

4.软中断 
struct softirq_action; //包含软中断处理函数指针和传递给该函数的参数。
open_softirq();
raise_softirq();


中断共享:


时间、延迟:
    时钟中断, 间隔HZ <linux/param.h> --> 计数器加1 (jiffies jiffies_64, 滴答数, 系统引导时初始化为0)
    64位计算机上, jiffies 和 jiffies_64 相同, 32位计算机上, jiffies 是 jiffies_64 的低32位.
    #include <linux/jiffies.h>
    unsigned long j, stamp_1, stamp_half, stamp_n;
    j = jiffies;                /*read the current value */
    stamp_1 = j + HZ;           /*1 second in the future */
    stamp_half = j + HZ/2;      /*half a second */
    stamp_n = j + n * HZ / 1000;/*n milliseconds */
    //jiffies溢出问题               
    #include <linux/jiffies.h> /* 比较两个时间的前后 */
    int time_after(unsigned long a, unsigned long b);
    int time_before(unsigned long a, unsigned long b);
    int time_after_eq(unsigned long a, unsigned long b);
    int time_before_eq(unsigned long a, unsigned long b);
    
    #include <linux/time.h> /*用户空间时间表述 timeval 和 timespec 结构 与 jiffies值的转换*/
    unsigned long timespec_to_jiffies(struct timespec *value);
    void jiffies_to_timespec(unsigned long jiffies, struct timespec *value);
    unsigned long timeval_to_jiffies(struct timeval *value);
    void jiffies_to_timeval(unsigned long jiffies, struct timeval *value);

    #include <linux/jiffies.h>
    u64 get_jiffies_64(void);


延迟相关函数:
ssleep/msleep
mdelay/udelay/ndelay



