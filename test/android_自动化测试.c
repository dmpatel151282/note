
自动化完成：


Android自动化测试主要分为Monkeyrunner、Rubotium、UiAutomator、Monkey（在我看来这个不算）等。

主要特点：

1、Monkeyrunner：
    优点：操作最为简单，可以录制测试脚本，可视化操作；
    缺点：主要生成坐标的自动化操作，移植性不强，功能最为局限；
    
2、Rubotium：
    优点：主要针对某一个APK进行自动化测试，APK可以有源码，也可以没有源码，功能强大；
    缺点：只能针对APK操作，而且需要对APK重新签名（有工具），因此操作相对复杂；

3、UiAutomator：
    优点：可以对所有操作进行自动化，操作简单；
    缺点：Android版本需要高于4.0，无法根据控件ID操作，相对来说功能较为局限，但也够用了；

4、Monkey：准确来说，这不算是自动化测试，因为其只能产生随机的事件，无法按照既定的步骤操作；

由上面介绍可以有这样的结论：
    测试某个APK，可以选择Rubotium；
    测试过程可能涉及多个APK，选择UiAutomator；
    一些简单的测试，选择Monkeyrunner；


UiAutomator 环境搭建
  必备条件：
  1、JDK              
  2、SDK（API高于15）
  3、Eclipse（安装ADT插件）
  4、ANT（用于编译生成jar）
  所需环境变量：
      JAVA_HOME 
      ANDROID_HOME 
      ANT_HOME

详细操作
1.建立工程
 用Eclipse新建Java Project，注意，不是Android Project！

2. 添加JUnit库

3. 添加Android库
  找到路径Android-sdk/platforms/android-17/下面的android.jar和uiautomator.jar添加进来

4. 编写测试程序：在src中添加包，然后添加class文件

5. 找到SDK ID

Android-sdk/tools/目录下，运行命令：
    android list
        查看API大于15的SDK的ID值

6. 创建build文件
Android-sdk/tools/目录下，运行命令：
    android create uitest-project -n <name> -t <android-sdk-ID> -p <path>

上面的name就是将来生成的jar包的名字，可以自己定义，path是Eclipse新建的工程的路径；
运行命令后，将会在工程的根目录下生成build.xml文件。

7. 编译生成jar
进入项目的工程目录，然后运行ant build，将使用ant编译生成jar
在bin目录下生成jar文件。

8. push并运行jar

adb push <jar文件路径> data/local/tmp
adb shell uiautomator runtest <jar文件名> -c <工程中的类名，包含包名>


