Activity的生命周期(7个方法、3个阶段)

onCreate
onStart
onRestart
onResume
onPause
onStop
onDestroy

--------------------------------------
开始Activity
onCreate -> onStart -> onResume

结束Activity
onPause -> onStop -> onDestroy

重新获取焦点
onRestart -> onStart -> onResume

--------------------------------------
焦点生命周期：Activity始终可见
onPause -> onResume -> onPause

生命周期：
onStart -> onRestart -> onResume -> onPause -> onStop -> onStart

--------------------------------------

Activity 之间传递数据

Intent

Intent Structure
action：
     ACTION_EDIT, ACTION_MAIN，ACTION_VIEW

category：类别， Gives additional information about the action.

CATEGORY_LAUNCHER means it should appear in the Launcher as a top-level application
CATEGORY_ALTERNATIVE means it should be included in a list of alternative actions the user can perform on a piece of data.


data：uri形式

type ： Specifies an explicit type (a MIME type) of the intent data.

component: 若以上找不到，则指定组件。

extras：This is a Bundle of any additional information.


---------------------------
通过静态变量传递数据

布局文件
  <button   android:id 
  ></button>

activity类中：
private Button button;

oncreat()中：
setContentView(R.layout.main); //加载布局文件

button = (Button)this.findViewById(R.id.button);
button.setOnClickListener(new View.onClickListener());

public void onClick(View v){

    Intent intent = new Intent(main.this, otherActivity.class);//选择指定组件的方法
    Intent.putExtra("name", "张三");	//name, 应带上包名com.xxx.xxx.xxx.name
    Intent.putExtra("age","23");
    startActivity(intent);
}

取数据
在otherActivity类中：
onCreate()中：
setContentView(R.layout.other);
Intent intent = getIntent();
String name = intent.getStringExtra("name");
int age = intent.getStringExtra("age");
textView.setText("age--->>" + age);

---------------------------------------------------------
通过剪切板传递数据

----------------------------------------------------------o
线程	运行不确定性

java中线程

  实现方式：1. 继承Thread类， 重写run函数

创建:
 class xx extends Thread{
 public void run(){
 	Thread.sleep(1000) //线程休眠1000毫秒，sleep使线程进入Block状态，并释放资源
 }}
 开启线程:
 对象.start() //启动线程，run函数运行

	    2. 实现Runnable接口，重写run函数
开启线程:
 Thread t = new Thread(对象) //创建线程对象
 t.start()

	    3. 实现Callable接口，重写call函数

  生命周期：
	    1. 创建
	    2. 调用线程start, 进入就绪状态，如果抢占CPU-> 3
	    3. 运行状态，如果被别的线程抢占，-> 进入就绪状态
			 如果阻塞， -> 进入就绪状态
	    4. 线程体执行完毕， -> 进入死亡状态

  多线程同步：解决竞争
	    
 	    
mainThread   	主线程（UI线程）
woker Thread	

原则：UI线程之外，不能修改UI

ProgressBar setProgress方法 可以在woker Thread中使用

主线程阻塞会产生：ANR问题
在一个应用程序中，主线程通常用于接收用户的输入以及将运算的结果反馈给用户。
对于一些可能会产生阻塞的操作必须放在woker Thread中。


Handler、Looper、MessageQueue 消息处理机制

Handler把消息对象加入消息队列
Looper不停从消息队列中取出对象，没有对象则等待
每取出一个消息Looper调用   交给Handler去处理


使用：
  
创建一个类 继承 handler类， 复写 handleMeassage(Message msg) 如打印消息等								
创建其对象 handler

创建Message对象： Message msg = handler.obtainMessage();
		  msg.what = 2;
		  handler.sendMessages(msg); //将消息放入消息队列
					     //Looper将会取出msg，找出与之对应的handler
					     //Looper将会调用其handler.handleMessage(Message msg);

1 通过Handler实现线程间通信

2 在主线程当中实现Handler的handleMessage（）方法

3 在Worder Thread当中通过Handler发送消息。

eg.

内部类NetworkThread继承Thread，复写run方法。休眠2秒，s的值模拟从网络中获取的数据。s的值放到textview中，不能直接在改线程中更新ui的内容。

button监听器的onClickListener方法中，定义NetworkThread线程对象并启动

定义一个Handler的实现类继承Handler,实现handleMessage（）方法。

用MyHandler定义handler对象。

网络线程中定义msg，handler.obtionMessage（）生成，发送消息（handler.sendMessage（）），msg作为参数传入。sendMessage（）方法在主线程或WorkerThread中发送都是可以的。

主线程中接收消息--handler在主线程中定义，handleMessage处理消息也是在主线程。建立起两个线程之间的通道。



相反例子：从mainthread向workerthread发送消息。

1 准备looper对象

2 在WorkerThread当中生成Handler对象

3 在MainThread当中发送消息

在WorkerThread的run方法中执行Looper.prepare（）；就生成一个Looper对象，并完成该对象的初始化工作。

在WorkerThread中生成handler对象。用匿名内部类。handler = new Handler(){
						public void handleMessages(Message msg){ }
						}

Looper调用loop（）方法。之后，looper对象将不断从消息队列中取出消息对象。然后调用handler的handleMessage（）方法处理消息对象（在worker thread中）。如果消息队列中没有对象，则该线程阻塞。

主线程中定义workerthread并启动。

button监听器的onClick()方法，生成msg和发送消息。

Looper.loop();

onClick运行在主线程，handleMessage运行在worker thread，发送、接收消息在不同线程。

在哪个线程中实现handler对象new Handler（）---实例化，并复写handleMessage（）方法，就在哪个线程中处理消息。handler定义在主线程。

从主线程向worker thread发送消息，worker thread中run方法里固定三个步骤：

（1）Looper.prepare；

（2）handler = new MyHandler（）；（或者用匿名内部类new Handler（）{}）

（3）Looper.loop（）；


--------------------------------------------------------------------------------------------------

content provider

    URI
    android.provider
	query()
	insert()
	update()
	delete()
	getType()
	onCreate()

实现ContentProvider的过程
1. 定义一个CONTENT_URI常量   content://name...
2，定义一个类，继承 ContentProvider
3. 实现query、insert等方法
4. 
	

---------------------------------------------------------------------

service

不是一个单独的进程
不是一个线程

创建一个类，继承servce; 复写onCreate onDestroy onStartCommand等
在AndroidManifest.xml注册

启动服务
   Intent intent
   intent.setClass();
   startService(intent);

关闭服务
   Intent intent
   intent.setClass();
   stopService(intent);

onStartCommand 接收intent对象的数据，实现service的主功能

 
onBind 


