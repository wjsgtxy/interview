- gcc HelloWorld.c -E -o HelloWorld.i预处理：加入头文件，替换宏。

- gcc HelloWorld.c -S -c -o HelloWorld.s编译：包含预处理，将C程序转换成汇编程序。

- gcc HelloWorld.c -c -o HelloWorld.o汇编：包含预处理和编译，将汇编程序转换成可链接的二进制程序。

- gcc HelloWorld.c -o HelloWorld链接：包含以上所有操作，将可链接的二进制程序和其它别的库链接在一起，形成可执行的程序文件。

  如上图所示，没有链接的hello.o 无法执行的！

- objdump -d HelloWorld 查看汇编指令



##### gcc编译选项说明官方文档！

https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Invoking-GCC.html#Invoking-GCC

-L 指定库路径：

https://www.cnblogs.com/benio/archive/2010/10/25/1860394.html

```bash
gcc -lrt  -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm  devices.o   -o devices
```



#### 好文章：[C代码中如何调用C++ C++中如何调用C](https://www.cnblogs.com/Yogurshine/p/3913073.html)



#### 删除Linux环境变量

```
unset LD_LIBRARY_PATH
```

###### 设置编译时候的预处理环境变量：

https://blog.csdn.net/weixin_44327262/article/details/105860213

https://www.cnblogs.com/linuxbo/p/4301716.html

```
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/somewhere/include
```







#### 动态链接库的查找先后顺序为：https://zhuanlan.zhihu.com/p/235551437

- `LD_LIBRARY_PATH`环境变量中的路径
- `/etc/ld.so.cache`缓存文件
- `/usr/lib`和`/lib`

比如，我们把CUDA安装到`/opt`下面，我们可以使用下面的命令将CUDA添加到环境变量里。

```bash
# 查看
echo ${LD_LIBRARY_PATH}

# 添加环境变量
export LD_LIBRARY_PATH=/home/rdmacore/rdma-core-master/build/lib/:$LD_LIBRARY_PATH

# 清空环境变量
unset LD_LIBRARY_PATH
```

除了`LD_LIBRARY_PATH`环境变量外，还有一个`LD_PRELOAD`环境变量。`LD_PRELOAD`的查找顺序比`LD_LIBRARY_PATH`还要优先。`LD_PRELOAD`里是具体的目标文件列表（A list of shared objects）；`LD_LIBRARY_PATH`是目录列表（A list of directories）。



#### 添加so文件路径到共享库：

```bash
vi /etc/ld.so.conf

# 添加一行
/home/link/testso （注意：include是用来添加一个文件夹用的，只添加一个目录的话，直接在这个文件最下面append一行即可）

# 刷新ld.so.cache
ldconfig
```



###### 注意：配置了/etc/ld.so.conf 运行ldconfig 编译仍找不到动态库

ld.so是动态库载入器而不是连接器，某些翻译“动态连接器”有问题的，或者说英文原文就有问题

换句话说，配置ld.so.conf的作用就是如果so在非标准路径，可执行文件能够找到，这是运行期做的事情

**你现在是编译，ld.so根本不起作用，所有用非标准路径的话必需手动指定库目录**



#### /usr/bin/ld 搜索路径顺序：

###### 一. **静态库链接**时搜索路径顺序 

1. ld会去找GCC命令中的参数-L
2. 再找gcc的环境变量LIBRARY_PATH
3. 再找内定目录 /lib /usr/lib /usr/local/lib 这是当初compile gcc时写在程序内的  

###### 二. 动态链接时、执行时搜索路径顺序: 

```
1. 编译目标代码时指定的动态库搜索路径

2. 环境变量LD_LIBRARY_PATH指定的动态库搜索路径

3. 配置文件/etc/ld.so.conf中指定的动态库搜索路径

4. 默认的动态库搜索路径/lib 

5. 默认的动态库搜索路径/usr/lib  
```

###### 三. 有关环境变量

```
LIBRARY_PATH环境变量：指定程序**静态链接库**文件搜索路径
```

```bash
# 查看
echo ${LIBRARY_PATH}
echo ${LD_LIBRARY_PATH} 

# 添加环境变量，终于解决了 /usr/bin/ld: cannot find -lrdmacm 的问题!!!
// export LIBRARY_PATH=$LIBRARY_PATH:/home/rdmacore/rdma-core-master/build/lib/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/rdmacore/rdma-core-master/build/lib/
export LD_LIBRARY_PATH=/home/rdmacore/rdma-core-master/build/lib/
```

```
LD_LIBRARY_PATH环境变量：指定程序**动态链接库**文件搜索路径
```

