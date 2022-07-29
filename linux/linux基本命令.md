## 基本命令

***SCP 是基于SSH来加密拷贝文件，但要知道详细目录，不可断点续传。***

```bash
# 上传到服务器
$ scp [option] /path/to/source/file user@server-ip:/path/to/destination/directory

以下是scp命令常用的几个选项：
-C - 这会在复制过程中压缩文件或目录。
-P - 如果默认 SSH 端口不是 22，则使用此选项指定 SSH 端口。
-r - 此选项递归复制目录及其内容。
-p - 保留文件的访问和修改时间。

# 下载到本地
$ scp [option] user@server-ip:/path/to/source/file /path/to/local/directory

# scp帮助
$ man scp
```



###### 将win10中的文件，上传到linux系统

```bash
# Dos中的sftp命令
# 在dos窗口中输入：
sftp cent@10.xx.xx.43 根据提示输入用户的密码
# 在sftp中输入： 
put hadoop-2.8.5.tar.gz /home/cent/Downloads/
```



### 压缩与解压

###### unzip

```bash
unzip [参数] [文件] # 默认解压到当前位置
# -d 指定要解压到的目录
unzip mydata.zip -d mydatabak 
```

**常用参数：**

| -l   | 显示压缩文件内所包含的文件                       |
| ---- | ------------------------------------------------ |
| -v   | 执行时显示详细的信息                             |
| -c   | 将解压缩的结果显示到屏幕上，并对字符做适当的转换 |
| -n   | 解压缩时不要覆盖原有的文件                       |
| -j   | 不处理压缩文件中原有的目录路径                   |



###### zip

```bash
zip [参数] [文件]
# 将mydata目录下的所有文件压缩 -r表示递归处理
zip -r mydata.zip mydata 
```

**常用参数：**

| -q             | 不显示指令执行过程                               |
| -------------- | ------------------------------------------------ |
| -r             | 递归处理，将指定目录下的所有文件和子目录一并处理 |
| -z             | 替压缩文件加上注释                               |
| -v             | 显示指令执行过程或显示版本信息                   |
| -n<字尾字符串> | 不压缩具有特定字尾字符串的文件                   |



###### gzip

gzip [参数] **不能压缩目录，而且不保留源文件**

```bash
# 会把当前目录中的每一个文件都压缩成一个 .gz的文件，目录会跳过 
$ gzip *
# -r 递归把目录也压缩，注意，目录不是生成一个压缩文件，而是压缩目录里面的每个文件，目录任然存在！！！
$ gzip -r *

# 把当前目录所有.gz文件再解压出来
$ gunzip * 
```



###### tar

tar是打包，不是压缩，压缩调用的是gzip或者bzip2

tar 选项 [压缩后文件名] [目录]

```
-c 打包
-v 显示详细信息，如操作过程，或者查看压缩文件内容，不加的话就只显示文件名，加了还可以显示权限等
-f 指定文件名,这个参数必须要
-z 打包同时压缩 生成.tar.gz
-j 使用bzip2压缩
-x 解包，解压缩也还要加上-z
-t 显示压缩文件内容
```

```bash
# 只打包当前目录中的test目录
$ tar -cvf test.tar test
# 打包tutorial文件，不显示详细过程，注意如果要指定文件名，-f不可少
$ tar -cf tutorial.tar tutorial.cxx

# 打包同时压缩
$ tar -zcf t.tar.gz test

# 解压
$ tar -zxf t.tar.gz

# 查看打包文件的内容，不加v只显示文件名
$ tar -tf t.tar
```



## LINUX杂项


##### linux只列出目录
```
ls -d */
```



##### linux配置端口转发：

在Linux的下面部署了tomcat，为了安全我们使用非root用户进行启动，但是在域名绑定时无法直接访问80端口号。众所周知，在unix下，非root用户不能监听1024以上的端口号，这个tomcat服务器就没办法绑定在80端口下。所以这里需要使用linux的端口转发机制，把到80端口的服务请求都转到8080端口上。

在root账户下面运行一下命令：

```
iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-port 8080
```

注意：网上朋友的命令dport 前面都是 一个 -，通常都会报错。另外如果防火墙重新启动，该命令就会失效。

可以使用下面的命令把该规则保存到iptables里面 ：

```
service iptables save
```



##### 端口映射：

pgw_ip:205.55.128.108 

```
iptables -t nat -A PREROUTING -p tcp --dport 31942 -j DNAT --to 205.55.128.108:31943
iptables -t nat -A POSTROUTING -p tcp -d 205.55.128.108 --dport 31943 -j SNAT --to 205.55.128.236:31942
```



```
lsmod 查看内核模块
```



显示发行信息： http://ipcmen.com/lsb_release

```bash
lsb_release

lsb_release -c -s
=====
xenial
```



### Linux一次执行多条命令

一次执行多个相互间==没有依赖==的命令，==命令之间用分号隔开==，这样前后命令的执行没有依赖性

```bash
cmd1;cmd2;cmd3
```

但是当后面的命令运行与否与前一个命令的运行结果有关，即命令之间有依赖关系，有如下两种情况：

- cmd1失败了，那就不要执行后面的命令，解决：`cmd1 && cmd2` 当cmd1执行成功，即$?为0的时候才执行cmd2，否则不执行cmd2

- cmd1失败了，才去指令后面的命令，解决： `cmd1 || cmd2`  



### Linux后面的括号里面的数字表示什么

看<<unix高级程序设计>> 等书, 常会看到 kill(1) , conect(2) , select(2) 这样的写法.

1,2 表示什么?  

```
Linux下最通用的领域及其名称及说明如下：
领域 名称 说明
1 用户命令， 可由任何人启动的。
2 系统调用， 即由内核提供的函数。
3 例程， 即[库函数](https://so.csdn.net/so/search?q=库函数&spm=1001.2101.3001.7020)。
4 设备， 即/dev目录下的特殊文件。
5 文件格式描述， 例如/etc/passwd。
6 游戏， 不用解释啦！
7 杂项， 例如宏命令包、惯例等。
8 系统管理员工具， 只能由root启动。
9 其他（Linux特定的）， 用来存放内核例行程序的文档。
n 新文档， 可能要移到更适合的领域。
o 老文档， 可能会在一段期限内保留。
l 本地文档， 与本特定系统有关的。
```

在使用man 查询这些命令的时候,  可以用下面这样的格式:

$man  数字  命令

例如 :  

```bash
$man 1 kill  #表示命令kill 
$man 2 kill  #表示系统调用 kill
```

