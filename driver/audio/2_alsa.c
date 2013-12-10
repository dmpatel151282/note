
Alsa Lib 

Lib 移植不需要修改一丁点儿的源码，只需要重新编译 lib 以支持自己的平台。

编译过程如下：
1.Configure

./configure --host=arm-none-linux-gnueabi --prefix=/home/zhkkk/github/out/alsa-lib/arm-linux/ --enable-static=yes --enable-shared=no --with-configdir=/home/zhkkk/github/out/alsa-lib/arm-linux/config/

配置会先对此命令中的配置项进行检查，然后实现配置。需要注意的是：
配置如果出错，未必一定报错，而是会采用默认的配置选项。几个重要的配置项：

  1）--host   指定编译器。
  2）--prefix 指定编译后文件的安装路径，会在该目录中创建 lib 和 include 目录
  3）--with-configdir 指定 conf 文件的安装目录，该目录中对我们最有用的 
                      alsa.conf ，此文件会被直接移植到你的目标系统中。
  3）--enable-static  [no]
     --enable-shared  [yes]  

2. make 

3. make install 
 1）install 可能会在一些非用户目录安装一些文件，因此需要 root 权限
 2）install 也会执行一些编译，因此记得保证 root 下的交叉编译器同样可用。 

4.安装到嵌入式平台
  1）lib 库文件，放在 /system/lib/ 中
  2）conf 文件，应放在 /usr/local/share 中

--------------------------------------------------------------------------
Alsa Util 移植

Util 会生成用于播放，录制，配置音频的应用文件，对测试很有用处。
编译过程如下： 
1. Configure

./configure --host=arm-none-linux-gnueabi --prefix=/home/zhkkk/github/out/alsa-lib/arm-linux/ CFLAGS="-I/home/zhkkk/server/172.20.1.86/opt/alsa-lib/out/arm-linux/include" LDFLAGS="-L/home/zhkkk/server/172.20.1.86/opt/alsa-lib/out/arm-linux/lib -lasound" --disable-alsamixer --disable-xmlto --with-alsa-inc-prefix=/home/zhkkk/server/172.20.1.86/opt/alsa-lib/out/arm-linux/include --with-curses=ncurses --enable-static 

  1）--host 指定编译器，与 lib 的配置选项相同
  2）--prefix 指定编译后文件的安装路径，与 lib 的配置选项相同
  3）CFLAGS   lib 编译后生成的头文件
  4）LDFLAGS  lib 编译生成的库文件
  5）--with-alsa-inc-prefix 指定头文件目录，这个与 3） 类似，但必须指定，
     因为 Configure 程序会去该目录检查版本情况。如果不指定的话，
     则会直接去默认目录 ( 即宿主机对应的 alsalib 目录中寻找，可能会有错误 ) 
  
  2. make 
  3. sudo make install 
  4. 安装
        bin/下的可执行文件 copy 板子/system/bin/
        alsa.conf copy 板子/usr/local/share/alsa.conf 

util 的使用

arecord [flags] [filename]
aplay [flags] [filename [filename]] ...

设备: -D
采样格式: -f    [U8]
采样率: -r      [8000hz]
通道: -c        [1]
录音时间：-d    [0]         Interrupt after # seconds
--period-size
--buffer-size

---------------------------------------------------------------------------
issue

1.执行命令时：No such file or directory
原因：动态编译的程序有INTERP段，它指明了程序在加载过程中需要使用的动态链接加
      载器，如果这个加载器未找到，那么就会提示“ No such file or directory”
解决：使用readelf -l查看可执行文件需要的动态链接加载器，在板子上创建拷贝它。

遇到 configure: error: required curses helper header not found
→执行 sudo apt-get install libncurses5-dev 安装缺少的套件   
遇到 configure: error: panelw library not found
→改执行./configure --with-curses=ncurses
遇到 /bin/bash: xmlto: command not found
→执行 sudo apt-get install xmlto 安装缺少的套件 

2.执行aplay时，未指定period-size参数则kernel panic; 
  指定则出现aplay: pcm_write:1939: write error: Input/output error
