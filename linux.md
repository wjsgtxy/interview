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

## docker命令



