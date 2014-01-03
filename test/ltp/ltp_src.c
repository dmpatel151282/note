In the Package
--------------
testcases/
    Contains all tests that run under the LTP as well as the "bin"
    directory, which has hardlinks to all the test executables.

runtest/
    It houses command lists that are used by ltp-pan for automated testing.

pan/
    The pan directory contains a simple, lightweight test harness.  lpt-pan
    has the ability to run tests randomly and in parallel.

--------------------------------------------------------------------------
源安装包目录列表:

 doc: 说明文件和帮助文档,这个目录中对LTP的内容和每个工具都有详细的说明
 testscripts: 可执行的测试脚本，不同方面的测试脚本的集合
 testcases: 所有LTP测试套件中所使用的测试用例的源码
 runtest: 是要执行的测试用例的命令集合，每个文件针对测试的不同方面
          （用于链接testscripts内的测试脚本和testcases测试项目）
 include: LTP测试套件的头文件目录,定义了LTP自身的数据结构和函数结构
 lib: LTP测试套件运行时自身需要的库文件,定义了LTP自身的各种函数
 bin: 存放LTP测试的一些辅助脚本
 results: 测试结果默认存储目录
 output: 测试日志默认存储目录
 share: 脚本使用说明目录
 pan: 该目录存储的是LTP测试套件的测试驱动程序pan

文件列表:
 IDcheck.sh   ：检查系统是否缺少执行LTP测试套件所需的用户和用户组，
                如果缺少则为LTP测试套件创建所需的用户和用户组。
 runltplite.sh：用来测试LTP安装，也可用来对测试套件的子项目进行测试。
 ver_linux    ：获取硬件、软件、环境信息。

------------------------------------------------------------------------
LTP工作原理
  LTP测试套件通过执行测试脚本runalltests.sh(或runltp或runltplite.sh)或
/testscripts内的测试脚本调用驱动程序pan执行/testcases内的测试项目.

pan工作原理
  LTP测试套件有一个专门的测试驱动程序pan，具体的测试用例的执行都是由pan来
  调用执行，它可以跟踪孤儿进程和抓取测试的输出信息。

  工作方式：
    从一个测试命令文件中读取要测试的条目和要执行的命令行，然后等待该项测试
    的结束，并记录详细的测试输出。默认状态下pan会随机选择一个命令行来运行
    ，可以指定在同一时间要执行测试的次数(-x)。

  pan会记录测试产生的详细的格式复杂的输出，但它不进行数据的整理和统计，数据
  整理统计的工作由scanner来完成，scanner是一个测试结果分析工具，它会理解pan
  的输出格式，并输出成一个表格的形式来总结那些测试passed或failed.
--------------------------------------------------------------------------
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
run_child(coll->ary[c],running + i,quiet_mode,&failcnt,fmt_print,logfile)
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
