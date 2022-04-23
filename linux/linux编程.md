# linux编程



###### Linux系统调用--getrusage函数详解

https://www.cnblogs.com/iihcy/p/5105839.html

其中 rusage结构体存放的是资源使用信息，定义在/usr/include/sys/resource.h



###### calloc

```
void *calloc(size_t nitems, size_t size)
```

**malloc** 和 **calloc** 之间的不同点是，malloc 不会设置内存为零，而 calloc 会设置分配的内存为零。



###### getopt用法详解

###### https://blog.csdn.net/qq_22642239/article/details/106462986

用来处理命令行参数，不用自己处理 argv的参数了



###### getaddrinfo()函数详解

###### https://blog.csdn.net/fanx021/article/details/80549945

ipv6中新添加的函数，更好用，完成主机名到地址的解析





###### inet_pton()和inet_ntop()功能及使用方法

https://blog.csdn.net/weixin_38420191/article/details/114691058



###### inet_pton()
将**标准文本表示形式**的IPv4或IPv6 Internet网络地址**转换为数字二进制形式**。






###### memcpy

http://www.cplusplus.com/reference/cstring/memcpy/?kw=memcpy

```
void * memcpy ( void * destination, const void * source, size_t num );
```

*destination* is returned.



```
errno_t memcpy_s(  void* dest, size_t destMax,
  const void* src,
  size_t count
)
```



###### [bcopy函数](https://www.cnblogs.com/winifred-tang94/p/5842529.html)

> 函数原型：void bcopy（const  void  *src,  void  *dest,  int  n）
>
> 头文件：#include <string.h>
>
> 函数功能：将src指针指向的地址的内容的前n个字节复制给dest指针指向的地址
>
> 此函数在VC6.0和MinGW5下编译没有通过，但是利用LINUX下的GCC可以支持



###### snprintf：格式化字符串

