Activity����������(7��������3���׶�)

onCreate
onStart
onRestart
onResume
onPause
onStop
onDestroy

--------------------------------------
��ʼActivity
onCreate -> onStart -> onResume

����Activity
onPause -> onStop -> onDestroy

���»�ȡ����
onRestart -> onStart -> onResume

--------------------------------------
�����������ڣ�Activityʼ�տɼ�
onPause -> onResume -> onPause

�������ڣ�
onStart -> onRestart -> onResume -> onPause -> onStop -> onStart

--------------------------------------

Activity ֮�䴫������

Intent

Intent Structure
action��
     ACTION_EDIT, ACTION_MAIN��ACTION_VIEW

category����� Gives additional information about the action.

CATEGORY_LAUNCHER means it should appear in the Launcher as a top-level application
CATEGORY_ALTERNATIVE means it should be included in a list of alternative actions the user can perform on a piece of data.


data��uri��ʽ

type �� Specifies an explicit type (a MIME type) of the intent data.

component: �������Ҳ�������ָ�������

extras��This is a Bundle of any additional information.


---------------------------
ͨ����̬������������

�����ļ�
  <button   android:id 
  ></button>

activity���У�
private Button button;

oncreat()�У�
setContentView(R.layout.main); //���ز����ļ�

button = (Button)this.findViewById(R.id.button);
button.setOnClickListener(new View.onClickListener());

public void onClick(View v){

    Intent intent = new Intent(main.this, otherActivity.class);//ѡ��ָ������ķ���
    Intent.putExtra("name", "����");	//name, Ӧ���ϰ���com.xxx.xxx.xxx.name
    Intent.putExtra("age","23");
    startActivity(intent);
}

ȡ����
��otherActivity���У�
onCreate()�У�
setContentView(R.layout.other);
Intent intent = getIntent();
String name = intent.getStringExtra("name");
int age = intent.getStringExtra("age");
textView.setText("age--->>" + age);

---------------------------------------------------------
ͨ�����а崫������

----------------------------------------------------------o
�߳�	���в�ȷ����

java���߳�

  ʵ�ַ�ʽ��1. �̳�Thread�࣬ ��дrun����

����:
 class xx extends Thread{
 public void run(){
 	Thread.sleep(1000) //�߳�����1000���룬sleepʹ�߳̽���Block״̬�����ͷ���Դ
 }}
 �����߳�:
 ����.start() //�����̣߳�run��������

	    2. ʵ��Runnable�ӿڣ���дrun����
�����߳�:
 Thread t = new Thread(����) //�����̶߳���
 t.start()

	    3. ʵ��Callable�ӿڣ���дcall����

  �������ڣ�
	    1. ����
	    2. �����߳�start, �������״̬�������ռCPU-> 3
	    3. ����״̬�����������߳���ռ��-> �������״̬
			 ��������� -> �������״̬
	    4. �߳���ִ����ϣ� -> ��������״̬

  ���߳�ͬ�����������
	    
 	    
mainThread   	���̣߳�UI�̣߳�
woker Thread	

ԭ��UI�߳�֮�⣬�����޸�UI

ProgressBar setProgress���� ������woker Thread��ʹ��

���߳������������ANR����
��һ��Ӧ�ó����У����߳�ͨ�����ڽ����û��������Լ�������Ľ���������û���
����һЩ���ܻ���������Ĳ����������woker Thread�С�


Handler��Looper��MessageQueue ��Ϣ�������

Handler����Ϣ���������Ϣ����
Looper��ͣ����Ϣ������ȡ������û�ж�����ȴ�
ÿȡ��һ����ϢLooper����   ����Handlerȥ����


ʹ�ã�
  
����һ���� �̳� handler�࣬ ��д handleMeassage(Message msg) ���ӡ��Ϣ��								
��������� handler

����Message���� Message msg = handler.obtainMessage();
		  msg.what = 2;
		  handler.sendMessages(msg); //����Ϣ������Ϣ����
					     //Looper����ȡ��msg���ҳ���֮��Ӧ��handler
					     //Looper���������handler.handleMessage(Message msg);

1 ͨ��Handlerʵ���̼߳�ͨ��

2 �����̵߳���ʵ��Handler��handleMessage��������

3 ��Worder Thread����ͨ��Handler������Ϣ��

eg.

�ڲ���NetworkThread�̳�Thread����дrun����������2�룬s��ֵģ��������л�ȡ�����ݡ�s��ֵ�ŵ�textview�У�����ֱ���ڸ��߳��и���ui�����ݡ�

button��������onClickListener�����У�����NetworkThread�̶߳�������

����һ��Handler��ʵ����̳�Handler,ʵ��handleMessage����������

��MyHandler����handler����

�����߳��ж���msg��handler.obtionMessage�������ɣ�������Ϣ��handler.sendMessage��������msg��Ϊ�������롣sendMessage�������������̻߳�WorkerThread�з��Ͷ��ǿ��Եġ�

���߳��н�����Ϣ--handler�����߳��ж��壬handleMessage������ϢҲ�������̡߳������������߳�֮���ͨ����



�෴���ӣ���mainthread��workerthread������Ϣ��

1 ׼��looper����

2 ��WorkerThread��������Handler����

3 ��MainThread���з�����Ϣ

��WorkerThread��run������ִ��Looper.prepare������������һ��Looper���󣬲���ɸö���ĳ�ʼ��������

��WorkerThread������handler�����������ڲ��ࡣhandler = new Handler(){
						public void handleMessages(Message msg){ }
						}

Looper����loop����������֮��looper���󽫲��ϴ���Ϣ������ȡ����Ϣ����Ȼ�����handler��handleMessage��������������Ϣ������worker thread�У��������Ϣ������û�ж�������߳�������

���߳��ж���workerthread��������

button��������onClick()����������msg�ͷ�����Ϣ��

Looper.loop();

onClick���������̣߳�handleMessage������worker thread�����͡�������Ϣ�ڲ�ͬ�̡߳�

���ĸ��߳���ʵ��handler����new Handler����---ʵ����������дhandleMessage���������������ĸ��߳��д�����Ϣ��handler���������̡߳�

�����߳���worker thread������Ϣ��worker thread��run������̶��������裺

��1��Looper.prepare��

��2��handler = new MyHandler�������������������ڲ���new Handler����{}��

��3��Looper.loop������


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

ʵ��ContentProvider�Ĺ���
1. ����һ��CONTENT_URI����   content://name...
2������һ���࣬�̳� ContentProvider
3. ʵ��query��insert�ȷ���
4. 
	

---------------------------------------------------------------------

service

����һ�������Ľ���
����һ���߳�

����һ���࣬�̳�servce; ��дonCreate onDestroy onStartCommand��
��AndroidManifest.xmlע��

��������
   Intent intent
   intent.setClass();
   startService(intent);

�رշ���
   Intent intent
   intent.setClass();
   stopService(intent);

onStartCommand ����intent��������ݣ�ʵ��service��������

 
onBind 


