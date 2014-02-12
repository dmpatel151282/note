
Regular Expression
	
  正则表达式就是对在某个字母表Sigma上的字符串集合的符号描述.
比较容易理解的说法: 正则表达式是一种对字符串集合的表示方法.
or: 正则表达式描述一种模式(pattern).

  一个RE描述了一个定义在某个字母表Sigma上的字符串的集合, 外加一个表示空串的字符epsilon.
将字符串的这种集合称为一种语言.
对于给定的RE r来说, 我们将它规定的语言(or:字符串集合)记作L(r)

正则表达式的三个基本运算:
选择: R | S
	ex: a是一个正则表达式, 匹配字符串a; b是一个正则表达式, 匹配字符串b;
	那么 a|b 是一个正则表达式, 匹配字符a 或者 b.

连接: RS
	ex: ab匹配ab

闭包: R* (Kleen closure, 柯林闭包)
	ex: a*匹配epsilon, a, aa, aaa, aaaa...

为消除二义性, 括号具有最高优先级, 其次为闭包, 连接和选择.
基础正则表达式只具有闭包和连接功能, 扩展正则表达式则支持括号分组和选择.

tips:
	不要把正则表达式和bash通配符混为一谈.
	bash通配符是由bash解释器扩展和匹配的, 它不是通用的描述字符串集合的表达方式.
		而正则表达式则是通用的描述字符串集合的表达方式, -- 它被Unix like系统中广泛的
		应用程序以及程式设计语言支持.

==============================================================================

二、常用正则表达式符号

	基础正则表达式
	\		转义, 将元字符转换为普通字符, or将普通字符转义为元字符
			注意: \本身也是元字符.
			因此, \\转义其自身为普通字符.
	^		锚定行首
	$		锚定行尾
	\<good		表示以good开头的单词
	sh\>		表示以sh结尾的单词
	.		表示[字母表中的, 以下略]任何一个单一字符		
	*		柯林闭包, 重复0到无穷次前一个字符
	[abc]		表示当前位置为a或b或c其中任意一个
	[^abc]		表示当前位置除了a,b,c以外任意一个字符
	[0-9]		表示0-9中的一个字符. 注意语系.
	\{n,m\}		见下示例
	a\{3\}		匹配a为3次
	a\{3,\}		匹配a为至少3次
	a\{3,100\}	匹配a为3到100次		

	扩展正则表达式(需要用egrep测试)
	()		括号优先级分组
	r|s		选择运算
	+		重复1到无穷次前一个字符, 例: a+ 等价于 aa* ,亦等价于 a\{1,\}
	?		0个或者1个前一个字符, 例: a? 等价于 a\{0,1\}

			POSIX egrep和awk而不是传统的egrep和awk
	{n,m}		见下示例(即, 扩展正则表达式中{和}成为元字符)
	{3}		    匹配a为3次
	{3,}		匹配a为至少3次
	{3,100}		匹配a为3到100次
