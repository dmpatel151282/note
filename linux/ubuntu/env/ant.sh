#!/bin/sh

DOC_PATH="/home/liushui/github/doc/"

read -p "是否安装 java ? [Y/n]" check
if [ $check = "Y" -o $check = "y" ]; then
    #echo ""
    #echo "********* install java ***********"
    #apt-get install openjdk-7-jdk

    #echo -e "\n#for java\nJAVA_HOME=/usr/lib/jvm/default-java/" 
    #echo -e "CLASSPATH=$JAVA_HOME/lib:$JAVA_HOME/jre/lib" >> /home/liushui/.bashrc
    #echo -e "export JAVA_HOME CLASSPATH\nexport PATH=$PATH:$JAVA_HOME/bin\n"
else 
    read -p "ant 需要 java 支持，并配好JAVA_HOME CLASSPATH 变量，是否已经完成? [Y/n]" check
    if [ ! $check = "Y" -a ! $check = "y" ]; then
        echo "ant 安装终止。"
        exit 1
    fi
fi

echo ""
echo "********** install ant ***********"
apt-get install ant
cp ${DOC_PATH}/jsch-0.1.50.jar -v /usr/share/ant/lib/ || \
{
    echo "ssh 功能将不能使用"
    exit 2
}

echo "********* install ant successful! ***********"
