project packages/apps/SoundRecorder/       ��ɣ� xx


����
app, SoundRecorder: merge jb42-a23 for kitkat
    1 record file can be shown in music apk
    2 make this apk shown in Launcher
    3 change the suffix to .mp3

------------------------------------------
project packages/apps/bluetooth            ���
0002-modify-for-support-rtl8723bs-bt.patch

android4.4 ���£����ӣ�

 if (getConnectionState(device) != BluetoothProfile.STATE_DISC    ONNECTED) {
             Log.e(TAG, "Pan Device not disconnected: " + device);
             return false;
}

���֣�.git/rebase-apply/patch:22: trailing whitespace. 
ԭ���ύ�Ĵ������Կո��β
�����ɾ��patch����β�Ŀո񣬻��߼� --ignore-whitespace

���֣�previous rebase directory ..../.git/rebase-apply still exists but mbox given.
�������ʹ��git-am֮ǰ�� ��Ҫ����git am --abort һ�Σ�����������ǰ��am��Ϣ�������ſ��Խ���һ��ȫ�µ�am��

-----------------------------------------------
project packages/apps/Browser     δ���

repoʱ���֣�
project packages/apps/Browser/
It looks like git-am is in progress. Cannot rebase.

-----------------------------------------------
project packages/apps/Calculator  ���   xx

-------------------------------------------
project package/apps/Deskclock  δ���
�ı�ϴ�

--------------------------------------------------------
packages/apps/Gallery2  δ���

----------------------------------------------
packages\apps\Music   ���

�����ͻ������
git am --abort��Ȼ���ֶ�����Ӵ�patch, patch -p1 < PATCH���ֶ�����������ͻ�����ʹ�� git commit -a �ύ���롣
�����������и�����������ʧȥPATCH��ԭ��������commit��Ϣ

�����ʹ�� git apply �����patch��Ȼ���ֶ���������ͻ���������ɵ� .rej �ļ������ͻ�ļ��Ƚϣ��޸ĳ�ͻ���ݣ������հ��ļ����뵽index�У���

$ git apply --reject PATCH 
$ edit edit edit
����ע��$ git status��
$ git add FIXED_FILES
$ git am --resolved

-------------------------------------------
packages\apps\Nfc  ��� xx

--------------------------------------------------
packages\apps\Phone 

4.4 packages\apps\Phone�޴��룿
4.4�ĳ�packages\apps\PhoneCommon ������


-----------------------------------------------------
packages\apps\SpeechRecorder ��� xx
---------------------------------------------------
packages\apps\VideoEditor  ��� xx
----------------------------------------------------
packages\apps\Camera 

4.4 ��packages\apps\Camera �� packages\apps\Camera2


mm����ģ��ʱ�����֣�No private recovery resources for TARGET_DEVICE


�ϲ�patch����new blank line at EOF.

��������������Ķ�����ͬһ���ļ����������Կ� git add --patch �ķ�ʽ������