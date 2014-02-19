
vim 帮助

  打开vim, 输入:help 命令

  中文帮助手册的安装
    1. 下载vimcdoc包
    2. 解包后进入文件夹，sudo ./vimcdoc.sh -i

  切换中英文帮助， set helplang=en  set helplang=cn
  帮助文件的文本是utf-8编码的， set encoding=utf-8

  要查看帮助文件，可能要先重新索引一下：
  : helptags  ~/.vim/doc

vim 常用命令
    
  %     跳转到配对的括号去
  [[    跳转到代码块的开头去(但要求代码块中'{'必须单独占一行)
  gD    跳转到局部变量的定义处
  ''    跳转到光标上次停靠的地方, 是两个'
  mx    设置书签,x只能是a-z的26个字母
  `x    跳转到书签处("`"是1左边的键)
  >     增加缩进,"x>"表示增加以下x行的缩进
  <     减少缩进,"x<"表示减少以下x行的缩进 

语法高亮
  vimrc 加入
    syntax enable
    syntax on
  配色方案       ls /usr/share/vim/vim73/colors/
    colorscheme desert
  语法文件(:help syntax.txt)
    c语言：/usr/share/vim/vim64/syntax/c.vim

---------------------------------------------------------------------------
Ctags 的使用

  使用
    1. 生成tags:  源码根目录 ctags -R 

    一个目录下只有部分文件需要建立 tags ?
    答：使用 find 命令查找文件，bash 做后续处理，生成一个文件列表，
    如 project.files，最后使用 ctags -R -L project.files 命令

    ctags 把.cpp当成c++来处理，.c当成c语言来处理，.h当成C++的头文件处理
    如果你程序中有的.c文件其实是C++程序?  使用 --langmap=c++:+.c
    --force-language=c++   把所有文件当成C++来处理了

    --fields=[+|−]flags     用于指定每条标记的扩展字段域
    --c-kinds=+p            增加 C语言的 tags记录类型, p 函数原型
                            l 局部变量, x 外部变量

    --extra=+q              增加额外的条目: f表示为每个文件增加一个条目，  
                            q为每个类增加一个条目

    2. 将tags文件加入到vim中来: set tags=/path/to/project/tagsname
        在tagsname所在文件夹下，可以不用

  技巧
    在vimrc文件中初始把tags变量为 set tags=tags;  那么 vi会先在当前目录中
    搜索tags文件，如果没有查找到，会到父级目录中查找tags文件，依次类推。

  小瑕疵
      增加了函数定义, 删除了变量定义等, tags文件不能自动rebuild, 
    必须手动再运行一下命令: ctags -R --sort=yes

  :tselect 调出选择菜单
  :tn和:tp 显示后一个tag和前一个tag
---------------------------------------------------------------------------
插件: TagList
  安装  在 ~/.vim 目录下解压taglist_xx.zip
  手册  :help taglist.txt

  ~/.vimrc文件中添加:
    let Tlist_Show_One_File=1
    let Tlist_Exit_OnlyWindow=1

  使用
    打开命令 :Tlist

    将鼠标移到某个tag, 单击左键即可跳转
    ctrl+ww 或双击鼠标 切换窗口
    光标移到某个tag, 按空格键查看完整的写法
    
---------------------------------------------------------------------------
QuickFix 窗口

  手册 :help quickfix

  :make 产生错误

  :cw   // 调出QuickFix窗口
  :cn   // 切换到下一个结果
  :cp   // 切换到上一个结果

  可以在~/.vimrc中增加:
    nmap <F6> :cn<cr>
    nmap <F7> :cp<cr>

--------------------------------------------------------------------------
文件浏览器 -- 插件: NERD_tree
  安装
  手册 :help NERD_tree

  使用：
    打开 : NERDTree
    
    NERD_tree 常用命令(选中文件)：
        t       在标签页中打开
        ente    打开文件
        r       刷新选中目录
        m       显示文件系统菜单（文件/目录的添加、删除、移动操作） 
                 <---- 导航到目的目录，然后按m调出nerd目录，然后选择你要的操作。
        K       到第一个节点
        J       到最后一个节点
        u       打开上层目录
        !       执行此文件

---------------------------------------------------------------------------
Cscope 的使用
  安装  
    1. 下载源码包
    2. ./configure && make && sudo make install 
    3. .vimrc 加入 set cscopequickfix=s-,c-,d-,i-,t-,e-

  手册  :help if_cscop.txt

  使用
    1. 生成一个cscope的数据库: cscope -Rbq
       cscope.in.out
       cscope.out
       cscope.po.out
       用:cs s 检查是否成功

        -i cscope.files

    2. 把刚才生成的cscope文件导入到vim中来
        :cs add /path/to/project/cscope.out /path/to/project/


    add 用法 :cs add {file|dir} [pre-path] [flags]

	    [pre-path] 用来通知 cscope 使用 -P [pre-path] 选项
	    [flags]    可以用来给 cscope 传递额外的选项

    find 用法 cs[cope] find c|d|e|f|g|i|s|t name
        0 或 s  查找本符号(可以跳过注释)
        1 或 g  查找本定义
        2 或 d  查找本函数调用的函数
        3 或 c  查找调用本函数的函数
        4 或 t  查找本字符串
        6 或 e  查找本 egrep 模式
        7 或 f  查找本文件
        8 或 i  查找包含本文件的文件

    ~/.vimrc中
        nmap <C-_>s :cs find s <C-R>=expand("<cword>")<CR><CR>
        nmap <C-_>g :cs find g <C-R>=expand("<cword>")<CR><CR>
        nmap <C-_>c :cs find c <C-R>=expand("<cword>")<CR><CR>
        nmap <C-_>t :cs find t <C-R>=expand("<cword>")<CR><CR>
        nmap <C-_>e :cs find e <C-R>=expand("<cword>")<CR><CR>
        nmap <C-_>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
        nmap <C-_>i :cs find i <C-R>=expand("<cfile>")<CR><CR>
        nmap <C-_>d :cs find d <C-R>=expand("<cword>")<CR><CR>

    <C-_>g的按法是先按"Ctrl+Shift+-", 然后很快再按"g"

----------------------------------------------------------------------------
自动补全
  
  该功能要tags文件的支持, 并且是ctags 5.6版本

  全能补全 :help new-omni-completion

  .vimrc文件加入
    filetype plugin indent on
    打开文件类型检测, 加了这句才可以用智能补全
    set completeopt=longest,menu
    关掉智能补全时的预览窗口
  
加速你的补全 -- 插件: SuperTab

  安装  把supertab.vim 文件丢到 ~/.vim/plugin 文件夹就好了
  手册  supertab.vim 文件头部, 和命令 ":SuperTabHelp"

  .vimrc文件加入
    let g:SuperTabRetainCompletionType=2
    let g:SuperTabDefaultCompletionType="<C-X><C-O>"

  0 - 不记录上次的补全方式
  1 - 记住上次的补全方式,直到用其他的补全命令改变它
  2 - 记住上次的补全方式,直到按ESC退出插入模式为止
  其他的补全方式 :help ins-completion  :help compl-omni

  使用
    按<Tab>就好了

  问题:
    与缩进冲突

onmicppcomplete 插件

----------------------------------------------------------------------------
c/h文件间相互切换 -- 插件: A
  
  安装  将a.vim 放到 ~/.vim/plugin 文件夹中
  手册  无

  :A    在新Buffer中切换到c/h文件
  :AS   横向分割窗口并打开c/h文件
  :AV   纵向分割窗口并打开c/h文件
  :AT   新建一个标签页并打开c/h文件

  我在~/.vimrc中增加了一句:
    nnoremap <silent> <F12> :A<CR>
      意思是按F12时在一个新的buffer中打开c/h文件, 这样在写程序的时候就可以
    不假思索地在c/h文件间进行切换.

---------------------------------------------------------------------------
在工程中查找 -- 插件: Grep

  安装  把grep.vim 文件丢到 ~/.vim/plugin 文件夹就好了
  手册  在grep.vim 文件头部

  我在~/.vimrc中增加了下面这句:
    nnoremap <silent> <F3> :Grep<CR>

----------------------------------------------------------------------------
高亮的书签 -- 插件: VisualMark

  安装  把visualmark.vim 文件丢到 ~/.vim/plugin 文件夹就好了
  手册  无

  vim 有"书签"概念 :help Mark   不好用

  使用
    Ctrl+F2  mm  发现光标所在的行变高亮了
    F2键正向在期间切换, 用Shift+F2反向在期间切换


----------------------------------------------------------------------------
.vimrc 文件
    set ts=4        设置tab有多少空格
    set ai          自动对齐
    set nu
    set mouse=a
    set shiftwidth=4
        shift + .   向左向右缩进
        shift + ,

-------------------------------------------------------------------------
    jklh     上下左右
    
    ,cs     性感的注释
    ,cu     取消注释
    ,cc     注释一行
    ,ca     切换单行注释符

    选中按=　　自动对齐 

    ctrl + ]
    ctrl + o
    ctrl + t

二、VIM编辑器的使用

    vi  vim (增强版vi,现在各版本的linux都带有vim)

    三种模式：
        普通模式：控制光标移动，删除字符，段落复制(按ESC)
        编辑模式：包含插入模式和替换模式，新增文字和修改文字
        命令行模式/末行模式：保存文字，离开vim，以及其它设置

    vim newfile  新建一vim空白文件进行编辑

    普通模式
        编辑模式下按ESC切换为普通模式
        命令        说明

        移动：

        h           将光标向左移动一格
        l           将光标向右移动一格
        j           将光标向下移动一格
        k           将光标向上移动一格
        ^           光标移动到行首,即第一有效字符位置
        0           将光标移动到该行的行首，行的第一格位置
        $           将光标移动到行尾
        H           将光标移动到该屏幕的顶端
        M           将光标移动到该屏幕的中间
        L           将光标移动到该屏幕的底端
        gg          将光标移动到文章的首行
        G           将光标移动到文章的尾行
        w           将光标移动到下一单词
        b           将光标移动到上一单词
        e           将光标移动到下一单词，光标到下一单词词尾
        W           和w功能一样，只不过以.或-或)字符为分界
        B           和b功能一样，只不过以.或-或)字符为分界
        E           和e功能一样，只不过以.或-或)字符为分界
        f           f后跟一字符可直接跳到本行光标之后有此字符位置
        F           F后跟一字符可直接跳到本行光标之前有此字符位置
        t           和f一样功能，只不过光标停在要搜索字符前一字符位置
        T           和T一样功能，只不过光标停在要搜索字符后一字符位置
        zt          将当前位置置于屏幕顶端
        zb          将当前位置置于屏幕底端
        zz          将当前位置置于屏幕中间
        zf          创建折行
        zc          折叠
        zo          打开折叠
        %           跳到匹配的括号
        Ctrl+u      将窗口向上移动半屏
        Ctrl+d      将窗口向下移动半屏
        Ctrl+f      向前滚动一整屏
        ctrl+b      向后滚动一整屏
        ctrl+g      在窗口下方显示信息，告诉当前文件中的位置和文件名等
        Ctrl+o      跳到上次光标位置
        ‘’          跳到上次光标位置，和ctrl+o不同，它只能和上一次光标位置和当前位置来回跳转
        ctrl+i      和ctrl+o功能相反
        m           有名标记，在想要做标记位置按ma， 再到移动其它地方，当想回到做标记位置时,按'a就跳回做标记位置

        简单搜索：
        
        /           /后跟要搜索的关键词
            n       移到下一个找到关键词位置
            /\<the  找以the开头的关键词
            /the\>  找以the结尾的关键词
            /t.e    中间替换任何一个字符搜索关键词
        *           移动关键词按*，会自动高亮，再按n就可查找此关键词

        操作符命令：
       
        X           删除当前光标前一个字符,相当于dh
        u           恢复上一次命令，针对字符
        U           恢复上一次命令，针对行
        ctrl+r      和u相匹配重做
        r           替换当前光标位置字符
        dw          删除一个单词
        D           删除当前光标后面所有字符，相当于d$
        d^          删除当前光标前面所有字符
        d0          删除当前光标前所有的字符，置行首
        dd          删除一行
        c           和d功能一样，只不过删除完会进入插入模式
        yy          选定光标所在行复制
        p           粘贴yy复制的行,贴在光标所在位置之右
        P           粘贴yy复制的行,贴在光标所在位置之左
        cw          删除光标所在的单词并且进入插入状态
        R           进入替换模式
        i           在当前位置进入插入模式
        I           将光标移动到行首进入插入模式
        o           在本行向下新建一行进入插入模式
        O           在本行向上新建一行进入插入模式
        a           将光标之后进入插入模式
        A           将光标移动到行尾进入插入模式
        s           删除光标位置字符进入插入模式,相当于cl
        S           删除光标所在行进入插入模式，相当于cc
        .           重复上一次命令
        V           进入可视模式
        ~           改变当前光标下字符的大小写

        可视模式：

        vjjjjd      选中四个字符删除
        Ctrl+v      可选择列进行操作

        d 和 p 组合使用
            d 删除相当于剪切，并没有完全丢掉，再按p可以粘贴

        交换两个字符
            x和p组合使用


    命令行模式：
        
        :                   进入命令行模式

        filetype plugin indent on
            开启自动识别文件类型，自动用对应文件类型的搭配编辑选项

        map <F5> ihello<ESC>    键盘映射F5插入hello单词

        set number          显示行号
        set nonumber        关闭行号，所有命令前加no为关闭
        set ruler           在右下角显示当前光标位置
        set autoindent      开始新行时对该行施行上一行缩进方式
        set nocompatible    不完全与Vi兼容，所以确保关闭
        set nowrap          不要折行显示
        set list            显示制表符
        syntax enable       打开色彩
        colorscheme evening 切换颜色方案

        set ignorecase      忽略大小写搜索
        set hlsearch        在关键词上按*，自动高亮
        set incsearch       搜索时输入一个字符会自动跳到匹配位置，按回车再确定是否要搜索此关键词
        set nowrapscan      搜索到头就停止，默认搜索到头会到最后一个匹配位置

    
        marks               查看关于标记的列表
        scriptnames         查看vimrc的具体名称和位置
        options             新窗口注释选项列表

        split               横分一新窗口
            3split          指定新分的窗口高度为3
        vsplit              竖分一新窗口
            3vsplit         指定新分窗口宽度为3
            
            Ctrl+w w        窗口之间切换

        !                   执行shell命令
        r!                  执行shell命令并把结果插入到当前光标位置
        o                   后面跟文件路径，可以横分屏打开文件
        O                   后面跟文件路径，可以竖分屏打开文件
        w                   保存
        q                   退出
        qall                全部退出
        wqall               全部保存退出
        qall                全部强制退出

        s/if/hello/         把本行的第一个if替换成hello
        1,$s/if/hello/      把第一行到最后一行的每行的第一个if替换成hello
        1,$s/if/hello/g     把第一行到最后一行的每行的if替换成hello
        :240                快速定位到240行
        
    打开多个文档
        vim -o a.txt b.txt  横屏打开两文件
        vim -O a.txt b.txt  竖屏打开两文件
        vim a.txt +42       打开a.txt光标定到42行

----------------------------------------------------------------------
    :TlistToggle 或 Tlist  打开和关闭，变量函数列表
     按住ctrl键然后按两下w键在正常编辑区域和tags区域中切换。 在tags区域中，把光标移动到变量、函数名称上，然后敲回车，就会自动在正常编辑区域中定位到指定内容了。
     “禁止自动改变当前Vim窗口的大小 let Tlist_Inc_Winwidth=0 
     “把方法列表放在屏幕的右侧 let Tlist_Use_Right_Window=1 
     “多文件时只显示当前文件的tag，其它的tag折叠 let Tlist_File_Fold_Auto_Close=1

-----------------------------------------------------------

折叠: 1）选中，zf
      2）命令，开始行号,结束行号folder
删除折叠： zd
-------------------------------------------------------------------------
多行缩进可以使用: 110,120> 来将110行到120行缩进
-----------------------------------------------------------------------

nt 查看文件树
tl 调taglist
：vertical res[ize] [N]  分屏窗口宽度
