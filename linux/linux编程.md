linux编程

Linux系统调用--getrusage函数详解

https://www.cnblogs.com/iihcy/p/5105839.html

其中 rusage结构体存放的是资源使用信息，定义在/usr/include/sys/resource.h



calloc：

```
void *calloc(size_t nitems, size_t size)
```

**malloc** 和 **calloc** 之间的不同点是，malloc 不会设置内存为零，而 calloc 会设置分配的内存为零。



###### getopt()用法详解:https://blog.csdn.net/qq_22642239/article/details/106462986

用来处理命令行参数，不用自己处理 argv的参数了



snprintf：格式化字符串

snprintf()，函数原型为[int](https://baike.baidu.com/item/int/54055) snprintf(char *str, size_t size, const char *format, ...)。

将可变参数 “…” 按照format的格式格式化为[字符串](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6%E4%B8%B2/1017763)，然后再将其拷贝至[str](https://baike.baidu.com/item/str/11045424)中。



###### getaddrinfo()函数详解：https://blog.csdn.net/fanx021/article/details/80549945

ipv6中新添加的函数，更好用，完成主机名到地址的解析