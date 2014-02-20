
//查看文件系统分区及使用情况
df -h  
//查看当前目录大小
du -sh 
//查看前10个占用空间最大的文件或目录
du -sh * | sort -nr | head

-----------------------------------------------------------------------------------------
安装和登录命令：login、shutdown、halt、reboot、install、mount、umount、chsh、exit、last；

shutdown

1.作用
shutdown命令的作用是关闭计算机，它的使用权限是超级用户。

2.格式
shutdown [-h][-i][-k][-m][-t]

3.重要参数
-t：在改变到其它运行级别之前，告诉init程序多久以后关机。
-k：并不真正关机，只是送警告信号给每位登录者。
-h：关机后关闭电源。
-c：cancel current process取消目前正在执行的关机程序。所以这个选项当然没有时间参数，
    但是可以输入一个用来解释的讯息，而这信息将会送到每位使用者。
-F：在重启计算机时强迫fsck。
-time：设定关机前的时间。
-m: 将系统改为单用户模式。
-i：关机时显示系统信息。

4.命令说明
  shutdown命令可以安全地将系统关机。有些用户会使用直接断掉电源的方式来关闭Linux系统，这是十分危险的。
因为Linux与Windows不同，其后台运行着许多进程，所以强制关机可能会导致进程的数据丢失，使系统处于不稳定的状态，
甚至在有的系统中会损坏硬件设备（硬盘）。在系统关机前使用 shutdown命令，系统管理员会通知所有登录的用户系统将要关闭，
并且login指令会被冻结，即新的用户不能再登录。

halt

1.作用
halt命令的作用是关闭系统，它的使用权限是超级用户。

2.格式
halt [-n] [-w] [-d] [-f] [-i] [-p]

3.主要参数说明
-n：防止sync系统调用，它用在用fsck修补根分区之后，以阻止内核用老版本的超级块覆盖修补过的超级块。
-w：并不是真正的重启或关机,只是写wtmp（/var/log/wtmp）纪录。
-f：没有调用shutdown，而强制关机或重启。
-i：关机（或重启）前，关掉所有的网络接口。
-f：强迫关机，不呼叫shutdown这个指令。
-p: 当关机的时候顺便做关闭电源的动作。
-d：关闭系统，但不留下纪录。　

4.命令说明
  halt就是调用shutdown -h。halt执行时，杀死应用进程，执行sync(将存于buffer中的资料强制写入硬盘中)系统调用，
文件系统写操作完成后就会停止内核。若系统的运行级别为0或6，则关闭系统；否则以shutdown指令（加上-h参数）来取代。

reboot

1.作用
reboot命令的作用是重新启动计算机，它的使用权限是系统管理者。

2.格式
reboot [-n] [-w] [-d] [-f] [-i]

3.主要参数
-n: 在重开机前不做将记忆体资料写回硬盘的动作。
-w: 并不会真的重开机，只是把记录写到/var/log/wtmp文件里。

-----------------------------------------------------------------------------------------
文件处理命令：file、mkdir、grep、dd、find、mv、ls、diff、cat、ln；

file

1.作用 
判断文件类型，使用权限是所有用户。

2.格式
file [options] 文件名

3.[options]主要参数

-v：在标准输出后显示版本信息，并且退出。
-z：探测压缩过的文件类型。
-L：允许符合连接。
-f name：从文件namefile中读取要分析的文件名列表。

4.简单说明
  使用file命令可以知道某个文件究竟是二进制（ELF格式）的可执行文件, 还是Shell Script文件，或者是其它的什么格式。
file能识别的文件类型有目录、Shell脚本、英文文本、二进制可执行文件、C语言源文件、文本文件、DOS的可执行文件。

5.应用实例
  如果我们看到一个没有后缀的文件grap，可以使用下面命令：

$ file grap

grap： English text

此时系统显示这是一个英文文本文件。需要说明的是，file命令不能探测包括图形、音频、视频等多媒体文件类型。


