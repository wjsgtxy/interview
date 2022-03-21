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



## docker命令



