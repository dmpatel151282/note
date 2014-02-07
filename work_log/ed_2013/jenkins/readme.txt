
jenkins上执行构建brush.xml 无法启动 LiveSuit
LiveSuit: cannot connect to X server

假设一：给尝试访问图形程序的用户主机赋予xhost权限：
xhost +$HOSTNAME 

假设二：未将DISPLAY配置到环境变量上，可如下配置：
export DISPLAY=:0.0

假设三：桌面环境Gnome、Unity界面（11.04默认界面）




------------------------------------------------------------------------

https://wiki.jenkins-ci.org/display/JENKINS/Jenkins+CLI
Jenkins CLI
Jenkins has a built-in command line client that allows you to access Jenkins from a script or from your shell. 
This is convenient for automation of routine tasks, bulk updates, trouble diagnosis, and so on.