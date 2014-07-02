
开机动画的源码：frameworks/base/cmds/bootanimation下

---------------------------------------------------------------------------
1.更改 kernel log (logo_linux_clut224.ppm)
  将新生成的logo_linux_clut224.ppm 替换/drivers/video/logo/logo_linux_clut224.ppm 文件，删除该文件夹下的logo_linux_clut224.o，编译烧录，就可以看到你所更改的图片。

2.更改 android 第一个log (initlogo.rle)

3.更改 android第二张图片(动画)
  开机动画文件为：/system/media/bootanimation.zip
    |-- part0/            #png 图片
    |-- desc.txt

        400 800 18  
        p 1 0 part0  
          #400 800 为图片的分辨率 400*800  
          #18为每秒显示图片的张数，也就是18fps/S  
          #p代表定义一个部分。  
          #p后面的第一个数是重复播放这一部分次数。如果这个数为0，就无限循环播放  
          #p后面第二个数是播放下一个部分前的延迟帧数  
          #字符串定义了加载文件的路径 

  zip -r -X -Zstore bootanimation part*/*.png desc.txt



