### LOG4CXX用法

###### [Log4j rootLogger根配置以及4种日志级别](https://www.cnblogs.com/renjiaqi/p/12192921.html)

Log4j有三个主要的组件：Loggers(记录器)，Appenders (输出源)和Layouts(布局)。

这里可简单理解为日志类别，日志要输出的地方和日志以何种形式输出。综合使用这三个组件可以轻松地记录信息的类型和级别，并可以在运行时控制日志输出的样式和位置。

#### 1、Loggers

Loggers组件在此系统中被分为六个级别： TRACE < DEBUG < INFO < WARN < ERROR < FATAL。这六个级别是有顺序的，分别用来指定这条日志信息的重要程度， Log4j有一个规则：只输出级别不低于设定级别的日志信息，假设Loggers级别设定为INFO，则INFO、WARN、ERROR和FATAL级别的日志信息都会输出，而级别比INFO低的DEBUG则不会输出。

- 每个logger都被分配了一个日志级别(log level),用来控制日志信息的输出。

- 未被分配level的logger可以继承它最近的父logger的level。
  若父logger也没有设置level，则将继承root logger的level。

- 每条输出到logger的日志请求(logging request)也都有一个level，如LOG4CXX_DEBUG
- 如果logging request的level大于等于该logger的level，则该request将被处理，否则将被忽略
  例如： logger的level是ERROR,request的level是DEBUG,则request的内容不会在日志中输出。 但若request的level是ERROR,则会在日志中输出。

#### 2、Appenders

允许把日志输出到不同的地方，如控制台（Console）、文件（Files）等，可以根据天数或者文件大小产生新的文件，可以以流的形式发送到其它地方等等。

常使用的类如下：

- org.apache.log4j.ConsoleAppender（控制台）
- org.apache.log4j.FileAppender（文件）
- org.apache.log4j.DailyRollingFileAppender （每天产生一个日志文件）
- org.apache.log4j.RollingFileAppender（文件大小到达指定尺寸的时候产生一个新的文件）
- org.apache.log4j.WriterAppender（将日志信息以流格式发送到任意指定的地方）

##### 配置模式：

```properties
log4j.appender.appenderName = className
log4j.appender.appenderName.Option1 = value1
…
log4j.appender.appenderName.OptionN = valueN
```

##### ConsoleAppender（控制台）选项

```properties
Threshold = WARN ： 指定日志信息的最低输出级别，默认为DEBUG
ImmediateFlush = true : 表示所有消息都被立即输出，设为false则不输出， 默认值为true
Target = System.err : 默认值是System.out
```

##### FileAppender（文件）选项

```properties
Threshold = WARN 
ImmediateFlush = true
Append = false : true表示消息增加到指定文件中，false则将消息覆盖指定的文件内容，默认值是true
File = D:/logs/logging.log4j : 指定消息输出的文件目录名
```

##### DailyRollingFileAppender（每天产生一个日志文件）选项

```properties
Threshold = WARN 
ImmediateFlush = true
Append = false
File = D:/logs/logging.log4j
DatePattern = '.'yyyy-MM : 每个月生成一个新的日志文件
              '.'yyyy-ww : 每周
              '.'yyyy-MM-dd:每天
              '.'yyyy-MM-dd-a:每天2次
              '.'yyyy-MM-dd-HH：每小时
              '.'yyyy-MM-dd-HH-mm : 每分钟
```

##### RollingFileAppender（文件大小到达指定尺寸的时候产生一个新的文件）选项

```properties
Threshold = WARN 
ImmediateFlush = true
Append = false
File = D:/logs/logging.log4j
MaxFileSize = 100KB ：文件达到该大小时，会自动滚动，即将原来的内容移到logging.log4j.1文件中
MaxBackupIndex = 2 : 指定可以产生滚动文件的最大数，例如，设为2则可以产生logging.log4j.1, logging.log4j.2两个滚动文件和一个log
```



#### 3、Layouts

