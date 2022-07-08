# Vmware安装Ubuntu

## 安装ubuntu

https://www.linuxidc.com/Linux/2020-03/162547.htm

安装过程中安装mini系统就好，不需要安装full的

安装完成后，首先关闭自动锁屏，要不很烦，路径为：settings->privacy->automatic screen lock 关闭

其次关闭自动更新：点击软件与更新，进入更新选项卡，将自动检查更新设置为从不



## 开启root用户

默认是不开始root用户的，su切换过去会报错，需要自己设置root密码开启

```
sudo passwd root
```



## 安装vmtools

一开始屏幕很小，要使ubuntu屏幕变大，需要安装vmtools，菜单栏选择安装之后，去到CDROM里面找到压缩包复制到tmp目录之后解压，然后

```
sudo ./vmware-install.pl
```

之后一路回车就可以了

注意：安装vmtools之后发现还是无法复制文字到ubuntu里面，需要重启一下ubuntu以及虚拟机！



## 安装VIM

在vi编辑文件的时候，发现上下左右键会输入ABCD等字母，完全错乱了，安装vim之后再编辑就ok了

```
sudo apt-get install vim
```



## 配置源

先配置源，否则后面安装软件太慢：


```bash
# 先备份
sudo cp /etc/apt/sources.list  /etc/apt/sources.list.bak
# 再编辑(直接删除重建还方便一点)
sudo vi /etc/apt/source.list
# 最后更新
sudo apt-get update
```



上面使用阿里云的源（https://developer.aliyun.com/mirror/ubuntu?spm=a2c6h.13651102.0.0.3e221b11sjcRPQ）
18.04的配置如下：（每个ubuntu版本的配置是不一样的！！！）
下面的十条主要区别也在于不同的源包含的软件不一样，有的是官方支持的，有的是第三方的
source注释掉，可以加快一些速度

```
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
#deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse

deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
#deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse

deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
#deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse

deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
#deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse

deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
#deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
```

16.04：
```
deb http://mirrors.aliyun.com/ubuntu/ xenial main
#deb-src http://mirrors.aliyun.com/ubuntu/ xenial main

deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main
#deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main

deb http://mirrors.aliyun.com/ubuntu/ xenial universe
#deb-src http://mirrors.aliyun.com/ubuntu/ xenial universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe
#deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates universe

deb http://mirrors.aliyun.com/ubuntu/ xenial-security main
#deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main
deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe
#deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security universe

```



## 安装net-tools

此时ifconfig还无法使用查看虚拟机ip，要安装net-tools才能使用这个命令

```
sudo apt-get install net-tools
```



## 安装ssh

这时候用moba是连接不上虚拟机中的ubuntu的，没有装ssh
```
sudo apt-get install openssh-server openssh-client
```




## 开启root用户远程登录
默认是禁止root用户ssh远程登录的
1. 编辑sshd_config文件
```
vi /etc/ssh/sshd_config
```

2. 找到PermitRootLogin 添加一行 PermitRootLogin yes
```
# Authentication:

#LoginGraceTime 2m
#PermitRootLogin prohibit-password
PermitRootLogin yes
#StrictModes yes
#MaxAuthTries 6
#MaxSessions 10
```

3. 重启ssh服务

```
/etc/init.d/ssh restart
```




## 安装开发工具

#### 一键安装c++工具

Ubuntu缺省情况下，并没有提供C/C++的编译环境，因此还需要手动安装。但是如果单独安装gcc以及g\++比较麻烦，幸运的是，Ubuntu提供了一个build-essential软件包。

查看该软件包的依赖关系：

```bash
root@uvm1:/usr/bin# apt-cache depends build-essential
build-essential
 |Depends: libc6-dev
  Depends: <libc-dev>
    libc6-dev
  Depends: gcc
  Depends: g++
  Depends: make
    make-guile
  Depends: dpkg-dev
```


然后直接安装：
```
sudo apt-get install build-essential
```



#### 安装Python

```
sudo apt-get install python3.8
```



#### 其他软件的安装

https://blog.csdn.net/gatieme/article/details/44917753



#### Vmware克隆虚拟机

注意，克隆之后会出现ip冲突，需要重新设置网卡，然后修改主机名称：

```bash
vi /etc/hostname
reboot
```



#### 直接配置ip地址

克隆虚拟机之后ip老是冲突，两个虚拟机总是一个ip，不知道为什么，命名主机名称和网卡mac都更改过来，还是一样。

```
vim /etc/netplan/00-installer-config.yaml
```

vm1: 192.168.40.129

```yaml
# 32网卡是vmnet1, 35是nat vmnet8
network:
  ethernets:
    ens32:
      addresses: [192.168.40.129/24]
      dhcp4: false
      gateway4: 192.168.40.1
      nameservers:
         addresses: [192.168.40.1]  
    ens35:
      addresses: [192.168.80.135/24]
      dhcp4: false
      gateway4: 192.168.80.2
      nameservers:
         addresses: [192.168.80.2]
  version: 2
```



vm2: 192.168.40.130

```yaml
# 32网卡是vmnet1, 35是nat vmnet8
network:
  ethernets:
    ens32:
      addresses: [192.168.40.130/24]
      dhcp4: false
      gateway4: 192.168.40.1
      nameservers:
         addresses: [192.168.40.1]  
    ens35:
      addresses: [192.168.80.136/24]
      dhcp4: false
      gateway4: 192.168.80.2
      nameservers:
         addresses: [192.168.80.2]
  version: 2
```



修改完毕，保存并退出，执行下面的指令使配置生效

```bash
netplan apply
```