dz: 静态库只是链接ld的时候起作用，运行时没作用，动态库链接的时候要去拿符号表，运行时也要用

**gcc -L可以直接指定库的路径：这样也能找到-lrdmacm库**

```
gcc -lrt  -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm  devices.o   -o devices
```



总结：

LD_LIBRARY_PATH这玩意是运行时候用的，链接的时候老老实实的用 -L添加路径



##### 生成动态链接库：

```
$gcc -shared -Wl,-soname,libhello.so.1 -o libhello.so.1.0 hello.o
```

另外再建立两个符号连接：

```
$ln -s libhello.so.1.0 libhello.so.1

$ln -s libhello.so.1 libhello.so
```

这样1个libhello的动态连接库就生成了。最重要的是传gcc -shared 参数使其生成是动态库而不是普通履行程序。 -Wl 表示后面的参数也就是-soname,libhello.so.1直接传给连接器ld进行处理。实际上，每个库都有1个soname，当连接器发现它正在查找的程序库中有这样1个名称，连接器便会将soname嵌入连结中的2进制文件内，而不是它正在运行的实际文件名，在程序履行期间，程序会查找具有 soname名字的文件，而不是库的文件名，换句话说，soname是库的辨别标志。 这样做的目的主要是允许系统中多个版本的库文件共存，习惯上在命名库文件的时候通常与soname相同 libxxxx.so.major.minor 其中，xxxx是库的名字，major是主版本号，minor 是次版本号



```
lsmod 查看内核模块
```



##### 静态库，动态库，头文件区别：

https://blog.csdn.net/weixin_42458272/article/details/106193786

静态库，动态库：

https://blog.csdn.net/qq_22122811/article/details/78226835

linux默认的头文件在 /usr/include里面，比如 stdio.h stdlib.h string.h

网络编程的在/usr/include/netinet里面

如果你的头文件在其他地方，gcc编译的时候，加上 -I （i:include）指定路径即可

```bash
# 解决了找不到 <rdma/rdma_cma.h>的问题
gcc -I /home/rdmacore/rdma-core-master/build/include/ yyy.c
```



##### "undefined reference to XXX"问题总结

好文章！！TODO，全文收藏起来！！

https://zhuanlan.zhihu.com/p/81681440

https://www.cnblogs.com/quant-lee/p/6639008.html

从知乎的这个链接的case2，解决了我“undefined reference to rdma_get_devices”的部分问题：

```bash
# 之前是这样的命令，一直报错 undefined reference to 'rdma_get_devices', undefined ...to 'ibv_query_device', undefined ... to 'rdma_free_deivecs'
gcc -lrt  -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm  devices.o -o devices

# 目标文件之间有依赖关系，A依赖B, A要放在前面才行，把devices.o放到前面
gcc devices.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -o devices

# 上面之后报了另外的错误：undefined reference to symbol 'ibv_query_devices@@IBVERBS_1.1' /usr/bin/ld: libibverbs.so.1: error adding symbols:DSO missing from command line

# 我将 ibibvers.so这个库添加上就OK了，终于编译成功了！！！
gcc devices.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -libverbs -o devices
```



注意，上面的报错，其实提示很明显了：**’DSO missing from command line‘**

https://blog.csdn.net/zrq293/article/details/105969423/

问题可以简单描述为：当链接可执行文件时，依赖于libA.so，而libA.so又依赖于libB.so，而且可执行文件中还直接调用了libB.so中的函数，那么此时链接就会出现错误。



```
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



##### 库的链接顺序问题：

https://www.cnblogs.com/ironx/p/4939508.html

如果你的库在链接时安排的顺序是：foo.o -lz bar.o。那么gcc的链接器先搜索库foo，然后是z库，然后是bar库。

这样就带来一个问题，如果库bar调用了库z里面的函数，但是链接器是先搜索的库z，这时候并没有发现库bar调用库z啊，所以就不会把库z中的这部分函数体挑出来进行链接。

而是只把库z中，被foo库调用的函数体挑出来。





##### linux头文件搜索

```bash
# linux 头文件搜索路径
root@xyvm2:/usr/include/linux# echo 'main(){}'|gcc -E -v -
...
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/9/include
 /usr/local/include # 这个目录里面放的是我之前学习cmake后install后将MathFunctions.h头文件放到了这里
 /usr/include/x86_64-linux-gnu
 /usr/include # 大部分头文件就在这里 rdma的目录也在这里

