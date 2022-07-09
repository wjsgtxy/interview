#### Ubuntu下配置全局代理

打开/etc/profile文件，在末尾加上这三行(password中`.`号不用转义可以直接使用)

```
export http_proxy=http://username:password@proxy_ip:port   #代表http代理
export https_proxy=http://proxy_ip:port  #代表https代理
export ftp_proxy=http://proxy_ip:port  #代表ftp代理
```

然后执行source /etc/profile 即可。

```bash
# 测试：
curl www.google.com
wget www.google.com
```

注意不能使用ping命令，因为ping走的是icmp协议，这里是不可行的。



#### backports是什么

> 百度百科：Backport是将一个软件的*补丁*应用到比此补丁所对应的版本*更老的版本*的**行为**。
> 这里不是说某个软件，而是这个行为本身叫做Backport(一般的应用软件，代码本身并不依赖特定版本的系统)

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



#### Ubuntu开启x11-forwarding

https://www.fujieace.com/linux/ssh-x11-forwarding.html



###### 一、什么是X11-forwarding？

X11 中的 X 指的就是 X 协议；11 指的是采用 X 协议的第 11 个版本。

X11-forwarding 说的简单明白点就是：可以通过一个支持 X Server 的 SSH 客户端，例如：MobaXterm。 连接到远程 Linux 服务器，可以在本地通过 MobaXterm 运行操作一个远程 Linux 服务器上有图形界面或命令行的程序。



###### 二、X协议原理

通常来说，Linux 本身是没有图形化界面的，所谓的图形化界面系统只不过中 Linux 下的应用程序。这一点和 Windows 不一样。

Windows 从 Windows 95 开始，图形界面就直接在系统内核中实现了，是操作系统不可或缺的一部分。Linux 的图形化界面，底层都是基于 X 协议。

X 协议由 **X server** 和 **X client** 组成：

X server 管理主机上与显示相关的硬件设置（如显卡、硬盘、鼠标等），它负责屏幕画面的绘制与显示，以及将输入设置（如键盘、鼠标）的动作告知 X client。

X client (即 X 应用程序) 则主要负责事件的处理（即程序的逻辑）。

举个例子：

如果用户点击了鼠标左键，因为鼠标归 X server 管理，于是 X server 就捕捉到了鼠标点击这个动作，然后它将这个动作告诉 X client，因为 X client 负责程序逻辑，于是 X client 就根据程序预先设定的逻辑（例如，画一个圆），告诉 X server 说：“请在鼠标点击的位置，画一个圆”。最后，X server 就响应 X client 的请求，在鼠标点击的位置，绘制并显示出一个圆。



###### 三、使Linux支持X11-Forwarding

```bash
# 安装x11支持 
sudo apt-get install libx11-dev libxext-dev libxtst-dev libxrender-dev libxmu-dev libxmuu-dev

# /etc/ssh/sshd_config 中 x11 forward 参数设置为yes，ubuntu20.04我看默认是开启了的

# 安装x11-apps
apt install x11-apps

# 测试：运行xclock
xclock （显示时钟）
xclock -d (显示数字)

# 测试：安装并运行firefox，这样就可以本地打开图形化界面了
apt install firefox
firefox
```



#### Ubuntu设置时间

```bash
# 查看时区相关信息
timedatectl
# 如果是容器中，可能没有这个命令，需要自己安装，12MB左右 timedatectl is part of systemd package.
apt install systemd
# 查看时区列表
timedatectl 
# 设置时区，之后date输出的，就显示中国时区了，从UTC 更改为 CST（中国标准时间）
timedatectl set-timezone Asia/Shanghai
# 图形化设置时区
dpkg-reconfigure tzdata
# 启动时间同步
timedatectl set-ntp true
# 将date输出改为24小时格式 添加一行LC_TIME
vi /etc/default/locale
LC_TIME=en_DK.UTF-8
```



#### Ubuntu systemctl

https://blog.csdn.net/skh2015java/article/details/94012643



### 添加开机启动脚本

update-rc.d命令用于安装或移除System-V风格的初始化脚本连接。脚本是存放在 /etc/init.d/目录下的，我们可以直接在/etc/init.d/目录下创建执行脚本，然后在rcN.d或者rcS.d中创建软链接，系统启动时会从rcS-rc0…rc6顺序启动文件夹里的启动脚本

- rcN 0（关闭系统） 
- 1（单用户模式，只允许root用户对系统进行维护。） 
- 2 到 5（多用户模式，其中3为字符界面，5为图形界面。） 
- 6（重启系统）

/etc 目录下面有 rc0.d 到 rc6.d 加上rcS.d，对应不同的系统启动模式。




1、进入 /etc/init.d/ 目录

```bash
[root@Ubuntu ~]# cd /etc/init.d/
```

2、新建一个自定义名称的sh脚本，这里以 xxx 名称为例建立一个 xxx.sh 的脚本

```bash
[root@Ubuntu /etc/init.d/]# vi softroce_init.sh
```

```bash
#!/bin/bash
### BEGIN INIT INFO
# Provides:          softroce_init.sh
# Required-start:    $local_fs $remote_fs $network $syslog
# Required-Stop:     $local_fs $remote_fs $network $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: starts softroce_init.sh
# Description:       starts softroce_init.sh
### END INIT INFO

# 这里为需要执行的命令
echo "dz start soferoce rdma_rxe"
modprobe rdma_rxe
rxe_cfg add ens32
```


注：脚本内容必须包含 ### BEGIN INIT INFO   ......    ### END INIT INFO   不然会报错误：missing LSB tags and overrides

3、赋予脚本权限

```bash
[root@Ubuntu /etc/init.d/]# sudo chmod 775 ./softroce_init.sh        # xxx为你的脚本文件名 
```

4、然后执行以下命令

```bash
[root@Ubuntu /etc/init.d/]# sudo update-rc.d softroce_init.sh defaults 90        # xxx为你的脚本文件名
```

5、最后重启系统即可：

```bash
reboot
```

6、查看服务列表

```bash
sudo service --status-all # 确实在输出里面看到了 [ + ]  softroce_init.sh
```

这时应该能看到新加开机启动脚本（new_service.sh）的名字在列表中；说明开机时会启动这个sh脚本的。
附加：看到列表中，启动前面会可能出现以下三种状态de

- [+] –具有此标志的服务当前正在运行。
- [–] –具有此标志的服务当前未运行。
- [?] –没有 状态开关的服务。



附：取消开机启动脚本

```bash
[root@Ubuntu /etc/init.d/]# sudo update-rc.d -f softroce_init.sh remove         # 移除脚本，XXX为你的脚本文件名
```





#### 查看命令在哪里

```
locate pg_ctl
```