snprintf()，函数原型为[int](https://baike.baidu.com/item/int/54055) snprintf(char *str, size_t size, const char *format, ...)。

将可变参数 “…” 按照format的格式格式化为[字符串](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6%E4%B8%B2/1017763)，然后再将其拷贝至[str](https://baike.baidu.com/item/str/11045424)中。



snprintf跟_snprintf的区别：（https://blog.csdn.net/cjjwwss/article/details/12579113）

> snprintf函数并不是标准c/c++中规定的函数，但是在许多编译器中，厂商提供了其实现的版本。 在gcc中，该函数名称就snprintf，而在VC中称为_snprintf。 由于不是标准函数，没有一个统一的标准来规定该函数的行为，所以导致了各厂商间的实现版本可 能会有差异。今天也的的确确看到了差异，因为这个小小的差异是我的程序无法正常的处理数据。 这个小小的差异发生在count参数。vc中这个值是不计算\0的，gcc中是计算\0的，而且两个函数返回值也不一样，vc返回-1，gcc返回原本要存入的字符长度

sprintf,sprintf_s,_snprintf和_snprintf_s之间的区别:(https://blog.csdn.net/tjcwt2011/article/details/112800487)

函数原型：

int sprintf(char *[buffer](https://so.csdn.net/so/search?q=buffer&spm=1001.2101.3001.7020),const char *format [,argument] ...); // 有溢出风险

int sprintf_s(char *buffer,size_t sizeOfBuffer,const char *format [,argument] ...); // 安全



> 1、sprintf_s对format 中的格式化的字符的有效性进行了检查，而sprintf仅仅检查format 或者缓冲区是否是空指针。如果有错误则返回相应的错误代码。
>
> 2、sprintf_s参数sizeOfBuffer是接收格式化字符串的缓冲区的大小。如果，格式化字符串过大，则sprintf_s会返回一个空string和设置无效参数句柄为激活。
>
> 3、sprintf_s将格式化字符串存到缓冲区，并在下一个位置填充Null后将格式化字符串未占用的缓冲区(Null之后的Buffer)全部填充为-3，而sprintf却不会填充而是保持缓冲区中未占用的存储位置上的数据。

**sprintf_s，_snprintf与_snprintf_s区别**

函数原型：
int sprintf_s(char *buffer,size_t sizeOfBuffer,const char *format [,argument] ... );
int _snprintf(char *buffer,size_t count,const char *format [,argument] ... ); // 注意，这个只有count,但是count任然可能超出 buffer的长度！也是不安全的！！！

int _snprintf_s(char *buffer,size_t sizeOfBuffer,size_t count,const char *format [,argument] ... );



从sprintf与sprintf_s区别可以知道，如果应该输出的字符串的大小已经达到了sizeOfBuffer，那么就溢出了。溢出的情况下，sprintf_s函数把这当做一个错误，会把buffer缓冲区置为一个空字符串""。而count参数的作用是，输出的字符串就算超过缓冲区长度，仍然会有输出，输出字符串被截断到count大小，在这个大小的字符串后面加null-teminate。

这里比较引人注目的是，_snprintf_s为什么在sizeOfBuffer的基础上，还要多加一个count？count似乎是用来控制理想的宽度的。如果得到的字符串超过了count，于是会被截断到count的长度后面再加一个null-teminate。当然，更高优先级的应该是sizeOfBuffer，必须不超过这个大小。当然，如果count被设置成和sizeOfBuffer同样大，或者不合理的更大，那么这个count参数就失去了意义。这时候，如果输出字符串将要达到或者超过sizeOfBuffer，一样导致一个错误，输出缓冲区被置为空字符串。因此，如果希望缓冲区被尽量利用，可以把count参数置为_TRUNCATE，这样的情况下，实际上效果相当于是将count设置为sizeOfBuffer -1。

总结来说，sprintf_s在缓冲区不够大时会失败，失败时缓冲区中是一个空字符串。

_snprintf不会失败，但是必须注意,如果缓冲区不够大，缓冲区的内容将不是null-teminate的，必须自己注意字符串的结束。

_snprintf_s结合了2者的优点，只要count参数设置合理，函数就不会因缓冲区不够而失败。

所以，在C++中使用这类函数，还是应该尽可能的使用_snprintf_s才好。



###### asprintf

https://blog.csdn.net/diangangqin/article/details/100526669

asprintf()可以说是一个增强版的sprintf(),在不确定字符串的长度时，能够根据格式化的字符串长度，申请足够的内存空间。

函数原型：
int asprintf(char **strp, const char *fmt, ...);
功能：
asprintf()可以说是一个增强版的sprintf(),在不确定字符串的长度时，非常灵活方便，能够根据格式化的字符串长度，申请足够的内存空间。此外，使用完后，必须通过free()释放空间。不过，这是GNU扩展的C函数库，不是标准C函数库或者POSIX。
头文件：
asprintf.h 

**但是我实际上点击这个函数，会进入到stdio里面，而且我这个pingpong.c里面也没有引用asprintf.h这个头文件。**





sysconf()函数：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main()
{
	printf("Size of a page in bytes:%ld\n",sysconf(_SC_PAGESIZE));
	printf("Max length of a  hostname:%ld\n",sysconf(_SC_HOST_NAME_MAX));
	printf(" The maximum number of files that a process can have open at any time.:%ld\n",sysconf(_SC_OPEN_MAX));
	printf("  The  number  of  clock  ticks  per  second.:%ld\n",sysconf(_SC_CLK_TCK)); 
	printf("The number of processors currently online .:%ld\n",sysconf(_SC_NPROCESSORS_ONLN)); 
	printf("The number of processors configured..:%ld\n",sysconf(_SC_NPROCESSORS_CONF)); 
	return 0;
}
```

```c
Size of a page in bytes:4096 // 一页多少个字节
Max length of a  hostname:64
The maximum number of files that a process can have open at any time.:1024
The number of clock ticks per second.:100
The number of processors currently online .:2
The number of processors configured..:2
```

