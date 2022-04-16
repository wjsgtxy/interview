#### ubuntu下配置全局代理

打开/etc/profile文件，在末尾加上这三行(password中`.`号不用转义可以直接使用)

```
export http_proxy=http://username:password@proxy_ip:port   #代表http代理
export https_proxy=http://proxy_ip:port  #代表https代理
export ftp_proxy=http://proxy_ip:port  #代表ftp代理
```

然后执行source /etc/profile 即可。

测试：

```
curl www.google.com
```

 或者:

```
wget www.google.com
```

注意不能使用ping命令，因为ping走的是icmp协议，这里是不可行的。



#### backports是什么

百度百科：Backport是将一个软件的*补丁*应用到比此补丁所对应的版本*更老的版本*的**行为**。
这里不是说某个软件，而是这个行为本身叫做Backport(一般的应用软件，代码本身并不依赖特定版本的系统)

比方说 16.10推出后，已经在16.10中运行的软件，会被backports到16.04中，甚至是14.04中。
那么，一般情况下，会有哪些软件会被backports到以前版本呢？



#### 添加keys

```
curl -sSL 'http://keyserver.ubuntu.com/pks/lookup?op=get&search=0xC1CF6E31E6BADE8868B172B4F42ED6FBAB17C654' | sudo apt-key add -
```



#### 提示NO_PUBKEY：1FE11567623A70C9

```bash
wget -qO --no-check-certificate - https://downloads.paradigm4.com/key | sudo apt-key add -
```

如果有代理好像上面的命令无法成功，所以还可以手动访问这个网址，得到一串key值

```bash
vi paradigmkey
apt-key add paradigmkey
# 查看key
apt-key list
# 发现存在上面的这个 70C9 的key了
cat /etc/apt/trusted.gpg
--------------------
pub   rsa2048 2012-10-25 [SC]
      FBD5 6460 30ED BF14 D261  79A3 1FE1 1567 623A 70C9
uid           [ unknown] SciDB Autosign key
sub   rsa2048 2012-10-25 [E]
```



#### OpenPGP keyserver:

https://keyserver.ubuntu.com/



#### ubuntu开启x11-forwarding

https://blog.csdn.net/weixin_41668084/article/details/113361765

##### 安装x11

```bash
# 安装x11支持 
sudo apt-get install libx11-dev libxext-dev libxtst-dev libxrender-dev libxmu-dev libxmuu-dev

# /etc/ssh/sshd_config 中 x11 forward 参数设置为yes，ubuntu20.04我看默认是开启了的

# 安装x11-apps
apt install x11-apps

# 运行xclock
xclock （显示时钟）
xclock -d (显示数字)

# 安装并运行firefox
apt install firefox
firefox
```

