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
1.action��
ACTION_MAIN         <==> android.intent.action.MAIN
ACTION_EDIT 
ACTION_VIEW
ACTION_PICK
ACTION_GET_CONTENT

2.category����� Gives additional information about the action.

CATEGORY_LAUNCHER   <==> android.intent.category.LAUNCHER 

CATEGORY_DEFAULT

CATEGORY_ALTERNATIVE

3.data��uri��ʽ
    

4.type �� Specifies an explicit type (a MIME type) of the intent data.

5.component: �������Ҳ�������ָ�������

6.extras��This is a Bundle of any additional information.

---------------------------
ͨ����̬������������

�����ļ�
  <button   android:id ></button>

activity���У�
private Button button;

oncreat()�У�
setContentView(R.layout.main); //���ز����ļ�
button = (Button)this.findViewById(R.id.button);
button.setOnClickListener(new View.onClickListener());

public void onClick(View v){

    Intent intent = new Intent(main.this, otherActivity.class);//ѡ��ָ������ķ���
    Intent.putExtra("name", "����");
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


