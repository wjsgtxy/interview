# GCC编译

**一个程序能正确编译、链接、运行需要满足3个条件：预处理时能找到头文件，链接时能找到库(静态库或动态库），运行时能找到库。**

对于以压缩包发布的软件，在它的目录下通常都有一个**配置脚本configure**，它的作用确定编译参数(比如头文件位置、连接库位置等)，然后**生成Makefile**以编译程序。可以进入该软件的目录，执行"./configure --help"命令查看使用帮助。



##### GCC编译选项说明官方文档！

https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Invoking-GCC.html#Invoking-GCC



##### 预处理，编译，汇编，链接 分步过程：

```bash
# 预处理：加入头文件，替换宏
gcc HelloWorld.c -E -o HelloWorld.i
# 编译：包含预处理，将C程序转换成汇编程序
gcc HelloWorld.c -S -c -o HelloWorld.s
# 汇编：包含预处理和编译，将汇编程序转换成可链接的二进制程序
gcc HelloWorld.c -c -o HelloWorld.o
# 链接：包含以上所有操作，将可链接的二进制程序和其它别的库链接在一起，形成可执行的程序文件， 没有链接的hello.o 无法执行的!
gcc HelloWorld.c -o HelloWorld
# 查看汇编指令
objdump -d HelloWorld 
```



## 预处理

https://www.cnblogs.com/linuxbo/p/4301716.html

#### GCC 搜索头文件三种策略

- 内定搜索目录。这是编译器自身预设的目录。由于是内定的，一定会搜索，所以总是最后才搜索。

- 手动指定搜索目录。可以在执行配置命令configure前，通过环境变量C_INCLUDE_PATH进行设定；也可以在执行配置命令configure时，通过-I选项进行指定。

- 手动指定不进行搜索的目录。这是在执行配置命令时，使用-nostdin选项进行指定。



##### 手动指定不搜索选项

###### -nostdin C选项

​	使编译器不在系统缺省的头文件目录里面找头文件，一般和 -I 联合使用，明确限定头文件的位置。

###### -nostdin C++选项

​	规定不在 **g++** 指定的标准路经中搜索，但仍在其他路径中搜索。此**选项** 在libg++库使用



#### GCC指定头文件的方式

在程序中，常用两种方法来包含头文件：

```c++
#include <headerfile.h> 
#include "headerfile.h"
```

当#include <headerfile.h> 时，编译时按照  **编译命令指定目录--->系统预设目录--->编译器预设** 的顺序搜索头文件。

当#include "headerfile.h"，编译时按照  **源文件当前目录---->编译命令指定目录--->系统预设目录--->编译器预设**的顺序搜索头文件。



#### **编译命令指定目录**

指定的头文件目录是编译程序时使用**" -I "**选项来指定目录。举个例子：

```bash
# 解决了找不到 <rdma/rdma_cma.h>的问题
gcc -I /home/rdmacore/rdma-core-master/build/include/ yyy.c
```



#### **系统预设目录**

##### 设置预处理头文件搜索目录

https://blog.csdn.net/weixin_44327262/article/details/105860213

系统预设的头文件搜索目录是通过环境变量C_INCLUDE_PATH来设置的，这个变量的值要在执行配置命令configure之前设置。

> `C_INCLUDE_PATH`、`CPLUS_INCLUDE_PATH`以及`CPATH`常被用于在全局性地添加预处理C/C++时的包含目录，其中`C_INCLUDE_PATH`仅对预处理C有效，`CPLUS_INCLUDE_PATH`仅对预处理C++有效，而`CPATH`对所有语言均有效。

```bash
# 删除环境变量
unset C_INCLUDE_PATH
# 查看当前值
echo $C_INCLUDE_PATH
# 在当前session中设置或者直接到 ~/.bashrc中设置
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/home/rdmacore/rdma-core-master/build/include/

# 2022年7月5日17:16:09 scidb rdma 我的rdma core没有build
export CPATH=$CPATH:/home/rdma/rdma-core-master/build/include/
```



注意：如果C_INCLUDE_PATH一开始为空，上面这么设置，会有潜在的问题。因为有个空，然后跟着冒号 ‘:’，会把当前目录添加进去。所以如果C_INCLUDE_PATH为空就直接设置值比较好。

> In all these variables, an empty element instructs the compiler to search its current working directory. Ｅmpty elements can appear at the beginning or end of a path. For instance, if the value of CPATH is :/special/include, that has the same effect as -I. -I/special/include.



#### 编译器预设目录

编译器预设目录由编译器自己决定的，由程序代码决定的，这是不需要工人设置或指定的。

```bash
# 查看gcc预处理C时搜索目录(ubuntu20)
echo | gcc -x c -v -E -
...
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include # 这个目录里面放的是我之前学习cmake后install后将MathFunctions.h头文件放到了这里
 /usr/include/x86_64-linux-gnu
 /usr/include # 大部分头文件就在这里 rdma的目录也在这里

# 查看GCC预处理C++时搜索目录(ubuntu14)
echo | gcc -x c++ -v -E -
...
#include <...> search starts here:
 /usr/include/c++/5
 /usr/include/x86_64-linux-gnu/c++/5
 /usr/include/c++/5/backward
 /usr/lib/gcc/x86_64-linux-gnu/5/include
 /usr/local/include
 /usr/lib/gcc/x86_64-linux-gnu/5/include-fixed
 /usr/include/x86_64-linux-gnu
 /usr/include
```



#### 自己遇到的问题

rdma 头文件目录：

```bash
/usr/include/rdma # 里面有
/usr/include/valgrind # 里面有memcheck.h
```

/usr/include/rdma目录里面的和我自己编译的rdmacore-master/build/include/rdma 相比，少了3个头文件，少了rdma_cma.h等头文件，所以我之前编译demo的时候，**才一直提示找不到rdma_cma.h**。

但是这个目录里面又有rdma_netlink.h和rdma_user_cm.h，现在我发现这两个文件都是链接到 **kernel-headers**目录里面的。rdma_cma.h是链接到**librdmacm**目录里面的，因为是用户态的，所以这个文件就没有直接包含在系统里面吧。

```bash
root@xyvm2:/home/rdmacore/rdma-core-master/build/include/rdma# ls
...
rdma_cma.h -> /home/rdmacore/rdma-core-master/librdmacm/rdma_cma.h
rdma_netlink.h -> /home/rdmacore/rdma-core-master/kernel-headers/rdma/rdma_netlink.h
rdma_user_cm.h -> /home/rdmacore/rdma-core-master/kernel-headers/rdma/rdma_user_cm.h
```

```
# cp的-L可以将软链接指向的文件复制过来
cp -rL /src /dst 
```



#### 头文件的查找路径及优先顺序总结

1. 如果源文件中使用双引号来包含头文件，则首先在源文件当前目录查找头文件。 
2. 如果编译时使用"-I/some/dir"，则在/some/dir中查找。
3. 如果设置了环境变量C_INCLUDE_PATH，则在指定的目录中查找。
4. 最后在编译器预设的路径中查找，这是不需要指定的，编译时一定会在该路径中搜索所需的头文件。

所以，编译程序时如果出现了找不到头文件的错误，可以通过设置C_INCLUDE_PATH或给编译器设置"-I"选线来指定头文件目录。

可以在执行配置命令configure之前设置C_INCLUDE_PATH或CFLAGS，如果不设置CFLAGS，它的默认值为"-g -O2"

```bash
export C_INCLUDE_PATH="/some/dir/1:/some/dir/2"
export CFLAGS = "-g -O2 -I/some/dir" #如果设置了C_INCLUDE_PATH，就可以不设置CFLAGS
./configure
```



## 链接

[动态链接库的查找先后顺序](https://zhuanlan.zhihu.com/p/235551437)

- `LD_LIBRARY_PATH`环境变量中的路径
- `/etc/ld.so.cache`缓存文件
- `/usr/lib`和`/lib`



##### 设置LD_LIBRARY_PATH

```bash
# 查看
echo ${LD_LIBRARY_PATH}
# 添加环境变量
export LD_LIBRARY_PATH=/home/rdmacore/rdma-core-master/build/lib/:$LD_LIBRARY_PATH
# 清空环境变量
unset LD_LIBRARY_PATH
```

除了`LD_LIBRARY_PATH`环境变量外，还有一个`LD_PRELOAD`环境变量。`LD_PRELOAD`的查找顺序比`LD_LIBRARY_PATH`还要优先。`LD_PRELOAD`里是具体的目标文件列表（A list of shared objects）；`LD_LIBRARY_PATH`是目录列表（A list of directories）。



##### /usr/bin/ld 搜索路径顺序

##### 一. **静态库链接**时搜索路径顺序 

1. ld会去找GCC命令中的参数-L
2. 再找GCC的环境变量LIBRARY_PATH
3. 再找内定目录 /lib， /usr/lib，  /usr/local/lib 这是当初compile gcc时写在程序内的 



##### 二. 动态链接时、执行时搜索路径顺序

1. 编译目标代码时指定的动态库搜索路径(-L选项)
2. 环境变量LD_LIBRARY_PATH指定的动态库搜索路径
3. 配置文件/etc/ld.so.conf中指定的动态库搜索路径
4. 默认的动态库搜索路径/lib 
5. 默认的动态库搜索路径/usr/lib



**编译时直接指定库的路径: GCC -L **

https://www.cnblogs.com/benio/archive/2010/10/25/1860394.html

```bash
gcc -lrt  -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm  devices.o -o devices
```



##### 环境变量指定库搜索路径

```bash
# LIBRARY_PATH：指定程序 静态链接库文件 搜索路径
echo ${LIBRARY_PATH}
# LD_LIBRARY_PATH：指定程序 动态链接库文件 搜索路径
echo ${LD_LIBRARY_PATH} 

# 添加环境变量，终于解决了 /usr/bin/ld: cannot find -lrdmacm 的问题!!!
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/rdmacore/rdma-core-master/build/lib/
# export LD_LIBRARY_PATH=/home/rdmacore/rdma-core-master/build/lib/
```

dz: 静态库只是链接ld的时候起作用，运行时没作用，动态库链接的时候要去拿符号表，运行时也要用



#### 查看链接某个库的搜索路径

问题：ld: cannot find -lrdmacm

https://stackoverflow.com/questions/16710047/usr-bin-ld-cannot-find-lnameofthelibrary

```bash
# 查看rdmacm库的搜索路径
ld -lrdmacm --verbose
=====================================================================
...
SEARCH_DIR("=/usr/local/lib/x86_64-linux-gnu"); SEARCH_DIR("=/lib/x86_64-linux-gnu"); SEARCH_DIR("=/usr/lib/x86_64-linux-gnu"); SEARCH_DIR("=/usr/lib/x86_64-linux-gnu64"); SEARCH_DIR("=/usr/local/lib64"); SEARCH_DIR("=/lib64"); SEARCH_DIR("=/usr/lib64"); SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib"); SEARCH_DIR("=/usr/x86_64-linux-gnu/lib64"); SEARCH_DIR("=/usr/x86_64-linux-gnu/lib");
...
=====================================================================
attempt to open /usr/local/lib/x86_64-linux-gnu/librdmacm.so failed
attempt to open /usr/local/lib/x86_64-linux-gnu/librdmacm.a failed
attempt to open /lib/x86_64-linux-gnu/librdmacm.so failed
attempt to open /lib/x86_64-linux-gnu/librdmacm.a failed
attempt to open /usr/lib/x86_64-linux-gnu/librdmacm.so failed
attempt to open /usr/lib/x86_64-linux-gnu/librdmacm.a failed
attempt to open /usr/lib/x86_64-linux-gnu64/librdmacm.so failed
attempt to open /usr/lib/x86_64-linux-gnu64/librdmacm.a failed
attempt to open /usr/local/lib64/librdmacm.so failed
attempt to open /usr/local/lib64/librdmacm.a failed
attempt to open /lib64/librdmacm.so failed
attempt to open /lib64/librdmacm.a failed
attempt to open /usr/lib64/librdmacm.so failed
attempt to open /usr/lib64/librdmacm.a failed
attempt to open /usr/local/lib/librdmacm.so failed
attempt to open /usr/local/lib/librdmacm.a failed
attempt to open /lib/librdmacm.so failed
attempt to open /lib/librdmacm.a failed
attempt to open /usr/lib/librdmacm.so failed
attempt to open /usr/lib/librdmacm.a failed
attempt to open /usr/x86_64-linux-gnu/lib64/librdmacm.so failed
attempt to open /usr/x86_64-linux-gnu/lib64/librdmacm.a failed
attempt to open /usr/x86_64-linux-gnu/lib/librdmacm.so failed
attempt to open /usr/x86_64-linux-gnu/lib/librdmacm.a failed
ld: cannot find -lrdmacm
```

我发现它找的都是librdmacm.so或者 librdmacm.o 但是我 /usr/lib/x86_64-linux-gnu目录里面的，就只有两个

```bash
lrwxrwxrwx 1 root root     21 Feb 26  2020 librdmacm.so.1 -> librdmacm.so.1.2.28.0
-rw-r--r-- 1 root root 117920 Feb 26  2020 librdmacm.so.1.2.28.0
```

并没有直接 so结尾的，只有so.1，添加一个.so的软连接：

```bash
sudo ln -s /usr/lib/x86_64-linux-gnu/librdmacm.so.1.2.28.0 /usr/lib/x86_64-linux-gnu/librdmacm.so
```

终于成功了，链接的时候，不用添加 -L路径了！



~~LD_LIBRARY_PATH这玩意是运行时候用的，链接的时候老老实实的用 -L添加路径~~

这个还是有用的，之前觉得没用是因为我的标准搜索路径里面没有 .so的文件，只有so.xxx具体的文件，所以编译的时候没有找到这个库文件

**所以，记住ld只会找libxxx.so。**



##### 链接技巧：

手工来写链接参数总是很麻烦的，还好很多库开发包提供了生成链接参数的程序，名字一般叫xxxx-config，一般放在/usr/bin目录下，比如gtk1.2的链接参数生成程序是gtk-config，执行gtk-config --libs就能得到以下输出"-L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm"，这就是编译一个gtk1.2程序所需的gtk链接参数，xxx-config除了--libs参数外还有一个参数是--cflags用来生成头文件包含目录的，也就是-I参数，在下面我们将会讲到。你可以试试执行gtk-config --libs --cflags，看看输出结果。

现在的问题就是怎样用这些输出结果了，最笨的方法就是复制粘贴或者照抄，聪明的办法是在编译命令行里加入这个\`xxxx-config --libs --cflags\`，比如编译一个gtk程序：

```bash
gcc gtktest.c `gtk-config --libs --cflags`
```

注意`不是单引号，而是1键左边那个键。



#### [pkg-config原理及用法](https://www.cnblogs.com/sddai/p/10266624.html)

我们在用第三方库的时候，经常会用到pkg-config这个东西来编译程序。那pkg-config究竟是什么呢？本博文简单地说一下。

##### pkgconfig有什么用：

大家应该都知道用第三方库，就少不了要使用到第三方的头文件和库文件。我们在编译、链接的时候，必须要指定这些头文件和库文件的位置。

对于一个比较大第三方库，其头文件和库文件的数量是比较多的。如果我们一个个手动地写，那将是相当麻烦的。所以，pkg-config就应运而生了。pkg-config能够把这些头文件和库文件的位置指出来，给编译器使用。如果你的系统装有gtk，可以尝试一下下面的命令

```bash
$pkg-config --cflags gtk+-2.0
```

可以看到其输出是gtk的头文件的路径。

我们平常都是这样用pkg-config的：

```bash
$gcc main.c `pkg-config --cflags --libs gtk+-2.0` -o main
```

上面的编译命令中，pkg-config命令把gtk的头文件路径和库文件列出来，让编译去获取。--cflags和--libs分别指定头文件和库文件。

其实，pkg-config同其他命令一样，有很多选项，不过我们一般只会用到--libs和--cflags选项。更多的选项可以在[这里](http://linux.die.net/man/1/pkg-config)查看。

 

##### 配置环境变量：

如何将pkg-config用于自己的库：

首先要明确一点，因为pkg-config也只是一个命令，所以不是你安装了一个第三方的库，pkg-config就能知道第三方库的头文件和库文件所在的位置。pkg-config命令是通过查询XXX.pc文件而知道这些的。我们所需要做的是，写一个属于自己的库的.pc文件。

但pkg-config又是如何找到所需的.pc文件呢？这就需要用到一个环境变量PKG_CONFIG_PATH了。这环境变量写明.pc文件的路径，pkg-config命令会读取这个环境变量的内容，这样就知道pc文件了。

对于Ubuntu系统，可以用root权限打开/etc/bash.bashrc文件。在最后输入下面的内容。

```bash
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
```

这样，pkg-config就会去/usr/local/lib/pkgconfig目录下，寻找.pc文件了。如果不是Ubuntu系统，那就没有/etc/bash.bashrc文件，可以参考我的一篇[博文](http://blog.csdn.net/luotuo44/article/details/8917764)，来找到一个合适的文件，修改这个环境变量。输入bash命令使得配置生效。

现在pkg-config能找到我们的.pc文件。但如果有多个.pc文件，那么pkg-config又怎么能正确找到我想要的那个呢？这就需要我们在使用pkg-config命令的时候去指定。比如:

```bash
$gcc main.c `pkg-config --cflags --libs gtk+-2.0` -o main
```

就指定了要查找的.pc文件是gtk+-2.0.pc。又比如，有第三方库OpenCV，而且其对应的pc文件为opencv.pc，那么我们在使用的时候，就要这样写

```bash
pkg-config --cflags --libs opencv
```

这样，pkg-config才会去找opencv.pc文件。

 

##### pc文件书写规范：

好了，现在我们开始写自己的.pc文件。只需写5个内容即可：Name、Description、Version、Cflags、Libs。

比如简单的：

```bash
Name: opencv
Description:OpenCV pc file
Version: 2.4
Cflags:-I/usr/local/include
Libs:-L/usr/local/lib –lxxx –lxxx
```

其中Name对应的内容要和这个pc文件的文件名一致。当然为了书写方便还会加入一些变量，比如前缀变量prefix。下面有一个更完整的pc文件的内容

​    ![img](https://img-blog.csdn.net/20140501110745640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvdHVvNDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

​    其中，Cflags和Libs的写法，是使用了-I -L -l这些gcc的编译选项。原理可以参考我的一篇[博文](http://blog.csdn.net/luotuo44/article/details/16970841)。



#### 生成动态链接库：

```bash
$gcc -shared -Wl,-soname,libhello.so.1 -o libhello.so.1.0 hello.o # -Wl是传递后面参数给ld的
```

另外再建立两个符号连接：

```bash
$ln -s libhello.so.1.0 libhello.so.1
$ln -s libhello.so.1 libhello.so
```

这样1个libhello的动态连接库就生成了。最重要的是传gcc -shared 参数使其生成是动态库而不是普通履行程序。 -Wl 表示后面的参数也就是-soname，libhello.so.1直接传给连接器ld进行处理。实际上，每个库都有1个soname，当连接器发现它正在查找的程序库中有这样1个名称，连接器便会将soname嵌入连结中的2进制文件内，而不是它正在运行的实际文件名，在程序执行期间，程序会查找具有 soname名字的文件，而不是库的文件名，换句话说，soname是库的辨别标志。 这样做的目的主要是允许系统中多个版本的库文件共存，习惯上在命名库文件的时候通常与soname相同 libxxxx.so.major.minor 其中，xxxx是库的名字，major是主版本号，minor 是次版本号



#### 静态库，动态库，头文件区别：

https://blog.csdn.net/weixin_42458272/article/details/106193786

静态库，动态库：

https://blog.csdn.net/qq_22122811/article/details/78226835

linux默认的头文件在 /usr/include里面，比如 stdio.h stdlib.h string.h

网络编程的在/usr/include/netinet里面





## 问题

#### "undefined reference to XXX"问题总结

好文章！！全文收藏起来！！

https://zhuanlan.zhihu.com/p/81681440

https://www.cnblogs.com/quant-lee/p/6639008.html

从知乎的这个链接的case2，解决了我“undefined reference to rdma_get_devices”的部分问题：

```bash
# 之前是这样的命令，一直报错 undefined reference to 'rdma_get_devices', undefined ...to 'ibv_query_device', undefined ... to 'rdma_free_deivecs'
gcc -lrt  -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm  devices.o -o devices

# 目标文件之间有依赖关系，A依赖B， A要放在前面才行，把devices.o放到前面
gcc devices.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -o devices

# 上面之后报了另外的错误：undefined reference to symbol 'ibv_query_devices@@IBVERBS_1.1' /usr/bin/ld: libibverbs.so.1: error adding symbols:DSO missing from command line

# 我将 ibibvers.so这个库添加上就OK了，终于编译成功了！！！
gcc devices.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -libverbs -o devices
```



注意，上面的报错，其实提示很明显了：**’DSO missing from command line‘**

https://blog.csdn.net/zrq293/article/details/105969423/

问题可以简单描述为：当链接可执行文件时，依赖于libA.so，而libA.so又依赖于libB.so，而且可执行文件中还直接调用了libB.so中的函数，那么此时链接就会出现错误，解决办法：

```bash
# 添加--copy-dt-needed-entries，链接器自己会去加载lrdmacm需要的库文件
LIBS = -lrt -Wl,--copy-dt-needed-entries -L/usr/lib/rdma/ -lrdmacm
```

其他：

```bash
# 查看so文件依赖了哪些其他的库
ldd librdmacm.so
```

查看so文件是64位还是32位：

```bash
file librdmacm.so.1.3.39.0
# 输出
librdmacm.so.1.3.39.0: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[sha1]=4fc4259f149d1fcdb1b7c215b662bf4fb8fe5ef9, with debug_info, not stripped
# 也可以使用 readelf -h <file> 输出格式更好看
readelf -h librdmacm.so.1.3.39.0
```





#### 库的链接顺序问题

https://www.cnblogs.com/ironx/p/4939508.html

如果你的库在链接时安排的顺序是：foo.o -lz bar.o。那么gcc的链接器先搜索库foo，然后是z库，然后是bar库。

这样就带来一个问题，如果库bar调用了库z里面的函数，但是链接器是先搜索的库z，这时候并没有发现库bar调用库z啊，所以就不会把库z中的这部分函数体挑出来进行链接。

而是只把库z中，被foo库调用的函数体挑出来。



#### 运行时库问题

添加so文件路径到共享库：

```bash
# 编辑ld.so.conf文件
vi /etc/ld.so.conf
# 添加一行
/home/link/testso （注意：include是用来添加一个文件夹用的，只添加一个目录的话，直接在这个文件最下面append一行即可）
# 刷新ld.so.cache
ldconfig
```

注意：配置了/etc/ld.so.conf 运行ldconfig 编译仍找不到动态库

**ld.so是动态库载入器而不是连接器，某些翻译“动态连接器”有问题的，或者说英文原文就有问题**

**换句话说，配置ld.so.conf的作用就是如果so在非标准路径，可执行文件能够找到，这是运行期做的事情**

**你现在是编译，ld.so根本不起作用，所有用非标准路径的话必需手动指定库目录**



```bash
# 查看系统是否有加载该库
ldconfig -v |grep "librdmacm" 
# 输出
librdmacm.so.1 -> librdmacm.so.1.2.28.0
```

在我把/etc/ld.so.conf中的路径删除之前，这里查询会有两个输出，另外一个librdmacm指向so.1.3.39那个，那个是在我编译的rdmacore中lib目录下的那个

**ldconfig做的这些东西都与运行程序时有关，跟编译时一点关系都没有。编译的时候还是该加-L就得加，不要混淆了!!!**



```bash
# 我把rdmacore中lib目录整体复制到 /usr/lib/rdma里面去了，这样不用每次编译都指定 -L参数了，但是仍然提示找不到 -lrdmacm
cp -r /home/rdmacore/rdma-core-master/build/lib/ /usr/lib/rdma/
gcc -lrt -lrdmacm client.o utilities.o -o client

# 还是要加-L，这个目录有 librdmacm.so.1.3.39.0
gcc -lrt -L/usr/lib/rdma/ -lrdmacm client.o utilities.o -o client

# 这样也是提示找不到 -lrdmacm，但是这个目录里面有 librdmacm.so.1.2.28.0
gcc -lrt -L/usr/lib/x86_64-linux-gnu -lrdmacm client.o utilities.o -o client
```

#### TODO 我觉得上面的都是因为没有.so文件，只有.so.xxx文件



#### 编译参数中-pthread以及-lpthread的区别

一般情况下，我们在链接一个（文件名为libxxx.so或libxxx.a等的）库时，会使用-lxxx的方式；在Linux中要用到[多线程](https://so.csdn.net/so/search?q=%E5%A4%9A%E7%BA%BF%E7%A8%8B&spm=1001.2101.3001.7020)时，需要链接pthread库，按照惯例，我们应该使用-lpthread的方式来进行链接；但是，通过日常的观察，我发现很多开源代码都是使用了-pthread参数，而非使用-lpthread，这是为什么呢？

一通Google后，总算找到一些线索：

1. 为了可移植性：在Linux中，pthread是作为一个单独的库存在的（libpthread.so），但是在其他Unix变种中却不一定，比如在FreeBSD中是没有单独的pthread库的，因此在FreeBSD中不能使用-lpthread来链接pthread，而使用-pthread则不会存在这个问题，因为FreeBSD的编译器能正确将-pthread展开为该系统下的依赖参数。同样道理，其他不同的变种也会有这样那样的区别，如果使用-lpthread，则可能在移植到其他Unix变种中时会出现问题，为了保持较高的可移植性，我们最好还是使用-pthread（尽管这种做法未被接纳成为C标准，但已基本是事实标准）。
2. 添加额外的标志：在多数系统中，-pthread会被展开为“-D_REENTRANT -lpthread”，即是除了链接pthread库外，还先定义了宏_REENTRANT。定义这个宏的目的，是为了打开系统头文件中的各种多线程支持分支。比如，我们常常使用的错误码标志errno，如果没有定义_REENTRANT，则实现为一个全局变量；若是定义了_REENTRANT，则会实现为每线程独有，从而避免线程竞争错误。



### **feature_test_macros**

编译ping_rdma_1失败，提示在rdma/arch.h 中找不到ntohll/htonll 底层使用的 betoh64/htobe64，而且这个文件也被提示不建议使用，我就把这个arch.h头文件去掉了，直接使用 <endian.h> (在 /usr/include中就有这个头文件)，这个头文件中根据本地大小端的情况来定义 betoh64和htobe64等函数。但是之后编译链接的时候，提示undefined reference to 'betoh64'

之后百度，发现有个人和我一样的问题：

https://www.itdaan.com/blog/2013/10/22/721ec93f924c45bdfc2b33a6929af39d.html

他编译的时候也写了 -std=c99 选项就编译不成功，和我一样的提示，但是去掉了之后就OK了，他不知道这是什么原因，下面有个回答：

```
#define _DEFAULT_SOURCE
```

我在头文件中添加了这个宏之后，再编译就OK了

试了下，只添加下面这个也是OK的：

```
#define _BSD_SOURCE
#define __USE_BSD
```



> Without explicit `-std=` option, calling `gcc` is the same as `-std=gnu89` with means C89 + GNU extensions. The GNU extensions will enable macros which will enable the presence of the functions in your header.



另外，作者说他 只需删除gcc 中的 -std=c99就可以了，我删除了 源代码中的 #define _ISOC99_SOURCE ，好像不行。

在linux be64toh的man page(https://linux.die.net/man/3/be64toh)中，开头也直接定义了bsd这个宏，同时下面的‘Conforming To’ 兼容性一致性说明里面，也说这个不是一个标准函数，在bsd系统里面，用的是<sys/endian.h>头文件而不是我用的<endian.h>， 我换成 上面sys这个就会提示找不到头文件了。NetBSD， FreeBSD and glibc 没有遵守OpenBSD的命名，OpenBSD中是 'betoh32'， 其他几个是 ’be32toh'.

```c++
#define _BSD_SOURCE
```



#### GCC 编译标准-std=的设置方法

1) 最初的 ANSI C 标准 (X3.159-1989) 在 1989 年被批准，并于 1990 年发布。稍后这个标准被接受为 ISO 标准 (ISO/IEC 9899:1990) 。虽然 ISO 标准将 ANSI 标准的某些章节重新编号并变为条款，但是两者实际上并无区别。不论是 ANSI 标准还是 ISO 标准，通常都称之为 C89 ，偶尔也因为发布日期而被叫做 C90 。 ANSI 标准 ( 非 ISO 标准 ) 同时附带了 rationale 文档。可以使用 -ansi ， -std=c89 或 -std=iso9899:1990 选项指定 [GCC](https://so.csdn.net/so/search?q=GCC&spm=1001.2101.3001.7020) 使用 C89 标准；可以使用 -pedantic 选项来得到所有的诊断信息( 或者使用 -pedantic-errors 选项以使 wangning 变为 error) 。PS：pedantic adj. 1. 卖弄学问的 2. 学究式的，迂腐的

2) 新的 ISO C 标准是 1999 年发布的 ISO/IEC 9899:1999 ，通常称之为 C99 。 GCC 目前不完整的支持这个版本。详情请参考 <http://gcc.gnu.org/gcc-4.4/c99status.html> 。为了指定 GCC 使用这个版本的 C 标准，需要 -std=c99 或 -std=iso9899:1999 选项。

3) 默认情况下， GCC 提供了一些 C 语言的扩展，极少的几处会与 C 标准冲突。关于这些冲突请参考 “ C 语言家族的扩展 ” 一节。使用上述的 -std 选项将会关闭这些有冲突的扩展。你也可以显式的使用选项 -std=gnu89 ( 对应 C89 的 GNU 扩展 ) 或 -std=gnu99 ( 对应 C99 的 GNU 扩展 ) 来选择对应版本的扩展功能。如果没有给出 C 语言 “ 方言 ” 选项，将默认的使用 -std=gnu89 ；若要使用C99的特性要设置-std=gnu9x。

4. -std= 选择C语言编译标准

```c++
-std=
A value for this option must be provided; possible values are

'c90''c89''iso9899:1990'
	Support all ISO C90 programs (certain GNU extensions that conflict with ISO C90 are disabled). Same as -ansi for C code.

'iso9899:199409'
	ISO C90 as modified in amendment 1.

'c99''c9x''iso9899:1999''iso9899:199x'
	ISO C99. Note that this standard is not yet fully supported; see http://gcc.gnu.org/c99status.html for more information. The names 'c9x'and 'iso9899:199x'are deprecated.

'c1x'
	ISO C1X, the draft of the next revision of the ISO C standard. Support is limited and experimental and features enabled by this option may be changed or removed if changed in or removed from the standard draft.

'gnu90''gnu89'
	GNU dialect of ISO C90 (including some C99 features). This is the default for C code.

'gnu99''gnu9x'
	GNU dialect of ISO C99. When ISO C99 is fully implemented in GCC, this will become the default. The name 'gnu9x'is deprecated.

'gnu1x'
	GNU dialect of ISO C1X. Support is limited and experimental and features enabled by this option may be changed or removed if changed in or removed from the standard draft.

'c++98'
	The 1998 ISO C++ standard plus amendments. Same as -ansi for C++ code.

'gnu++98'
	GNU dialect of -std=c++98. This is the default for C++ code.

'c++0x'
	The working draft of the upcoming ISO C++0x standard. This option enables experimental features that are likely to be included in C++0x. The working draft is constantly changing, and any feature that is enabled by this flag may be removed from future versions of GCC if it is not part of the C++0x standard.

'gnu++0x'
	GNU dialect of -std=c++0x. This option enables experimental features that may be removed in future versions of GCC.
```



###### BSD

BSD ([Berkeley](https://baike.baidu.com/item/Berkeley) [Software](https://baike.baidu.com/item/Software) [Distribution](https://baike.baidu.com/item/Distribution)，[伯克利](https://baike.baidu.com/item/%E4%BC%AF%E5%85%8B%E5%88%A9/25375)软件套件)是[Unix](https://baike.baidu.com/item/Unix/219943)的衍生系统，在1977至1995年间由[加州大学伯克利分校](https://baike.baidu.com/item/%E5%8A%A0%E5%B7%9E%E5%A4%A7%E5%AD%A6%E4%BC%AF%E5%85%8B%E5%88%A9%E5%88%86%E6%A0%A1/3755024)开发和发布的。历史上， BSD曾经被认为是[UNIX](https://baike.baidu.com/item/UNIX)的一支——"BSD UNIX"， 因为它和[AT&T](https://baike.baidu.com/item/AT%26T) UNIX操作系统共享基础代码和设计。在20世纪80年代，衍生出了许多变形的UNIX授权软件。比较著名的如[DEC](https://baike.baidu.com/item/DEC)的Ultrix及Sun公司的[SunOS](https://baike.baidu.com/item/SunOS)。1990年代，BSD很大程度上被System V4.x版以及OSF/1系统所取代，晚期BSD版本为几个开源软件开发提供了平台并且一直沿用。，“BSD”并不特指任何一个BSD衍生版本，而是类UNIX操作系统中的一个分支的总称。



#### Features.h

这个文件在/usr/include下面，这个文件定义了很多宏，用来设置编译环境，包括:

```c++
__strict_ansi // iso standard c
_isoc99_source // extensions to iso c89 from iso c99
_isoc11_source // extensions to iso c99 from iso c11
_isoc2x_source // extensions to iso c99 from iso c2x
_posix_source // IEEE std 1003.1
_posix_c_source // 1, 2, 199309L, 199506L, 200112L, 200809L
_xopen_source // include posix and xpg things. 500/600/700
...
_gnu_source // 设置了这个，就相当于设置了上面所有
_default_source // 啥都没设置，默认是这个
    
// 还有提示说 不建议使用 _bsd_source 和 _svid_source, 建议使用 _default_source, 源代码如下：
185 #if (defined _BSD_SOURCE || defined _SVID_SOURCE) \
186     && !defined _DEFAULT_SOURCE
187 # warning "_BSD_SOURCE and _SVID_SOURCE are deprecated, use _DEFAULT_SOURCE"
188 # undef  _DEFAULT_SOURCE
189 # define _DEFAULT_SOURCE        1
190 #endif

// 都没定义，则定义 _DEFAULT_SOURCE
/* If nothing (other than _GNU_SOURCE and _DEFAULT_SOURCE) is defined,
219    define _DEFAULT_SOURCE.  */

```

帮助页面也有详细说明：https://linux.die.net/man/7/feature_test_macros （这个页面感觉旧，看下面的gnu）

GNU的说明页面：https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html 部分内容如下：

```c++
Macro: _POSIX_SOURCE
If you define this macro, then the functionality from the POSIX.1 standard (IEEE Standard 1003.1) is available, as well as all of the ISO C facilities.

The state of _POSIX_SOURCE is irrelevant if you define the macro _POSIX_C_SOURCE to a positive integer.

Macro: _GNU_SOURCE
If you define this macro, everything is included: ISO C, POSIX.1, POSIX.2, BSD, SVID, X/Open, and GNU extensions. 'In the cases where POSIX.1 conflicts with BSD, the POSIX definitions take precedence.'

If you want to get the full effect of _GNU_SOURCE but make the BSD definitions take precedence over the POSIX definitions, use this sequence of definitions:

#define _GNU_SOURCE
#define _BSD_SOURCE
#define _SVID_SOURCE

Note that if you do this, you must link your program with the BSD compatibility library by passing the `-lbsd-compat' option to the compiler or linker. Note: If you forget to do this, you may get very sange errors at run time.

// 建议
'We recommend you use _GNU_SOURCE in new programs. If you don’t specify the ‘-ansi’ option to GCC, or other conformance options such as -std=c99, and don’t define any of these macros explicitly, the effect is the same as defining _DEFAULT_SOURCE to 1.'
    
'When you define a feature test macro to request a larger class of features, it is harmless to define in addition a feature test macro for a subset of those features. For example, if you define _POSIX_C_SOURCE, then defining _POSIX_SOURCE as well has no effect. Likewise, if you define _GNU_SOURCE, then defining either _POSIX_SOURCE or _POSIX_C_SOURCE as well has no effect.'

```

GNU文档，值得一看： https://www.gnu.org/software/libc/manual/html_node/



https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Code-Gen-Options.html#Code-Gen-Options

-fpic
Generate position-independent code (PIC) suitable for use in a shared library, if supported for the target machine. Such code accesses all constant addresses through a global offset table (**GOT**). The dynamic loader resolves the GOT entries when the program starts (the dynamic loader is not part of GCC; it is part of the operating system). If the GOT size for the linked executable exceeds a machine-specific maximum size, you get an error message from the linker indicating that -fpic does not work; in that case, recompile with -fPIC instead. (These maximums are 8k on the SPARC and 32k on the m68k and RS/6000. The x86 has no such limit.)

Position-independent code requires special support, and therefore works only on certain machines. For the x86, GCC supports PIC for System V but not for the Sun 386i. Code generated for the IBM RS/6000 is always position-independent.

When this flag is set, the macros __pic__ and __PIC__ are defined to 1.

-fPIC
If supported for the target machine, emit position-independent code, suitable for dynamic linking and avoiding any limit on the size of the global offset table. This option makes a difference on the m68k, PowerPC and SPARC.

Position-independent code requires special support, and therefore works only on certain machines.

When this flag is set, the macros __pic__ and __PIC__ are defined to 2.

-fpie
-fPIE
These options are similar to -fpic and -fPIC, but generated position independent code **can be only linked into executables.** Usually these options are used when -pie GCC option is used during linking.

-fpie and -fPIE both define the macros __pie__ and __PIE__. The macros have the value 1 for -fpie and 2 for -fPIE.





CC 与 CXX 这是 C 与 C++ 编译器命令。默认值一般是 “gcc” 与 “g++”。 CFLAGS 与 CXXFLAGS CFLAGS 表示用于 C 编译器的选项，CXXFLAGS 表示用于 C++ 编译器的选项。这两个变量实际上涵盖了编译和汇编两个步骤。大多数程序和库在编译时默认的优化级别是”2″(使用”-O2″选项)并且带有调试符号来编 译，也就是 CFLAGS=”-O2 -g”, CXXFLAGS=$CFLAGS 。事实上，”-O2″已经启用绝大多数安全的优化选项了。

## GCC编译选项CFLAGS参数

| 选项  | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| -c    | 用于把源码文件编译成 .o 对象文件,不进行链接过程              |
| -o    | 用于连接生成可执行文件，在其后可以指定输出文件的名称         |
| -g    | 用于在生成的目标可执行文件中，添加调试信息，可以使用GDB进行调试 |
| -Idir | 用于把新目录添加到include路径上，可以使用相对和绝对路径，“-I.”、“-I./include”、“-I/opt/include” |
| -Wall | 生成常见的所有告警信息，且停止编译，具体是哪些告警信息，请参见GCC手册，一般用这个足矣！ |
| -w    | 关闭所有告警信息                                             |
| -O    | 表示编译优化选项，其后可跟优化等级0\1\2\3，默认是0，不优化   |
| -fPIC | 用于生成位置无关的代码                                       |
| -v    | (在标准错误)显示执行编译阶段的命令，同时显示编译器驱动程序,预处理器,编译器的版本号 |

## GCC链接选项LDFLAGS参数

| 选项       | 说明                                                         |
| ---------- | ------------------------------------------------------------ |
| -llibrary  | 链接时在标准搜索目录中寻找库文件，搜索名为`liblibrary.a` 或 `liblibrary.so` |
| -Ldir      | 用于把新目录添加到库搜索路径上，可以使用相对和绝对路径，“-L.”、“-L./include”、“-L/opt/include” |
| -Wl,option | 把选项 option 传递给连接器，如果 option 中含有逗号,就在逗号处分割成多个选项 |
| -static    | 使用静态库链接生成目标文件，避免使用共享库，生成目标文件会比使用动态链接库大 |

## LIBS

LDFLAGS是选项，LIBS是要链接的库。LDFLAGS告诉链接器从哪里寻找库文件，LIBS告诉链接器要链接哪些库文件。



------

## **GCC参数详解**

gcc and g++分别是gnu的c & c++编译器。
gcc/g++在执行编译工作的时候，总共需要4步： 

1. 预处理，生成.i的文件 
2. 将预处理后的文件不转换成汇编语言，生成文件.s 
3. 汇编变为目标代码(机器代码)生成 .o 文件 
4. 连接目标代码，生成可执行程序

#### [参数详解] 

-E 

​		只激活预处理，这个不生成文件，你需要把它重定向到一个输出文件里面。

​		gcc -E hello.c > pianoapan.txt 

​		gcc -E hello.c | more  # 慢慢看吧，一个hello word 也要与处理成800行的代码 

-S 
　　只激活预处理和编译，就是指把文件编译成为汇编代码。 
　　gcc -S hello.c 
　　他将生成.s的汇编代码，你可以用文本编辑器察看 

-c  

　　只激活预处理，编译，和汇编，也就是他只把程序做成obj文件 
　　gcc -c hello.c 
　　他将生成.o的obj文件 

-o 
　　制定目标名称，缺省的时候，gcc 编译出来的文件是a.out
　　gcc -o hello.exe hello.c (哦，windows用习惯了) 
　　gcc -o hello.asm -S hello.c 

-ansi 
　　关闭gnu c中与ansi c不兼容的特性，激活ansi c的专有特性(包括禁止一些asm inline typeof关键字，以及UNIX，vax等预处理宏， 

-wall

​		显示警告信息

-fno-asm 
　　此选项实现ansi选项的功能的一部分，它禁止将asm，inline和typeof用作关键字。 
-fno-strict-prototype 
　　只对g++起作用，使用这个选项，g++将对不带参数的函数，都认为是没有显式的对参数的个数和类型说明，而不是没有参数. 
　　而gcc无论是否使用这个参数，都将对没有带参数的函数，认为城没有显式说明的类型 
-fthis-is-varialble 
　　就是向传统c++看齐，可以使用this当一般变量使用. 
-fcond-mismatch 
　　允许条件表达式的第二和第三参数类型不匹配，表达式的值将为void类型 
-funsigned-char 
-fno-signed-char 
-fsigned-char 
-fno-unsigned-char 
　　这四个参数是对char类型进行设置，决定将char类型设置成unsigned char(前两个参数)或者 signed char(后两个参数) 

-include file 
　　包含某个代码，简单来说，就是便以某个文件，需要另一个文件的时候，就可以用它设定，功能就相当于在代码中使用#include 
　　gcc hello.c -include /root/pianopan.h 
-imacros file 
　　将file文件的宏，扩展到gcc/g++的输入文件，宏定义本身并不出现在输入文件中 
-Dmacro
　　相当于C语言中的#define macro 
-Dmacro=defn
　　相当于C语言中的#define macro=defn 
-Umacro 
　　相当于C语言中的#undef macro 
-undef 
　　取消对任何非标准宏的定义 
-Idir 
　　在你是用#include"file"的时候，gcc/g++会先在当前目录查找你所制定的头文件，如果没有找到，他回到缺省的头文件目录找，如果使用-I制定了目录，他 
　　回先在你所制定的目录查找，然后再按常规的顺序去找. 
　　对于#include，gcc/g++会到-I制定的目录查找，查找不到，然后将到系统的缺省的头文件目录查找 
-I- 
　　就是取消前一个参数的功能，所以一般在-Idir之后使用 
-idirafter dir 
　　在-I的目录里面查找失败，讲到这个目录里面查找. 
-iprefix prefix 
-iwithprefix dir 
　　一般一起使用，当-I的目录查找失败，会到prefix+dir下查找 
-nostdinc 
　　使编译器不再系统缺省的头文件目录里面找头文件，一般和-I联合使用，明确限定头文件的位置 
-nostdin C++ 
　　规定不在g++指定的标准路经中搜索，但仍在其他路径中搜索。此选项在创建libg++库使用 
-C 
　　在预处理的时候，不删除注释信息，一般和-E使用，有时候分析程序，用这个很 
　　方便的 
-M 
　　生成文件关联的信息。包含目标文件所依赖的所有源代码 
　　你可以用gcc -M hello.c来测试一下，很简单。 
-MM 
　　和上面的那个一样，但是它将忽略由#include造成的依赖关系。 
-MD 
　　和-M相同，但是输出将导入到.d的文件里面 
-MMD 
　　和-MM相同，但是输出将导入到.d的文件里面 
-Wa，option 
　　此选项传递option给汇编程序;如果option中间有逗号，就将option分成多个选项，然后传递给会汇编程序 
-Wl.option 
　　此选项传递option给连接程序;如果option中间有逗号，就将option分成多个选项，然后传递给会连接程序. 
-llibrary  

　　制定编译的时候使用的库 
　　gcc -lcurses hello.c 
　　使用ncurses库编译程序 
-Ldir 
　　制定编译的时候，搜索库的路径。比如你自己的库，可以用它制定目录，不然编译器将只在标准库的目录找。这个dir就是目录的名称。 
-O0 
-O1 
-O2 
-O3 
　　编译器的优化选项的4个级别，-O0表示没有优化，-O1为缺省值，-O3优化级别最高　　 
-g 
　　只是编译器，在编译的时候，产生调试信息。 
-gstabs 
　　此选项以stabs格式声称调试信息，但是不包括gdb调试信息. 
-gstabs+ 
　　此选项以stabs格式声称调试信息，并且包含仅供gdb使用的额外调试信息. 
-ggdb 
　　此选项将尽可能的生成gdb的可以使用的调试信息。
