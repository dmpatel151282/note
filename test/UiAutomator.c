http://blog.csdn.net/u010961631/article/details/9616581
public class xxx extends UiAutomatorTestCase

UiAutomatorTestCase 类

getUiDevice()

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
isSelected()
    ----判断是否具备某个属性

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

等待操作和添加Log的方法
