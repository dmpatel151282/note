Toast 用于向用户显示一些帮助/提示

重要方法：
    Toast.makeText()
    对象.setGravity()
    对象.show()

----------------------------------------------------------------------------
PendingIntent 

  主要持有的信息是它所包装的Intent和当前App 的Context，使得外部App可以如同
当前 App一样的执行PendingIntent里的Intent，就算在执行时当前App已经不存在了，
也能通过存在PendingIntent里的 Context照样执行Intent。 

重要方法：
    PendingIntent.getBroadcast()
    PendingIntent.getActivities()
    PendingIntent.getService()

----------------------------------------------------------------------------
AlarmManager    

重要方法：
    对象.setExact(AlarmManager.RTC_WAKEUP, timeInMillis, pendingIntent);  4.4
    对象.set();


----------------------------------------------------------------------------
PowerManager   用于控制设备的电源状态的切换     http://buaadallas.blog.51cto.com/399160/376930

重要方法：
  1. newWakeLock(int flags, String tag); //取得相应层次的锁
  参数flags说明:
    PARTIAL_WAKE_LOCK: Screen off, keyboard light off
    SCREEN_DIM_WAKE_LOCK: screen dim, keyboard light off
    SCREEN_BRIGHT_WAKE_LOCK: screen bright, keyboard light off
    FULL_WAKE_LOCK: screen bright, keyboard bright
    ACQUIRE_CAUSES_WAKEUP: 一旦有请求锁时强制打开Screen和keyboard light
    ON_AFTER_RELEASE: 在释放锁时reset activity timer

    
  2. void goToSleep(long time); //强制设备进入Sleep状态

  3. void userActivity(long when, boolean noChangeLights);
     //User activity事件发生,设备会被切换到Full on的状态,同时Reset Screen off timer.

eg:
    PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
    PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
    wl.acquire();
    ..screen will stay on during this section..
    wl.release();

----------------------------------------------------------------------------
KeyguardManager    用于锁屏和解锁的类

通过调用Context.getSystemService(Context.KEYGUARD_SERVICE)来获取实例

内部类:  KeyguardLock
    disableKeyguard()函数来解除锁屏。
    reenableKeyguard()反解除锁屏。意思是：如果在调用disableKeyguard()函数之前是锁屏的，
那么就进行锁屏，否则不进行任何操作。当然如果之前没调用disableKeyguard()函数，也不进行任何操作。

------------------------------------------------------------------------------
Iterator

------------------------------------------------------------------------------
Method
