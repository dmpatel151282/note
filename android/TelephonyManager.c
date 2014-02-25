    http://www.cnblogs.com/linjiqin/archive/2011/02/26/1965682.html
  TelephonyManager类主要提供了一系列用于访问与手机通讯相关的状态和信息的get方法。
其中包括手机SIM的状态和信息、电信网络的状态及手机用户的信息。

添加权限：

对象可以通过Context.getSystemService(Context.TELEPHONY_SERVICE)方法来获得.

/**
 ** 返回电话状态
 ** CALL_STATE_IDLE 无任何状态时 
 ** CALL_STATE_OFFHOOK 接起电话时
 ** CALL_STATE_RINGING 电话进来时 
 **/
对象.getCallState();


    