mkdir

1.作用
mkdir命令的作用是建立名称为dirname的子目录，与MS DOS下的md命令类似，它的使用权限是所有用户。

2.格式
mkdir [options] 目录名

3.[options]主要参数

-m, --mode=模式：设定权限<模式>，与chmod类似。
-p, --parents：需要时创建上层目录；如果目录早已存在，则不当作错误。
-v, --verbose：每次创建新目录都显示信息。
--version：显示版本信息后离开。


grep

1.作用
  grep命令可以指定文件中搜索特定的内容，并将含有这些内容的行标准输出。
grep全称是Global Regular Expression Print，表示全局正则表达式版本，它的使用权限是所有用户。

2.格式
grep [options]

3.[options]主要参数：
-I：不区分大小写（只适用于单字符）。
-r：递归
-n：显示匹配行及行号。
-s：不显示不存在或无匹配文本的错误信息。
-c：只输出匹配行的计数。
-h：查询多文件时不显示文件名。
-l：查询多文件时只输出包含匹配字符的文件名。
-v：显示不包含匹配文本的所有行。

pattern正则表达式主要参数：
    \：忽略正则表达式中特殊字符的原有含义。
    ^：匹配正则表达式的开始行。
    $：匹配正则表达式的结束行。
    \<：从匹配正则表达式的行开始。
    \>：到匹配正则表达式的行结束。
    [ ]：单个字符，如[A]即A符合要求 。
    [ - ]：范围，如[A-Z]，即A、B、C一直到Z都符合要求 。
    .：所有的单个字符。
    * ：有字符，长度可以为0。


4.应用实例
  查询DNS服务是日常工作之一，这意味着要维护覆盖不同网络的大量IP地址。有时IP地址会超过2000个。