```

rdma 头文件目录：

```bash
/usr/include/rdma # 里面有
/usr/include/valgrind # 里面有memcheck.h
```

问题 /usr/include/rdma目录里面的和我自己编译的rdmacore-master/build/include/rdma 相比，少了3个头文件，少了rdma_cma.h等头文件，所以我之前编译demo的时候，才一直提示找不到rdma_cma.h。

但是这个目录里面又有rdma_netlink.h和rdma_user_cm.h，现在我发现这两个文件都是链接到 **kernel-headers**目录里面的。rdma_cma.h是链接到**librdmacm**目录里面的，因为是用户态的，所以这个文件就没有直接包含在系统里面吧。

```
root@xyvm2:/home/rdmacore/rdma-core-master/build/include/rdma# ls
...
lrwxrwxrwx 1 root root   52 Jan 17 08:25 rdma_cma.h -> /home/rdmacore/rdma-core-master/librdmacm/rdma_cma.h
lrwxrwxrwx 1 root root   66 Jan 17 08:25 rdma_netlink.h -> /home/rdmacore/rdma-core-master/kernel-headers/rdma/rdma_netlink.h
lrwxrwxrwx 1 root root   66 Jan 17 08:25 rdma_user_cm.h -> /home/rdmacore/rdma-core-master/kernel-headers/rdma/rdma_user_cm.h

```

```
# -L可以将软链接指向的文件复制过来
cp -rL /src /dst 
```



设置gcc头文件搜索目录：

> `C_INCLUDE_PATH`、`CPLUS_INCLUDE_PATH`以及`CPATH`常被用于在全局性地添加预处理C/C++时的包含目录，其中`C_INCLUDE_PATH`仅对预处理C有效，`CPLUS_INCLUDE_PATH`仅对预处理C++有效，而`CPATH`对所有语言均有效。

https://blog.csdn.net/weixin_44327262/article/details/105860213

```bash
echo $C_INCLUDE_PATH
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/home/rdmacore/rdma-core-master/build/include/
```



```bash
# 查看系统是否有加载该库
ldconfig -v |grep "librdmacm" 
# 输出
librdmacm.so.1 -> librdmacm.so.1.2.28.0
```

在我把/etc/ld.so.conf中的路径删除之前，这里查询会有两个输出，另外一个librdmacm指向so.1.3.39那个，那个是在我编译的rdmacore中lib目录下的那个





ldconfig做的这些东西都与运行程序时有关，跟编译时一点关系都没有。编译的时候还是该加-L就得加，不要混淆了!!!



```bash
# 我把rdmacore中lib目录整体复制到 /usr/lib/rdma里面去了，这样不用每次编译都指定 -L参数了，但是仍然提示找不到 -lrdmacm
cp -r /home/rdmacore/rdma-core-master/build/lib/ /usr/lib/rdma/
gcc -lrt -lrdmacm client.o utilities.o -o client

# 还是要加-L，这个目录有 librdmacm.so.1.3.39.0
gcc -lrt -L/usr/lib/rdma/ -lrdmacm client.o utilities.o -o client

# 这样也是提示找不到 -lrdmacm，但是这个目录里面有 librdmacm.so.1.2.28.0
gcc -lrt -L/usr/lib/x86_64-linux-gnu -lrdmacm client.o utilities.o -o client


```



解决“DSO missing from command line”的问题

https://blog.csdn.net/zrq293/article/details/105969423/

```
# 添加--copy-dt-needed-entries，链接器自己回去加载lrdmacm需要的库文件
LIBS = -lrt -Wl,--copy-dt-needed-entries -L/usr/lib/rdma/ -lrdmacm
```



------

### **GCC参数详解**

 [介绍] 
gcc and g++分别是gnu的c & c++编译器 
gcc/g++在执行编译工作的时候，总共需要4步 
1.预处理,生成.i的文件 
2.将预处理后的文件不转换成汇编语言,生成文件.s 
3.有汇编变为目标代码(机器代码)生成.o的文件 
4.连接目标代码,生成可执行程序 

[参数详解] 
-c  
　　只激活预处理,编译,和汇编,也就是他只把程序做成obj文件 
　　例子用法: 
　　gcc -c hello.c 
　　他将生成.o的obj文件  

**dz: makefile中就是分两步处理的，先用-c 生成.o文件，然后链接上其他库文件**

```bash
# -c
gcc -Wall -O -g -I /home/rdmacore/rdma-core-master/build/include/   -c -o devices.o devices.c
# -o
gcc -lrt -lrdmacm  devices.o  -o devices
```



-S 
　　只激活预处理和编译，就是指把文件编译成为汇编代码。 
　　例子用法 
　　gcc -S hello.c 
　　他将生成.s的汇编代码，你可以用文本编辑器察看 
-E 
　　只激活预处理,这个不生成文件,你需要把它重定向到一个输出文件里 
　　面. 
　　例子用法: 
　　gcc -E hello.c > pianoapan.txt 
　　gcc -E hello.c | more 
　　慢慢看吧,一个hello word 也要与处理成800行的代码 
-o 
　　制定目标名称,缺省的时候,gcc 编译出来的文件是a.out,很难听,如果 
　　你和我有同感，改掉它,哈哈 
　　例子用法 
　　gcc -o hello.exe hello.c (哦,windows用习惯了) 
　　gcc -o hello.asm -S hello.c 
-ansi 
　　关闭gnu c中与ansi c不兼容的特性,激活ansi c的专有特性(包括禁止一 
　　些asm inline typeof关键字,以及UNIX,vax等预处理宏, 

-wall

```
  显示警告信息
