文件编码
1.在Vim中可以直接查看文件编码
:set fileencoding
即可显示文件编码格式。

2. vimrc 中

set encoding=utf-8

设置字符编码列表
set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr  
如果没有找到合适的编码，就用latin-1(ASCII)编码打开

文件编码转换
1.在Vim中直接进行转换文件编码,比如将一个文件转换成utf-8格式
:set fileencoding=utf-8

2. iconv 转换，iconv的命令格式如下：
iconv -f encoding -t encoding inputfile

比如将一个UTF-8 编码的文件转换成GBK编码
iconv -f utf-8 -t gbk file1 -o file2

文件名编码转换:

convmv
convmv -f 源编码 -t 新编码 [选项] 文件名

常用参数：
-r 递归处理子文件夹
--notest 真正进行操作，请注意在默认情况下是不对文件进行真实操作的，而只是试验。
--list 显示所有支持的编码
--unescap 可以做一下转义，比如把%20变成空格

比如我们有一个utf8编码的文件名，转换成GBK编码，命令如下：
convmv -f UTF-8 -t GBK --notest utf8编码的文件名