如果要查看nnn.nnn网络地址，但是却忘了第二部分中的其余部分，只知到有两个句点，例如nnn nn..。
要抽取其中所有nnn.nnn IP地址，使用[0-9 ]\{3 \}\.[0-0\{3\}\。含义是任意数字出现3次，
后跟句点，接着是任意数字出现3次，后跟句点。

$grep '[0-9 ]\{3 \}\.[0-0\{3\}\' ipfile

补充说明，grep家族还包括fgrep和egrep。fgrep是fix grep，允许查找字符串而不是一个模式；
egrep是扩展grep，支持基本及扩展的正则表达式，但不支持\q模式范围的应用及与之相对应的一些更加规范的模式。


dd

1.作用
dd命令用来复制文件，并根据参数将数据转换和格式化。

2.格式
dd [options]

3.[opitions]主要参数
bs=字节：强迫 ibs=<字节>及obs=<字节>。
cbs=字节：每次转换指定的<字节>。
conv=关键字：根据以逗号分隔的关键字表示的方式来转换文件。
count=块数目：只复制指定<块数目>的输入数据。
ibs=字节：每次读取指定的<字节>。
if=文件：读取<文件>内容，而非标准输入的数据。
obs=字节：每次写入指定的<字节>。
of=文件：将数据写入<文件>，而不在标准输出显示。
seek=块数目：先略过以obs为单位的指定<块数目>的输出数据。
skip=块数目：先略过以ibs为单位的指定<块数目>的输入数据。

4.应用实例
dd命令常常用来制作Linux启动盘。先找一个可引导内核，令它的根设备指向正确的根分区，
然后使用dd命令将其写入软盘：
    $ rdev vmlinuz /dev/hda
    $dd if＝vmlinuz of＝/dev/fd0

上面代码说明，使用rdev命令将可引导内核vmlinuz中的根设备指向/dev/hda，请把“hda”换成自己的根分区，
接下来用dd命令将该内核写入软盘。

-----------------------------------------------------------------------------------------
系统管理相关命令：df、top、free、quota、at、lp、adduser、groupadd、kill、crontab；

df

1.作用
df命令用来检查文件系统的磁盘空间占用情况，使用权限是所有用户。

2.格式
df [options]

3.主要参数
-s：对每个Names参数只给出占用的数据块总数。
-a：递归地显示指定目录中各文件及子目录中各文件占用的数据块数。若既不指定-s，也不指定-a，
    则只显示Names中的每一个目录及其中的各子目录所占的磁盘块数。
-k：以1024字节为单位列出磁盘空间使用情况。
-x：跳过在不同文件系统上的目录不予统计。
-l：计算所有的文件大小，对硬链接文件则计算多次。
-i：显示inode信息而非块使用量。
-h：以容易理解的格式印出文件系统大小，例如136KB、254MB、21GB。
-P：使用POSIX输出格式。
-T：显示文件系统类型。

4.说明
df命令被广泛地用来生成文件系统的使用统计数据，它能显示系统中所有的文件系统的信息，
包括总容量、可用的空闲空间、目前的安装点等。

  超级权限用户使用df命令时会发现这样的情况：某个分区的容量超过了100％。这是因为Linux系统
为超级用户保留了10％的空间，由其单独支配。也就是说，对于超级用户而言，他所见到的硬盘容量将是110％。
这样的安排对于系统管理而言是有好处的，当硬盘被使用的容量接近100％时系统管理员还可以正常工作。

5.应用实例
Linux支持的文件系统非常多，包括JFS、ReiserFS、ext、ext2、ext3、ISO9660、XFS、Minx、vfat、MSDOS等。
使用df -T命令查看磁盘空间时还可以得到文件系统的信息：

＃df -T
文件系统 类型 容量 已用 可用 已用% 挂载点
/dev/hda7 reiserfs 5.2G 1.6G 3.7G 30% /
/dev/hda1 vfat 2.4G 1.6G 827M 66% /windows/C
/dev/hda5 vfat 3.0G 1.7G 1.3G 57% /windows/D
/dev/hda9 vfat 3.0G 2.4G 566M 82% /windows/E
/dev/hda10 NTFS 3.2G 573M 2.6G 18% /windows/F
/dev/hda11 vfat 1.6G 1.5G 23M 99% /windows/G

从上面除了可以看到磁盘空间的容量、使用情况外，分区的文件系统类型、挂载点等信息也一览无遗。


top

1.作用
top命令用来显示执行中的程序进程，使用权限是所有用户。

2.格式
top [-] [d delay] [q] [c] [S] [s] [i] [n]

3.主要参数
d：指定更新的间隔，以秒计算。
q：没有任何延迟的更新。如果使用者有超级用户，则top命令将会以最高的优先序执行。
c：显示进程完整的路径与名称。
S：累积模式，会将己完成或消失的子行程的CPU时间累积起来。
s：安全模式。
i：不显示任何闲置(Idle)或无用(Zombie)的行程。
n：显示更新的次数，完成后将会退出top。

4.说明
top命令是Linux系统管理的一个主要命令，通过它可以获得许多信息。

第一行表示的项目依次为当前时间、系统启动时间、当前系统登录用户数目、平均负载。
第二行显示的是所有启动的进程、目前运行的、挂起 (Sleeping)的和无用(Zombie)的进程。
第三行显示的是目前CPU的使用情况，包括系统占用的比例、用户使用比例、闲置(Idle)比例。
第四行显示物理内存的使用情况，包括总的可以使用的内存、已用内存、空闲内存、缓冲区占用的内存。
第五行显示交换分区使用情况，包括总的交换分区、使用的、空闲的和用于高速缓存的大小。
第六行显示的项目最多，下面列出了详细解释。
  PID（Process ID）：进程标示号。
  USER：进程所有者的用户名。
  PR：进程的优先级别。
  NI：进程的优先级别数值。
  VIRT：进程占用的虚拟内存值。
  RES：进程占用的物理内存值。
  SHR：进程使用的共享内存值。
  S：进程的状态，其中S表示休眠，R表示正在运行，Z表示僵死状态，N表示该进程优先值是负数。
  %CPU：该进程占用的CPU使用率。
  %MEM：该进程占用的物理内存和总内存的百分比。
  TIME＋：该进程启动后占用的总的CPU时间。
  Command：进程启动的启动命令名称，如果这一行显示不下，进程会有一个完整的命令行。

top命令使用过程中，还可以使用一些交互的命令来完成其它参数的功能。这些命令是通过快捷键启动的。
  <空格>：立刻刷新。
  P：根据CPU使用大小进行排序。
  T：根据时间、累计时间排序。
  q：退出top命令。
  m：切换显示内存信息。
  t：切换显示进程和CPU状态信息。
  c：切换显示命令名称和完整命令行。
  M：根据使用内存大小进行排序。
  W：将当前设置写入~/.toprc文件中。这是写top配置文件的推荐方法。

可以看到，top命令是一个功能十分强大的监控系统的工具，对于系统管理员而言尤其重要。
但是，它的缺点是会消耗很多系统资源。

5.应用实例
  使用top命令可以监视指定用户，缺省情况是监视所有用户的进程。如果想查看指定用户的情况，
在终端中按“U”键，然后输入用户名，系统就会切换为指定用户的进程运行界面


free

1.作用
free命令用来显示内存的使用情况，使用权限是所有用户。

2.格式
free [-b|-k|-m] [-o] [-s delay] [-t] [-V]

3.主要参数
-b -k -m：分别以字节（KB、MB）为单位显示内存使用情况。
-s delay：显示每隔多少秒数来显示一次内存使用情况。
-t：显示内存总和列。
-o：不显示缓冲区调节列。

4.应用实例
  free命令是用来查看内存使用情况的主要命令。和top命令相比，它的优点是使用简单，
并且只占用很少的系统资源。通过-s参数可以使用free命令不间断地监视有多少内存在使用，
这样可以把它当作一个方便实时监控器。

＃free -b -s5

使用这个命令后终端会连续不断地报告内存使用情况（以字节为单位），每5秒更新一次。
-----------------------------------------------------------------------------------------
网络操作命令：ifconfig、ip、ping、netstat、telnet、ftp、route、rlogin、rcp、finger、mail、 nslookup；

ifconfig
1.作用
ifconfig用于查看和更改网络接口的地址和参数，包括IP地址、网络掩码、广播地址，使用权限是超级用户。

2.格式
ifconfig -interface [options] address

3.主要参数
-interface：指定的网络接口名，如eth0和eth1。
up：激活指定的网络接口卡。
down：关闭指定的网络接口。
broadcast address：设置接口的广播地址。
pointopoint：启用点对点方式。
address：设置指定接口设备的IP地址。
netmask address：设置接口的子网掩码。

4.应用说明
ifconfig是用来设置和配置网卡的命令行工具。为了手工配置网络，这是一个必须掌握的命令。
使用该命令的好处是无须重新启动机器。要赋给eth0接口IP地址207.164.186.2，并且马上激活它，
使用下面命令：
  #ifconfig eth0 210.34.6.89 netmask 255.255.255.128 broadcast 210.34.6.127
  该命令的作用是设置网卡eth0的IP地址、网络掩码和网络的本地广播地址。若运行不带任何参数的ifconfig命令，
这个命令将显示机器所有激活接口的信息。带有-a参数的命令则显示所有接口的信息，包括没有激活的接口。
注意，用ifconfig命令配置的网络设备参数，机器重新启动以后将会丢失。

如果要暂停某个网络接口的工作，可以使用down参数：
#ifconfig eth0 down

ip
1.作用
ip是iproute2软件包里面的一个强大的网络配置工具，它能够替代一些传统的网络管理工具，例如ifconfig、route等，
使用权限为超级用户。几乎所有的Linux发行版本都支持该命令。

2.格式
ip [OPTIONS] OBJECT [COMMAND [ARGUMENTS]]

3.主要参数
OPTIONS是修改ip行为或改变其输出的选项。所有的选项都是以-字符开头，分为长、短两种形式。目前，ip支持如表1所示选项。

OBJECT是要管理者获取信息的对象。目前ip认识的对象见表2所示。

表1 ip支持的选项

-V,-Version 打印ip的版本并退出。
-s,-stats,-statistics 输出更为详尽的信息。如果这个选项出现两次或多次，则输出的信息将更为详尽。
-f,-family 这个选项后面接协议种类，包括inet、inet6或link，强调使用的协议种类。如果没有足够的信息告诉ip使用的协议种类，ip就会使用默认值inet或any。link比较特殊，它表示不涉及任何网络协议。
-4 是-family inet的简写。
-6 是-family inet6的简写。
-0 是-family link的简写。
-o,-oneline 对每行记录都使用单行输出，回行用字符代替。如果需要使用wc、grep等工具处理ip的输出，则会用到这个选项。
-r,-resolve 查询域名解析系统，用获得的主机名代替主机IP地址

COMMAND 设置针对指定对象执行的操作，它和对象的类型有关。一般情况下，ip支持对象的增加(add)、删除(delete)和展示(show或list)。有些对象不支持这些操作，或者有其它的一些命令。对于所有的对象，用户可以使用help命令获得帮助。这个命令会列出这个对象支持的命令和参数的语法。如果没有指定对象的操作命令，ip会使用默认的命令。一般情况下，默认命令是list，如果对象不能列出，就会执行help命令。

ARGUMENTS 是命令的一些参数，它们倚赖于对象和命令。ip支持两种类型的参数：flag和parameter。flag由一个关键词组成；parameter由一个关键词加一个数值组成。为了方便，每个命令都有一个可以忽略的默认参数。例如，参数dev是ip link命令的默认参数，因此ip link ls eth0等于ip link ls dev eth0。我们将在后面的详细介绍每个命令的使用，命令的默认参数将使用default标出。

4.应用实例
添加IP地址192.168.2.2/24到eth0网卡上：
#ip addr add 192.168.1.1/24 dev eth0

丢弃源地址属于192.168.2.0/24网络的所有数据报：
#ip rule add from 192.168.2.0/24 prio 32777 reject


ping
1.作用
ping检测主机网络接口状态，使用权限是所有用户。

2.格式
ping [-dfnqrRv][-c][-i][-I][-l][-p][-s][-t] IP地址

3.主要参数
-d：使用Socket的SO_DEBUG功能。
-c：设置完成要求回应的次数。
-f：极限检测。
-i：指定收发信息的间隔秒数。
-I：网络界面使用指定的网络界面送出数据包。
-l：前置载入，设置在送出要求信息之前，先行发出的数据包。
-n：只输出数值。
-p：设置填满数据包的范本样式。
-q：不显示指令执行过程，开头和结尾的相关信息除外。
-r：忽略普通的Routing Table，直接将数据包送到远端主机上。
-R：记录路由过程。
-s：设置数据包的大小。
-t：设置存活数值TTL的大小。
-v：详细显示指令的执行过程。
 
ping 命令是使用最多的网络指令，通常我们使用它检测网络是否连通，它使用ICMP协议。
但是有时会有这样的情况，我们可以浏览器查看一个网页，但是却无法 ping通，这是因为
一些网站处于安全考虑安装了防火墙。另外，也可以在自己计算机上试一试，通过下面的
方法使系统对ping没有反应：
  # echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all


netstat
1.作用
检查整个Linux网络状态。

2.格式
netstat [-acCeFghilMnNoprstuvVwx][-A][--ip]

3.主要参数
-a--all：显示所有连线中的Socket。
-A：列出该网络类型连线中的IP相关地址和网络类型。
-c--continuous：持续列出网络状态。
-C--cache：显示路由器配置的快取信息。
-e--extend：显示网络其它相关信息。

-----------------------------------------------------------------------------------------
系统安全相关命令：passwd、su、umask、chgrp、chmod、chown、chattr、sudo ps、who；
passwd

1.作用
passwd命令原来修改账户的登陆密码，使用权限是所有用户。

2.格式
passwd [选项] 账户名称

3.主要参数
-l：锁定已经命名的账户名称，只有具备超级用户权限的使用者方可使用。
-u：解开账户锁定状态，只有具备超级用户权限的使用者方可使用。
-x, --maximum=DAYS：最大密码使用时间（天），只有具备超级用户权限的使用者方可使用。
-n, --minimum=DAYS：最小密码使用时间（天），只有具备超级用户权限的使用者方可使用。
-d：删除使用者的密码, 只有具备超级用户权限的使用者方可使用。
-S：检查指定使用者的密码认证种类, 只有具备超级用户权限的使用者方可使用。

4.应用实例

$ passwd
Changing password for user cao.
Changing password for cao
(current) UNIX password:
New UNIX password:
Retype new UNIX password:
passwd: all authentication tokens updated successfully.

从上面可以看到，使用passwd命令需要输入旧的密码，然后再输入两次新密码。

su

1.作用
su的作用是变更为其它使用者的身份，超级用户除外，需要键入该使用者的密码。

2.格式
su [选项]... [-] [USER [ARG]...]

3.主要参数
-f ， --fast：不必读启动文件（如 csh.cshrc 等），仅用于csh或tcsh两种Shell。
-l ， --login：加了这个参数之后，就好像是重新登陆为该使用者一样，大部分环境变量（例如HOME、SHELL和USER等）都是以该使用者（USER）为主，并且工作目录也会改变。如果没有指定USER，缺省情况是root。
-m， -p ，--preserve-environment：执行su时不改变环境变数。
-c command：变更账号为USER的使用者，并执行指令（command）后再变回原来使用者。
USER：欲变更的使用者账号，ARG传入新的Shell参数。

4.应用实例
变更账号为超级用户，并在执行df命令后还原使用者。 su -c df root

umask

1.作用
umask设置用户文件和目录的文件创建缺省屏蔽值，若将此命令放入profile文件，就可控制该用户后续所建文件的存取许可。它告诉系统在创建文件时不给谁存取许可。使用权限是所有用户。

2.格式
umask [-p] [-S] [mode]

3.参数
－S：确定当前的umask设置。
－p：修改umask 设置。
[mode]：修改数值。

4.说明
传统Unix的umask值是022，这样就可以防止同属于该组的其它用户及别的组的用户修改该用户的文件。既然每个用户都拥有并属于一个自己的私有组，那么这种“组保护模式”就不在需要了。严密的权限设定构成了Linux安全的基础，在权限上犯错误是致命的。需要注意的是，umask命令用来设置进程所创建的文件的读写权限，最保险的值是0077，即关闭创建文件的进程以外的所有进程的读写权限，表示为-rw-------。在～/.bash_profile中，加上一行命令umask 0077可以保证每次启动Shell后, 进程的umask权限都可以被正确设定。

5.应用实例

umask -S
u=rwx,g=rx,o=rx
umask -p 177
umask -S
u=rw,g=,o=

上述5行命令，首先显示当前状态，然后把umask值改为177，结果只有文件所有者具有读写文件的权限，其它用户不能访问该文件。这显然是一种非常安全的设置。

chgrp

1.作用
chgrp表示修改一个或多个文件或目录所属的组。使用权限是超级用户。

2.格式
chgrp [选项]... 组 文件...
或
chgrp [选项]... --reference=参考文件 文件...

将每个<文件>的所属组设定为<组>。

3.参数
-c, --changes ：像 --verbose，但只在有更改时才显示结果。
--dereference：会影响符号链接所指示的对象，而非符号链接本身。
-h, --no-dereference：会影响符号链接本身，而非符号链接所指示的目的地(当系统支持更改符号链接的所有者，此选项才有效)。
-f, --silent, --quiet：去除大部分的错误信息。
--reference=参考文件：使用<参考文件>的所属组，而非指定的<组>。
-R, --recursive：递归处理所有的文件及子目录。
-v, --verbose：处理任何文件都会显示信息。

4.应用说明
该命令改变指定指定文件所属的用户组。其中group可以是用户组ID，也可以是/etc/group文件中用户组的组名。文件名是以空格分开的要改变属组的文件列表，支持通配符。如果用户不是该文件的属主或超级用户，则不能改变该文件的组。

5.应用实例
改变/opt/local /book/及其子目录下的所有文件的属组为book，命令如下：
$ chgrp - R book /opt/local /book

chmod

1.作用
chmod命令是非常重要的，用于改变文件或目录的访问权限，用户可以用它控制文件或目录的访问权限，使用权限是超级用户。

2.格式
chmod命令有两种用法。一种是包含字母和操作符表达式的字符设定法（相对权限设定）；另一种是包含数字的数字设定法（绝对权限设定）。

（1）字符设定法
chmod [who] [+ | - | =] [mode] 文件名

◆操作对象who可以是下述字母中的任一个或它们的组合
u：表示用户，即文件或目录的所有者。
g：表示同组用户，即与文件属主有相同组ID的所有用户。
o：表示其它用户。
a：表示所有用户，它是系统默认值。

◆操作符号
+：添加某个权限。
-：取消某个权限。
=：赋予给定权限，并取消其它所有权限（如果有的话）。

◆设置mode的权限可用下述字母的任意组合
r：可读。
w：可写。
x：可执行。

-----------------------------------------------------------------------------------------
其它命令：tar、unzip、gunzip、unarj、mtools、man、unendcode、uudecode

tar

1.作用
tar命令是Unix/Linux系统中备份文件的可靠方法，几乎可以工作于任何环境中，它的使用权限是所有用户。

2.格式
tar [主选项+辅选项] 文件或目录

3.主要参数
使用该命令时，主选项是必须要有的，它告诉tar要做什么事情，辅选项是辅助使用的，可以选用。

主选项：
-c 创建新的档案文件。如果用户想备份一个目录或是一些文件，就要选择这个选项。

-r 把要存档的文件追加到档案文件的未尾。例如用户已经做好备份文件，又发现还有一个目录或是一些文件忘记备份了，这时可以使用该选项，将忘记的目录或文件追加到备份文件中。
-t 列出档案文件的内容，查看已经备份了哪些文件。
-u 更新文件。就是说，用新增的文件取代原备份文件，如果在备份文件中找不到要更新的文件，则把它追加到备份文件的最后。
-x 从档案文件中释放文件。

辅助选项：
-b 该选项是为磁带机设定的，其后跟一数字，用来说明区块的大小，系统预设值为20（20×512 bytes）。
-f 使用档案文件或设备，这个选项通常是必选的。
-k 保存已经存在的文件。例如把某个文件还原，在还原的过程中遇到相同的文件，不会进行覆盖。
-m 在还原文件时，把所有文件的修改时间设定为现在。
-M 创建多卷的档案文件，以便在几个磁盘中存放。
-v 详细报告tar处理的文件信息。如无此选项，tar不报告文件信息。
-w 每一步都要求确认。
-z 用gzip来压缩/解压缩文件，加上该选项后可以将档案文件进行压缩，但还原时也一定要使用该选项进行解压缩。

4.应用说明
tar 是Tape Archive（磁带归档）的缩写，最初设计用于将文件打包到磁带上。如果下载过Linux的源代码，或许已经碰到过tar文件

请注意，不要忘了Linux是区分大小写的。例如，tar命令应该总是以小写的形式执行。命令行开关可以是大写、小写或大小写的混合。例如，-t和-T执行不同的功能。文件或目录名称可以混合使用大小写，而且就像命令和命令行开关一样是区分大小写的。

5.应用实例
tar是一个命令行的工具，没有图形界面。使用Konsole打开一个终端窗口，接下来是一个简单的备份命令（在/temp目录中创建一个back.tar的文件，/usr目录中所有内容都包含在其中。）：
$tar cvf - /usr > /temp/back.tar

另外，tar命令支持前面第三讲中讲过的crontab命令，可以用crontab工具设置成基于时间的有规律地运行。例如，每晚6点把/usr目录备份到hda—第一个IDE接口的主驱动器 (总是位于第一个硬盘)中，只要将下面语句添加到root的crontab中即可：
$00 06 * * * tar cvf /dev/hda1/usrfiles.tar - /usr

一般情况下，以下这些目录是需要备份的：
◆/etc 包含所有核心配置文件，其中包括网络配置、系统名称、防火墙规则、用户、组，以及其它全局系统项。
◆ /var 包含系统守护进程（服务）所使用的信息，包括DNS配置、DHCP租期、邮件缓冲文件、HTTP服务器文件、dB2实例配置等。
◆/home 包含所有默认用户的主目录，包括个人设置、已下载的文件和用户不希望失去的其它信息。
◆/root 根（root）用户的主目录。
◆/opt 是安装许多非系统文件的地方。IBM软件就安装在这里。OpenOffice、JDK和其它软件在默认情况下也安装在这里。

有些目录是可以不备份的：
◆ /proc 应该永远不要备份这个目录。它不是一个真实的文件系统，而是运行内核和环境的虚拟化视图，包括诸如/proc/kcore这样的文件，这个文件是整个运行内存的虚拟视图。备份这些文件只是在浪费资源。
◆/dev 包含硬件设备的文件表示。如果计划还原到一个空白的系统，就可以备份/dev。然而，如果计划还原到一个已安装的Linux 系统，那么备份/dev是没有必要的。

unzip

1.作用
unzip 命令位于/usr/bin目录中，它们和MS DOS下的pkzip、pkunzip及MS Windows中的Winzip软件功能一样，将文件压缩成.zip文件，以节省硬盘空间，当需要的时候再将压缩文件用unzip命令解开。该命令使用权限是所有用户。

2.格式
unzip [-cflptuvz][-agCjLMnoqsVX][-P <密码>][.zip文件][文件][-d <目录>][-x <文件>]

3.主要参数
-c：将解压缩的结果显示到屏幕上，并对字符做适当的转换。
-f：更新现有的文件。
-l：显示压缩文件内所包含的文件。
-p：与-c参数类似，会将解压缩的结果显示到屏幕上，但不会执行任何的转换。
-t：检查压缩文件是否正确。
-u：与-f参数类似，但是除了更新现有的文件外，也会将压缩文件中的其它文件解压缩到目录中。
-v：执行是时显示详细的信息。
-z：仅显示压缩文件的备注文字。
-a：对文本文件进行必要的字符转换。
-b：不要对文本文件进行字符转换。
-C：压缩文件中的文件名称区分大小写。
-j：不处理压缩文件中原有的目录路径。
-L：将压缩文件中的全部文件名改为小写。
-M：将输出结果送到more程序处理。
-n：解压缩时不要覆盖原有的文件。
-o：不必先询问用户，unzip执行后覆盖原有文件。
-P<密码>：使用zip的密码选项。
-q：执行时不显示任何信息。
-s：将文件名中的空白字符转换为底线字符。
-V：保留VMS的文件版本信息。
-X：解压缩时同时回存文件原来的UID/GID。
[.zip文件]：指定.zip压缩文件。
[文件]：指定要处理.zip压缩文件中的哪些文件。
-d<目录>：指定文件解压缩后所要存储的目录。
-x<文件>：指定不要处理.zip压缩文件中的哪些文件。
-Z unzip：-Z等于执行zipinfo指令。在Linux中，还提供了一个叫zipinfo的工具，能够察看zip压缩文件的详细信息。unzip最新版本是5.50。

gunzip

1.作用
gunzip命令作用是解压文件，使用权限是所有用户。

2.格式