```

-fno-asm 
　　此选项实现ansi选项的功能的一部分，它禁止将asm,inline和typeof用作 
　　关键字。 
-fno-strict-prototype 
　　只对g++起作用,使用这个选项,g++将对不带参数的函数,都认为是没有显式 
　　的对参数的个数和类型说明,而不是没有参数. 
　　而gcc无论是否使用这个参数,都将对没有带参数的函数,认为城没有显式说 
　　明的类型 
-fthis-is-varialble 
　　就是向传统c++看齐,可以使用this当一般变量使用. 
-fcond-mismatch 
　　允许条件表达式的第二和第三参数类型不匹配,表达式的值将为void类型 
-funsigned-char 
-fno-signed-char 
-fsigned-char 
-fno-unsigned-char 
　　这四个参数是对char类型进行设置,决定将char类型设置成unsigned char(前 
　　两个参数)或者 signed char(后两个参数) 

-include file 
　　包含某个代码,简单来说,就是便以某个文件,需要另一个文件的时候,就可以 
　　用它设定,功能就相当于在代码中使用#include 
　　例子用法: 
　　gcc hello.c -include /root/pianopan.h 
-imacros file 
　　将file文件的宏,扩展到gcc/g++的输入文件,宏定义本身并不出现在输入文件 
　　中 
-Dmacro 
　　相当于C语言中的#define macro 
-Dmacro=defn 
　　相当于C语言中的#define macro=defn 
-Umacro 
　　相当于C语言中的#undef macro 
-undef 
　　取消对任何非标准宏的定义 
-Idir 
　　在你是用#include"file"的时候,gcc/g++会先在当前目录查找你所制定的头 
　　文件,如果没有找到,他回到缺省的头文件目录找,如果使用-I制定了目录,他 
　　回先在你所制定的目录查找,然后再按常规的顺序去找. 
　　对于#include,gcc/g++会到-I制定的目录查找,查找不到,然后将到系 
　　统的缺省的头文件目录查找 
-I- 
　　就是取消前一个参数的功能,所以一般在-Idir之后使用 
-idirafter dir 
　　在-I的目录里面查找失败,讲到这个目录里面查找. 
-iprefix prefix 
-iwithprefix dir 
　　一般一起使用,当-I的目录查找失败,会到prefix+dir下查找 
-nostdinc 
　　使编译器不再系统缺省的头文件目录里面找头文件,一般和-I联合使用,明确 
　　限定头文件的位置 
-nostdin C++ 
　　规定不在g++指定的标准路经中搜索,但仍在其他路径中搜索,.此选项在创建 
　　libg++库使用 
-C 
　　在预处理的时候,不删除注释信息,一般和-E使用,有时候分析程序，用这个很 
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
-Wa,option 
　　此选项传递option给汇编程序;如果option中间有逗号,就将option分成多个选 
　　项,然后传递给会汇编程序 
-Wl.option 
　　此选项传递option给连接程序;如果option中间有逗号,就将option分成多个选 
　　项,然后传递给会连接程序. 
-llibrary  
　　制定编译的时候使用的库 
　　例子用法 
　　gcc -lcurses hello.c 
　　使用ncurses库编译程序 
-Ldir 
　　制定编译的时候，搜索库的路径。比如你自己的库，可以用它制定目录，不然 
　　编译器将只在标准库的目录找。这个dir就是目录的名称。 
-O0 
-O1 
-O2 
-O3 
　　编译器的优化选项的4个级别，-O0表示没有优化,-O1为缺省值，-O3优化级别最 
　　高　　 
-g 
　　只是编译器，在编译的时候，产生条是信息。 
-gstabs 
　　此选项以stabs格式声称调试信息,但是不包括gdb调试信息. 
-gstabs+ 
　　此选项以stabs格式声称调试信息,并且包含仅供gdb使用的额外调试信息. 
-ggdb 
　　此选项将尽可能的生成gdb的可以使用的调试信息.
