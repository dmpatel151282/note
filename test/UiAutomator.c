
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

adb push <jar文件路径> /data/local/tmp
adb shell uiautomator runtest <jar文件名> -c <工程中的类名，包含包名> -c  <工程中的类名，包含包名> ...

--------------------------------------------------------------------------------------
http://blog.csdn.net/u010961631/article/details/9616581
public class xxx extends UiAutomatorTestCase

UiAutomatorTestCase 类

getUiDevice()

---------------------------------------------------------------------------------------
UiObject 类  
这个对象可以理解为控件的对象。

UiObject mItem = new UiObject(new UiSelector().text("English")); 

通过 UiSelector

重要方法：
click()                         ----点击控件
clickAndWaitForNewWindow()      ----点击某个控件，并等待窗口刷新
longClick()                     ----长按
clearTextField()                ----清除文本，主要针对编辑框
getChildCount()                 ----这个方法可以看出，其实UiObject也可以是一个控件的集合
getPackageName()                ----得到控件的包名
getSelector()                   ----得到当前控件的选择条件
getText()                       ----得到控件上的Text
isCheckable()
isChecked()
isClickable()
isLongClickable()
isScrollable()
isScrollable()
isSelected()                    ----判断是否具备某个属性

-----------------------------------------------------------------------------
UiCollection 对象
这个对象可以理解为一个对象的集合。因为UiSelector描述后得到的有可能是多个满足条件的控件集合，
因此可以用来生成UiCollection:

UiCollection mUiCollection = new UiCollection(new UiSelector().text("Settings"));

重要方法： 
getChild(selector)
    ----从集合中再次通过UiSelector选择一个UiObject对象
getChildByDescription(childPattern, text)
    ----从一个匹配模式中再次以text为条件选择UiObject
getChildByText(childPattern, text)
    ----与上面类似。
getChildCount()
    ----得到当前集合中控件的个数

-------------------------------------------------------------------------------
UiScrollable对象

UiScrollable可以生成一个滚动动作的对象，其最大的作用就是可以实现滚动的查找某个元素。
比如在“设置”菜单中，“语言和输入法”这个菜单比较靠下，需要滚动后才可以看到（找到）

UiScrollable settingItems = new UiScrollable(
                new UiSelector().scrollable(true));

重要方法：
    UiObject languageAndInputItem = settingItems.getChildByText(
                new UiSelector().text("Language & input"), "Language & input", true);

    setAsHorizontalList(); // 设置界面为水平，水平滑动
    

等待操作和添加Log的方法