有时用户希望根据自己的喜好格式化自己的日志输出，Log4j可以在Appenders的后面附加Layouts来完成这个功能。Layouts提供四种日志输出样式，如根据HTML样式、自由指定样式、包含日志级别与信息的样式和包含日志时间、线程、类别等信息的样式。

常使用的类如下：

> org.apache.log4j.HTMLLayout（以HTML表格形式布局）
> org.apache.log4j.PatternLayout（可以灵活地指定布局模式）
> org.apache.log4j.SimpleLayout（包含日志信息的级别和信息字符串）
> org.apache.log4j.TTCCLayout（包含日志产生的时间、线程、类别等信息）

##### 配置模式：

```properties
log4j.appender.appenderName.layout = className
log4j.appender.appenderName.layout.Option1 = value1
…
log4j.appender.appenderName.layout.OptionN = valueN
```



##### HTMLLayout选项

```properties
LocaltionInfo = true : 输出java文件名和行号，默认值是false
Title = My Logging : 默认值是Log4j Log Messages
```

##### PatternLayout选项

```properties
ConversionPattern = %m%n ： 设定以怎样的格式显示消息
```

格式化符号说明:

```properties
%p： 输出日志信息的优先级，即DEBUG,INFO,WARN,ERROR,FATAL
%d： 输出日志时间点的日期或时间，默认格式为ISO8601，也可以指定格式，eg.%d{yyyy/MM/dd HH:mm:ss,SSS}。
%r： 输出自应用程序启动到输出到该log信息耗费额毫秒数
%t： 输出产生该日志事件的线程名
%l:  输出日志事件的发生位置，相当于%c.%M(%F:%L)的组合，包括类全名、方法、文件名及在代码行中的行数。
     eg.test.TestLog4j.main(TestLog4j.java:10)。
%c: logger所属类的全名，最小长度是20，若名称长度小于20，默认右对齐。
%M: 输出产生日志信息的方法名
%F: 输出日志消息产生时所在的文件名称
%L: 输出代码中的行号
%m：输出logger的日志消息，eg.LOG4CXX_DEBUG(m_logger,"日志消息")
%n：输出一个回车换行符，windows平台为"\r\n",Unix平台为"\n"
%x: 输出和当前线程相关联的NDC(嵌套诊断环境)，尤其用到像java servlets这样的多客户多线程的应用中
%%：输出一个"%"字符

另外，还可以在%与格式字符之间加上修饰符，来控制器最小长度、最大长度、文本对齐方式。如：
1） %-20c : "-"号表示左对齐
2） %.30c : 指定%c最大长度为30，若大于30，会将左边多出来的字符截掉，但小于30也不会补空格
```



##### 简单的配置文件例子

Log4j支持两种配置文件格式，一种是XML格式的文件，一种是properties属性文件。下面以properties属性文件为例

```properties
#设置rootlogger为DEBUG级别,两个Appender
log4j.rootLogger=DEBUG, ca, fa

#对Appender fa进行设置：这是一个文件类型的Appender，
#其输出文件（File）为./output.log，
#输出方式（Append）为覆盖方式，
#输出格式（layout）为PatternLayout
log4j.appender.fa=org.apache.log4j.FileAppender
log4j.appender.fa.File=./output.log
log4j.appender.fa.Append=false
log4j.appender.fa.layout=org.apache.log4j.PatternLayout
log4j.appender.fa.layout.ConversionPattern=%d[%t] %-5p %.16c - %m%n

#对Appender ca进行设置：这是一个控制台类型的Appender
#输出格式（layout）为PatternLayout
log4j.appender.ca=org.apache.log4j.ConsoleAppender
log4j.appender.ca.layout=org.apache.log4j.PatternLayout
log4j.appender.ca.layout.ConversionPattern=%d[%t] %-5p %.16c - %m%n
```



#### 4、Log4j 根配置语法

```java
log4j.rootLogger = [level], appenderName, appenderName, … 
```

**// 注意，先配置level，然后后面的都是自己写的apperder名称**
把指定级别以上的日志信息输出到指定的一个或者多个位置

