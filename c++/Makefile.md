makefile文件中

LIBS添加库文件，LDFLAGS指定位置



warning: Clock skew detected. Your build may be incomplete

其实是因为我的文件是windows直接同步到ubuntu上面的，时钟有些不同步，make clean之后重新make就OK了