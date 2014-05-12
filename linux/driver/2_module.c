
module_

module_init(dev_name_init);  subsys_initcall();
module_exit(dev_name_exit);  module_exit();

MODULE_LICENSE("GPL");




-------------------------------------------------------------------------------------
Module 编译

1. 在编译modules之前要先区内核所在的目录下执行 make modules_prepare
    即先编译过内核



-------------------------------------------------------------------------
一、内核模块对操作系统版本的依赖
  内核模块经常会用到操作系统提供的内核接口。而linux操作系统不同的版本提供的内核接口会有所不同。

加载内核模块，function 或者 symbol无法解析的错误。
用ksyms -a 命令查看操作系统目前提供了哪些可调用的函数和变量
内核中module_list变量

-------------------------------------------------------------------------
模块加载有先后
  模块之间的调用:
    1.使用宏 EXPORT_SYMBOL 导出函数
    2.通过include 

单向调用关系: 

双向调用关系:
    1.注册方式
    先在模块 b 中定义一个函数指针，用于存放模块 a 中定义的函数指针，将函数
指针导出供模块 a 使用，在模块 a 中的初始化函数中将模块 b 需要的函数地址赋
值给模块 b 定义的函数指针。这样先加载模块 b，再加载模块a，两个模块实现了相
互调用. 


1、在模块代码中不修改操作系统内容
    纯粹以内核模块的形式在内核态调用操作系统已经提供的调用接口函数或变量，以实现全新的功能。

2、修改内核函数

--------------------------------------------------------------------------------