我们把INFO层级以及以上的信息输出到Console和File;
即输出到控制台和本地硬盘文件

```properties
log4j.rootLogger=INFO, Console ,File 

#Console 
log4j.appender.Console=org.apache.log4j.ConsoleAppender 
log4j.appender.Console.layout=org.apache.log4j.PatternLayout 
log4j.appender.Console.layout.ConversionPattern=%d [%t] %-5p [%c] - %m%n
    
#File
log4j.appender.File = org.apache.log4j.FileAppender
log4j.appender.File.File = d://log4j2.log
log4j.appender.File.layout = org.apache.log4j.PatternLayout
log4j.appender.File.layout.ConversionPattern =%d [%t] %-5p [%c] - %m%n
```



### SciDB LOG配置文件

###### log1.properties文件

```properties
log1.
# ##
# Levels: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
# ##

log4j.rootLogger=DEBUG, file

# examples of some individual settings
# ##
# log4j.logger.scidb.common.thread=TRACE
# log4j.logger.scidb.entry=TRACE
# log4j.logger.scidb.executor=TRACE
# log4j.logger.scidb.qproc.operator=TRACE
# log4j.logger.scidb.HabilisOptimizer=TRACE
# log4j.logger.scidb.qproc.processor=TRACE
# log4j.logger.scidb.services.network=TRACE
# log4j.logger.scidb.smgr=TRACE
# log4j.logger.scidb.smgr.chunk=TRACE
# log4j.logger.scidb.storage.bufmgr=TRACE
# log4j.logger.scidb.storage.bufmgr.bytecount=TRACE

log4j.appender.file=org.apache.log4j.FileAppender
log4j.appender.file.layout=org.apache.log4j.PatternLayout
log4j.appender.file.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss.SSSSSS} [%t] [%-5p]: %-20.20c %m%n
log4j.appender.file.File=scidb.log

log4j.rootLogger.file.File=scidb1.log
```



###### log4cxx.properties文件

```properties
###
# Levels: TRACE < DEBUG < INFO < WARN < ERROR < FATAL
###
# Loggers hierarchy for individual settings:
#
# scidb.common.thread
# scidb.services.network
# scidb.entry
# scidb.qproc.operator
# scidb.qproc.processor
# scidb.qproc.HabilisOptimizer
# scidb.qproc.pluginmanager
# scidb.executor
# scidb.statistics - logger for gathering statistics
# scidb.smgr
# scidb.smgr.chunk - logger for chunk auditing
###

log4j.rootLogger=DEBUG, file

log4j.appender.file=org.apache.log4j.RollingFileAppender
log4j.appender.file.File=scidb.log
log4j.appender.file.MaxFileSize=100MB
log4j.appender.file.MaxBackupIndex=10
log4j.appender.file.layout=org.apache.log4j.PatternLayout
log4j.appender.file.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss.SSSSSS} [%t] [%-5p]: %m%n
# 上面格式依次是：时间%d 线程名%t 日志级别%p 日志%m 换行符%n
```

一个是FileAppender，一个是RollingFileAppender



实际的scidb.log日志输出：

```bash
2022-06-23 06:25:49.000185 [0x7f06e49cf700] [TRACE]: Types canonicalization
2022-06-23 06:25:49.000185 [0x7f06e49cf700] [TRACE]: LogicalQueryPlanNode::inferTypes produced PS: 5
2022-06-23 06:25:49.000185 [0x7f06e49cf700] [TRACE]: Query parseLogical 2 finished
2022-06-23 06:25:49.000185 [0x7f06e49cf700] [INFO ]: S_defaultDistType: 1 hashed
2022-06-23 06:25:49.000185 [0x7f06e49cf700] [TRACE]: OperatorDist::setInheritedDistType: operator list _inheritedDistType hashed
```



2022年7月6日20:52:59 更改日志输出格式

```properties
log4j.rootLogger=DEBUG, file
...
# 在最后面添加一个%l 可以输出方法和行数等
log4j.appender.file.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss.SSSSSS} [%t] [%-5p] [%l]: %m%n
```

