Alsa Lib 

Lib 移植不需要修改一丁点儿的源码，只需要重新编译 lib 以支持自己的平台。

编译过程如下：
1.Configure

静态库
./configure --host=arm-none-linux-gnueabi --prefix=/home/zhkkk/github/out/alsa-lib/arm-linux/ --enable-static=yes --enable-shared=no --with-configdir=/data/local/share

动态库
./configure --host=arm-none-linux-gnueabi --prefix=/home/zhkkk/github/out/alsa-lib/arm-linux/ --enable-static=no --enable-shared=yes --with-configdir=/data/local/share

配置会先对此命令中的配置项进行检查，然后实现配置。需要注意的是：
配置如果出错，未必一定报错，而是会采用默认的配置选项。几个重要的配置项：

  1）--host   指定编译器。
  2）--prefix 指定编译后文件的安装路径，会在该目录中创建 lib 和 include 目录
  3）--with-configdir 
        指定 conf 文件的安装目录，该目录中对我们最有用的 
        alsa.conf ，此文件会被直接移植到你的目标系统中。
        它将影响 include/config.h中的 ALSA_CONFIG_DIR 目录
  4）--enable-static  [no]
     --enable-shared  [yes]  

2. make 

3. make install 
 1）install 可能会在一些非用户目录安装一些文件，因此需要 root 权限
 2）install 也会执行一些编译，因此记得保证 root 下的交叉编译器同样可用。 

4.安装到嵌入式平台
  1）lib 库文件，放在 /system/lib/ 中
  2）conf 文件，应放在 /usr/local/share 中
-------------------------------------------------------------------------
alsa-lib 重要的数据结构和函数接口

snd_pcm_t                                           //pcm设备handle
snd_pcm_stream_t                                    //PCM stream (direction)
int snd_pcm_open(snd_pcm_t **pcmp, const char *name,         
         snd_pcm_stream_t stream, int mode)     //打开PCM设备，获得handle

snd_pcm_hw_params_t
int snd_pcm_hw_params_malloc(snd_pcm_hw_params_t*)          //分配
int snd_pcm_hw_params_any(snd_pcm_t*, snd_pcm_hw_params_t*) //初始化硬件结构
snd_pcm_hw_params_set_access()
snd_pcm_hw_params_set_format()
snd_pcm_hw_params_set_rate_near()
snd_pcm_hw_params_set_channels()
int snd_pcm_hw_params(snd_pcm_t*, snd_pcm_hw_params_t*)       //设置硬件
snd_pcm_hw_params_free()

int snd_pcm_prepare(snd_pcm_t*)                         //准备PCM硬件

snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm,        //写PCM数据到声卡
        const void *buffer, snd_pcm_uframes_t size)

snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm,              
        void *buffer, snd_pcm_uframes_t size)

int snd_pcm_close(snd_pcm_t*)

----------------------------------------------------------------------
snd_pcm_open 过程
  1.snd_config_update()    刷新config配置
    1.1 snd_config_update_r(&snd_config, &snd_config_global_update, NULL);
  2.snd_pcm_open_noupdate(pcmp, snd_config, name, stream, mode, 0)



