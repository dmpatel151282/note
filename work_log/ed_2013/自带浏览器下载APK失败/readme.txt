���裺
	1.�ڴ治�㵼��downloader��ͣ����lowmemorykiller: Killing 'Download Servic' 		
	2.�����йأ���������ʧ��
	
	
���ܴ�����أ�
		1. packages/apps/Browser
		2. packages/providers/DownloadProvider   
			DownloadProvider.apk (android.process.media) DownloadProviderui.apk(android.process.media)
		3. android/frameworks/base/services/java/com/android/server/am/ActivityManagerService.java  ProcessList.java
		

lowmemorykiller���ƣ��ڴﵽĳ���ڴ����޵������ȥѡ�����ɾ�����ͷ��ڴ�

/sys/module/lowmemorykiller/parameters/adj        adj�ļ�����oomadj����
/sys/module/lowmemorykiller/parameters/minfree    minfree������Ӧoom_adj�ľ���ֵ��

��Ӧ��minfreeֵ�ﵽ������̵�oomadj����������ֵ����ɱ����

AndroidManifest.xml �� <application android:process="android.process.media"   -> ������
Top -> ���̺� 
/proc/���̺�/ -> oom_adj oom_score��
oom_adj 0
oom_score 69


����:
	1. P1��P4�ԱȲ��ԡ�P4�޴����⡣
	2. ��λdownloadʱ�Ĵ��룬��������
		
	DownloadService onCreate() -> DownloadService onStartCommand() ->
	DownloadHandler.java startDownloadThreadLocked() ->  DownloadThread.java executeDownload()

ȷ������DownloadManager��kill�󣬳�������ʧ�ܣ�
�����������֣�W/DownloadManager( 2006): Aborting request for download 39: Trying to resume a download that can't be resumed
�޷��������ء�	

��Ϊ��setupDestinationFile() ��   ...else if (mInfo.mETag == null && !mInfo.mNoIntegrity) { �׳��쳣...

DownloadHandler.java startDownloadThreadLocked() -> DownloadInfo.startDownloadThread -> DownloadTread.DownloadThread
                                                                                        SystemFacade.startThread
                                                                                        
DownloadInfo info = mDownloadsQueue.get(id);
info.startDownloadThread();


http://papaya-backend.net/2013/04/12/why-http-etag-header-may-cause-your-downloading-apps-on-android-failed/
The default Android download manager use ETag header to determine whether the downloaded content is the same resource. If you have two threads downloading the same resource, 
the download manager will check whether they have the same ETag, an error will be raised if no Etag is provided(Yes, we have no ETag header). The default Android browser will
send an GET request to the server and get the content reponsed, if the content is an apk the browser will send an intent to the download manager and let it download it again. 
The beheavior of the browser may explain why there are two concurrent download threads, and first thread may not finisesh when the new thread of the download manager runs.

So we add a ETag header on our server, and it sovle the problem.

