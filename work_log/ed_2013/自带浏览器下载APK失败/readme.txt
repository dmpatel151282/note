假设：
	1.内存不足导致downloader被停掉，lowmemorykiller: Killing 'Download Servic' 		
	2.网络有关，导致下载失败
	
	
可能代码相关：
		1. packages/apps/Browser
		2. packages/providers/DownloadProvider   
			DownloadProvider.apk (android.process.media) DownloadProviderui.apk(android.process.media)
		3. android/frameworks/base/services/java/com/android/server/am/ActivityManagerService.java  ProcessList.java
		

lowmemorykiller机制：在达到某个内存门限的情况下去选择进程删除来释放内存

/sys/module/lowmemorykiller/parameters/adj        adj文件包含oomadj队列
/sys/module/lowmemorykiller/parameters/minfree    minfree包含对应oom_adj的警戒值。

对应的minfree值达到，则进程的oomadj如果大于这个值将被杀掉。

AndroidManifest.xml 中 <application android:process="android.process.media"   -> 进程名
Top -> 进程号 
/proc/进程号/ -> oom_adj oom_score等
oom_adj 0
oom_score 69


测试:
	1. P1、P4对比测试。P4无此问题。
	2. 定位download时的代码，进程名字
		
	DownloadService onCreate() -> DownloadService onStartCommand() ->
	DownloadHandler.java startDownloadThreadLocked() ->  DownloadThread.java executeDownload()

确定当内DownloadManager被kill后，出现下载失败，
服务重启出现：W/DownloadManager( 2006): Aborting request for download 39: Trying to resume a download that can't be resumed
无法重新下载。	

因为：setupDestinationFile() 中   ...else if (mInfo.mETag == null && !mInfo.mNoIntegrity) { 抛出异常...

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

