
#### gdb相关

gdb凭什么可以调试一个程序？凭什么能够接管一个程序的运行？
其实，很简单，通过一个系统调用：ptrace。
ptrace()系统调用提供了一个方法，该方法使一个程序（追踪者）可以观察和控制另外一个程序（被追踪者）的执行，并检查和改变被追踪者的内存及寄存器。它主要用于实现断点调试和追踪系统调用。
(http://man7.org/linux/man-pages/man2/ptrace.2.html)



##### GDB调试方法：

方式一：启动程序并调试  #gdb [program]
1.该模式针对启动失败问题非常有效，一般需要设置好so加载路径export LD_LIBRARY_PATH=xx，然后逐步跟踪程序运行到问题点。一般自己调试以及实验室环境操作。

方式二：调试core文件  #gdb [program][core]
1.在这种模式下，GDB调试的其实是core文件，所以是可以直接看到core文件产生时的信息，如使用bt查看调用栈，以及寄存器现场信息。一般现网问题、自己调试以及实验室环境操作。

方式三：调试已在运行的文件（通过pid ）#gdb [program][pid]

程序可能已经在运行了，这时候就需要使用GDB的attach功能。 

1. gdb [program][pid]会自动识别第二个参数为pid(如果是十进制的数字的话)。
2. 运行gdb之后，在gdb中使用指令(gdb) attach [pid], 该方式最常见。
3. 如果不使用源文件调试，可以直接#gdb –p [pid]来指定pid进入attach状态。



#### GDB通用命令介绍：

断点命令：break，缩写b, 
   break <source_file_name:line_num> 文件名:行号, 
   break <function_name> 函数名
删除断点命令：delete <break_point>，缩写d, 
   delete 1  删除1号断点
查看堆栈命令：backtrace,缩写bt （打印最内n层：bt n, 最外n层： bt -n）
查看所有线程堆栈命令：thread apply all bt
查看变量命令:p
   p <para_name> 变量名 （可使用p直接修改变量值：p <para_name> =xx）
查看所有的局部变量: info local
转到线程令：thread <thread_number> 
转到帧命令：frame <frame_number> 
执行下一步命令：next，缩写n
继续运行直到达到下一个断点命令：continue,缩写c
进入子函数命令：step，缩写s
结束子函数返回到调用函数命令：finish，缩写f
结束循环命令：until，缩写u



查看内存地址：examine，缩写x, 
   x/<n/f/u> <addr>, 
      输出格式
	      x 按十六进制格式显示变量。
      d 按十进制格式显示变量。
      u 按十六进制格式显示无符号整型。
      o 按八进制格式显示变量。
       t 按二进制格式显示变量。
      a 按十六进制格式显示变量。
      c 按字符格式显示变量。
      f 按浮点数格式显示变量。
      s 按字符串格式显示变量。
      举例：x/10wx addr  查看10个4字节按16进制输出
       x/10i  addr  查看该地址的10条指令
查看数据：print，缩写, 
         print /f exp  exp是表达式，/f指定打印时的格式
         打印格式和x输出格式一致



信号 signals, 
   x/<n/f/u> <addr>, 
   info signals 
       打印信号种类表及GDB对他们相应的处理。
   handle signal keyword
       改变GDB对signal信号的处理。
   stop/nostop
   print/noprint
   pass/nopass
   一般常用： handle SIG35 nostop noprint (产品一般会自定义该信号的处理，因此调试过程先过滤掉)
watch 内存, 
         watch  expr：GDB在expr被程序写及其值改变时停止；
         rwatch expr：GDB在expr被程序读时停止；
         awatch expr：GDB在expr被程序读和写时停止；
         使用较多的是watch一个地址内存是否有变化，watch *(int*)addr
info register 查看寄存器，
       i r（缩写）: 查看当前栈的所有寄存器信息，定位问题的时候经常用到；



多线程调试常用命令如下：
info thread                  --显示所有thread线程（*号代表gdb的当前线程）
thread thread_num            --gdb现场切换到对应thread
thread apply ID1 ID2 command --让一个或者多个线程执行GDB命令command
thread apply all command     --让所有被调试线程执行GDB命令command

```
比如：
```

```
thread apply all bt // 查看所有线程调用栈
```

b xxxx thread thread_num     --只给thread_num这个线程的xxxx函数打断点
set scheduler-locking off|on|step 
off 不锁定任何线程，也就是所有线程都执行，这是默认值；
on 只有当前被调试程序会执行；
step 当用" step "命令调试线程时，其它线程不会执行，但是用其它命令（比如" next "）调试 线程时，其它线程也许会执行；
thread apply all bt          --查看所有线程调用栈（常用）



gdb watch 内存好像可以watch到变量的改变，改变一下打印一下 TODO试试



先让我们普及下寄存器和任务栈的基础知识，这是异常问题分析定位的必备知识。
x86_32:
2个变址寄存器(ESI和EDI) 
6个段寄存器(ES、CS、SS、DS、FS和GS) 
4个数据寄存器(EAX、EBX、ECX和EDX) 
esp：寄存器存放当前线程的栈顶指针ebp：寄存器存放当前线程的栈底指针eip：寄存器存放下一个CPU指令存放的内存地址，当CPU执行完当前的指令后，从EIP寄存器中读取下一条指令的内存地址，然后继续执行
入参顺序：EDI ESI EDX ECX 多余的通过栈传递，依次对应第1参数，第2参数。。。

X86_64:
X86-64有16个64位寄存器，分别是：%rax，%rbx，%rcx，%rdx，%esi，%edi，%rbp，%rsp，%r8，%r9，%r10，%r11，%r12，%r13，%r14，%r15。其中：%rax 作为函数返回值使用
%rbp 栈帧寄存器 %rsp 栈指针寄存器，指向栈顶%rdi，%rsi，%rdx，%rcx，%r8，%r9 用作函数参数，依次对应第1参数，第2参数。。。%rbx，%rbp，%r12，%r13，%14，%15 用作数据存储，遵循被调用者使用规则，简单说就是随便用，调用子函数之前要备份它，以防他被修改



arm32：
R0-R3传递函数入参用,  多出4个参数的部分，保存在调用函数的堆栈中，通过堆栈传递给被调用函数
R1和R2作为返回值寄存器，返回值类型为字长及以下的参数只使用R0，超过字长的参数使用R0和R1
R4-R9：在程序中一般用做局部变量寄存器
R11：fp栈框指针，栈的最低端，指向函数堆栈框的底端
R12：ip指令指针，栈的最高端，解决32位空间的寻址问题
R13：SP堆栈指针
R14：LR程序连接寄存器，保存函数调用的返回地址
R15：PC指针 （程序计数器）

arm64/arm64_ILP32
X0~X7：用于传递子程序参数和结果，使用时不需要保存，多余参数采用堆栈传递，64位返回结果采用X0表示，128位返回结果采用X1:X0表示
X8：用于保存子程序返回地址，尽量不要使用 
X9~X15：临时寄存器，使用时不需要保存
X16~X17：子程序内部调用寄存器，使用时不需要保存，尽量不要使用
X18：平台寄存器，它的使用与平台相关，尽量不要使用
X19~X28：临时寄存器，使用时必须保存
X29：帧指针寄存器，用于连接栈帧，使用时需要保存
X30：链接寄存器LR
X31：堆栈指针寄存器SP或零寄存器ZXR
PC：程序指针寄存器

$\color{red}{红色字}$

<font color=blue>MD</font>



GDB调试100个技巧：

https://github.com/hellogcc/100-gdb-tips/blob/master/src/index.md

##### gdb调试传入命令行参数

1. r的时候传入参数
2. set args 命令行参数

##### [LWP作用](https://baike.baidu.com/item/lwp/760839?fr=aladdin)

todo 点开链接看百度百科的图片

LWP: 用户级线程和[内核级线程](https://baike.baidu.com/item/%E5%86%85%E6%A0%B8%E7%BA%A7%E7%BA%BF%E7%A8%8B/7355786)之间的中间层。

轻量级进程 (LWP, light weight process) 是一种由[内核](https://baike.baidu.com/item/%E5%86%85%E6%A0%B8)支持的用户线程。它是基于[内核](https://baike.baidu.com/item/%E5%86%85%E6%A0%B8)线程的高级抽象，因此只有先支持[内核](https://baike.baidu.com/item/%E5%86%85%E6%A0%B8)线程，才能有 LWP 。每一个LWP可以支持一个或多个用户线程，每个 LWP 由一个[内核](https://baike.baidu.com/item/%E5%86%85%E6%A0%B8)线程支持。内核线程与LWP之间的模型实际上就是《[操作系统概念](https://baike.baidu.com/item/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E6%A6%82%E5%BF%B5/8259031)》上所提到的一对一线程模型。在这种实现的操作系统中， LWP 相当于[用户线程](https://baike.baidu.com/item/%E7%94%A8%E6%88%B7%E7%BA%BF%E7%A8%8B)。 由于每个 LWP 都与一个特定的内核线程关联，因此每个 LWP 都是一个独立的[线程调度](https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B%E8%B0%83%E5%BA%A6)单元。即使有一个 LWP 在[系统调用](https://baike.baidu.com/item/%E7%B3%BB%E7%BB%9F%E8%B0%83%E7%94%A8)中阻塞，也不会影响整个进程的执行。

[轻量级进程](https://baike.baidu.com/item/%E8%BD%BB%E9%87%8F%E7%BA%A7%E8%BF%9B%E7%A8%8B)具有局限性。首先，大多数 LWP 的操作，如建立、析构以及同步，都需要进行系统调用。系统调用的代价相对较高：需要在 user mode 和 kernel mode 中切换。其次，每个 LWP 都需要有一个内核线程支持，因此 LWP 要消耗内核资源（内核线程的栈空间）。因此一个系统不能支持大量的 LWP 。



#### 使用gdb调试多线程程序

gdb命令：

```bash
# 查看当前所有线程
info threads
# 切换到线程，这里的id指的是gdb给出的线程id
thread t_id

```

https://www.cnblogs.com/WindSun/p/12785322.html

gdb调试r运行程序后，才能设置 线程锁

```
set scheduler-locking on
```

设置之后，当前线程运行的时候，其他几个线程会暂停，不会运行，只有手动 thread t-id切换过去然后输入c才会继续运行，这个选项默认是关闭的，即当前线程打断点执行的时候，其他线程也在执行。



non-stop模式

1. 当某个或多个线程在一个断点上，其他线程仍会并行运行 
2. 你可以选择某个被中断的线程，只让他运行。

```bash
# 设置non-stop模式，打开gdb后，在开始r之前，首先连续输入下面的指令
set target-async 1
set pagination off
set non-stop on
```

