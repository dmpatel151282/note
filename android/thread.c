线程(运行不确定性)

实现方式：
1. 继承Thread类， 重写run函数

  创建:
   class 类名 extends Thread{
    public void run(){
   	Thread.sleep(1000) //线程休眠1000毫秒，sleep使线程进入Block状态，并释放资源
   }}

   开启线程:
   对象.start() //启动线程，run函数运行

2. 实现Runnable接口，重写run函数

new Thread(new Runnable() {
    @Override
    pubilc void run() {
    }
}, "nameofThread").start();

class 类名 implements Runnable {
    @Override
    pubilc void run() {}
}


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
