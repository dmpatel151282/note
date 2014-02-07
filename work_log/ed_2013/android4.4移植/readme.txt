project packages/apps/SoundRecorder/       完成， xx


灵活处理。
app, SoundRecorder: merge jb42-a23 for kitkat
    1 record file can be shown in music apk
    2 make this apk shown in Launcher
    3 change the suffix to .mp3

------------------------------------------
project packages/apps/bluetooth            完成
0002-modify-for-support-rtl8723bs-bt.patch

android4.4 更新，增加：

 if (getConnectionState(device) != BluetoothProfile.STATE_DISC    ONNECTED) {
             Log.e(TAG, "Pan Device not disconnected: " + device);
             return false;
}

出现：.git/rebase-apply/patch:22: trailing whitespace. 
原因：提交的代码中以空格结尾
解决：删掉patch中行尾的空格，或者加 --ignore-whitespace

出现：previous rebase directory ..../.git/rebase-apply still exists but mbox given.
解决：在使用git-am之前， 你要首先git am --abort 一次，来放弃掉以前的am信息，这样才可以进行一次全新的am。

-----------------------------------------------
project packages/apps/Browser     未解决

repo时出现：
project packages/apps/Browser/
It looks like git-am is in progress. Cannot rebase.

-----------------------------------------------
project packages/apps/Calculator  完成   xx

-------------------------------------------
project package/apps/Deskclock  未解决
改变较大，

--------------------------------------------------------
packages/apps/Gallery2  未解决

----------------------------------------------
packages\apps\Music   完成

如果冲突发生，
git am --abort，然后手动的添加此patch, patch -p1 < PATCH，手动解决掉代码冲突，最后使用 git commit -a 提交代码。
但是这样做有个问题就是你会失去PATCH中原本包含的commit信息

你可以使用 git apply 命令打patch，然后手动解决代码冲突（检视生成的 .rej 文件，与冲突文件比较，修改冲突内容，并最终把文件加入到index中）：

$ git apply --reject PATCH 
$ edit edit edit
（译注：$ git status）
$ git add FIXED_FILES
$ git am --resolved

-------------------------------------------
packages\apps\Nfc  完成 xx

--------------------------------------------------
packages\apps\Phone 

4.4 packages\apps\Phone无代码？
4.4改成packages\apps\PhoneCommon ？？？


-----------------------------------------------------
packages\apps\SpeechRecorder 完成 xx
---------------------------------------------------
packages\apps\VideoEditor  完成 xx
----------------------------------------------------
packages\apps\Camera 

4.4 有packages\apps\Camera 和 packages\apps\Camera2


mm编译模块时，出现：No private recovery resources for TARGET_DEVICE


合并patch错误：new blank line at EOF.

如果针对两个问题改动的是同一个文件，可以试试看 git add --patch 的方式将部分