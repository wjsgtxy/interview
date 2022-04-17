### OpenSSH的安装、使用

因为SSH受版权和加密算法的限制，现在很多人都转而使用OpenSSH。OpenSSH（Open Secure Shell，开放安全Shell）是SSH的替代软件，而且是免费的。OpenSSH默认使用RSA密钥，它采用安全、加密的网络连接工具代替telnet、ftp、rlogin、rsh和rcp工具。

#### 1、安装

###### 1.1 服务器端安装

openssh-server：服务器端
openssh：服务器端与客户端核心文件

###### 1.2 客户端安装

openssh：服务器端与客户端核心文件
openssh-clients : 客户端

可通过如下命令找到上述软件：

```
[root@localhost ~]# yum search openssh
openssh.x86_64 : An open source implementation of SSH protocol versions 1 and 2
openssh-clients.x86_64 : An open source SSH client applications
openssh-server.x86_64 : An open source SSH server daemon
```

然后采用如下命令安装相关软件：

```bash
[root@localhost ~]# yum install openssh.x86_64
```

**注意：如果一台机器既要做客户端又要做服务器端，就需要在该系统中安装上述三个软件包；CentOS 7中默认已安装好上述三个软件包。**

#### 2、使用

##### 2.1 远程登陆主机

###### 1、远程主机登陆(首次登陆)

```
[hadoop@localhost ~]$ ssh 192.168.112.130
The authenticity of host '192.168.112.130 (192.168.112.130)' can't be established.
ECDSA key fingerprint is ff:7b:94:49:f5:2d:d3:59:23:c7:a8:cf:b0:d6:e4:b9.
Are you sure you want to continue connecting (yes/no)? yes # 再次登陆不会有这个提示
Warning: Permanently added '192.168.112.130' (ECDSA) to the list of known hosts.
hadoop@192.168.112.130's password: 
```

###### 2、指定用户名登陆

```
[hadoop@localhost home]$ ssh -l root 192.168.112.130
root@192.168.112.130's password: 
Last login: Sat Jun  9 15:18:44 2018 from 192.168.112.131
```

##### 2.2 远程拷贝

###### 1、本地文件拷贝到远程主机

```
[root@localhost ~]# scp anaconda-ks.cfg root@192.168.112.131:/tmp
root@192.168.112.131's password: 
anaconda-ks.cfg                                         100% 1704     1.7KB/s   00:00
```

###### 2、远程主机文件拷贝到本地

```
[root@localhost ~]# scp  root@192.168.112.130:/tmp/anaconda-ks.cfg  /root
root@192.168.112.130's password: 
anaconda-ks.cfg                   100% 1704     1.7KB/s   00:00 
```



## SSH使用秘钥登录

本文基于实际Linux管理工作，实例讲解工作中使用ssh证书登录的实际流程，讲解ssh证书登录的配置原理，基于配置原理，解决实际工作中，windows下使用[SecureCRT](https://so.csdn.net/so/search?q=SecureCRT&spm=1001.2101.3001.7020)证书登录的各种问题，以及实现hadoop集群部署要求的无密码跳转问题。

ssh**有密码登录和证书登录**，初学者都喜欢用密码登录，甚至是root账户登录，密码是123456。但是在实际工作中，尤其是互联网公司，基本都是证书登录的。内网的机器有可能是通过密码登录的，但在外网的机器，如果是密码登录，很容易受到攻击，真正的生产环境中，ssh登录都是证书登录。

#### 证书登录的步骤

1.客户端生成证书:[私钥](https://so.csdn.net/so/search?q=私钥&spm=1001.2101.3001.7020)和公钥，然后私钥放在客户端，妥当保存，一般为了安全，访问有黑客拷贝客户端的私钥，客户端在生成私钥时，会设置一个密码，以后每次登录ssh服务器时，客户端都要输入密码解开私钥(如果工作中，你使用了一个没有密码的私钥，有一天服务器被黑了，你是跳到黄河都洗不清)。

2.服务器添加信用公钥：把客户端生成的公钥，上传到ssh服务器，添加到指定的文件中，这样，就完成ssh证书登录的配置了。

假设客户端想通过私钥要登录其他ssh服务器，同理，可以把公钥上传到其他ssh服务器。

真实的工作中:员工生成好私钥和公钥(千万要记得设置私钥密码)，然后把公钥发给运维人员，运维人员会登记你的公钥，为你开通一台或者多台服务器的 权限，然后员工就可以通过一个私钥，登录他有权限的服务器做系统维护等工作，所以，员工是有责任保护他的私钥的，如果被别人恶意拷贝，你又没有设置私钥密 码，那么，服务器就全完了，员工也可以放长假了。

#### 客户端建立私钥和公钥

在客户端终端运行命令

> ssh-keygen -t rsa

rsa是一种密码算法，还有一种是dsa，证书登录常用的是rsa。

假设用户是blue, 执行 ssh-keygen 时，才会在我的home目录底下的 .ssh/ 这个目录里面产生所需要的两把 Keys ，分别是**私钥** (id_rsa) 与**公钥** (id_rsa.pub)。

另外就是私钥的密码了，如果不是测试，不是要求无密码ssh，那么对于passphrase，不能输入空(直接回车),要妥当想一个有特殊字符的密码。

#### ssh服务端配置

ssh服务器配置如下:

```bash
vim /etc/ssh/sshd_config 
#禁用root账户登录，非必要，但为了安全性，请配置 
PermitRootLogin no 
# 是否让 sshd 去检查用户家目录或相关档案的权限数据， 
# 这是为了担心使用者将某些重要档案的权限设错，可能会导致一些问题所致。 
# 例如使用者的 ~.ssh/ 权限设错时，某些特殊情况下会不许用户登入 
StrictModes no 
# 是否允许用户自行使用成对的密钥系统进行登入行为，仅针对 version 2。 
# 至于自制的公钥数据就放置于用户家目录下的 .ssh/authorized_keys 内 
RSAAuthentication yes 
PubkeyAuthentication yes 
AuthorizedKeysFile      %h/.ssh/authorized_keys 
# 有了证书登录了，就禁用密码登录吧，安全要紧 
PasswordAuthentication no 

# 配置好ssh服务器的配置了，那么我们就要把客户端的公钥上传到服务器端，然后把客户端的公钥添加到authorized_keys 

# 在客户端执行命令 
scp ~/.ssh/id_rsa.pub blue@<ssh_server_ip>:~ 

# 在服务端执行命令 
cat  id_rsa.pub >> ～/.ssh/authorized_keys 

# 如果有修改配置/etc/ssh/sshd_config，需要重启ssh服务器  
systemctl restart sshd
```

dz: 疑问，上面将他人的公钥放到 authorized_keys中，是否需要root用户权限？



#### 客户端通过私钥登录ssh服务器

```bash
# ssh命令
ssh -i /blue/.ssh/id_rsa blue@<ssh_server_ip>

# scp命令
scp -i /blue/.ssh/id_rsa filename blue@<ssh_server_ip>:/blue

#每次敲命令，都要指定私钥，是一个很繁琐的事情，所以我们可以把私钥的路径加入ssh客户端的默认配置里
修改/etc/ssh/ssh_config

#其实默认id_rsa就已经加入私钥的路径了,这里只是示例而已
IdentityFile ~/.ssh/id_rsa
#如果有其他的私钥，还要再加入其他私钥的路径
```



### 实操：

两台虚拟机 vm1, vm2

我再vm1上面用root用户生成了公钥和私钥，通过scp将公钥放到了vm2的root用户目录下面的 authorized_keys文件里面

之后再vm1上面使用 ssh vm2_ip 可以直接登录 vm2的root用户，但是ssh xy@vm2就不行，需要你输入密码

然后我再 vm1上面切换到xy用户，另外生成了该用户的公钥和私钥，将公钥放到了vm2的authorized_keys中，发现在vm1的xy用户可以直接登录vm2的root用户。



也就是说，是谁的公钥，添加到keys中，就可以登录该机器的root用户。因为公钥最后面有用户名称和机器！比如root@vm1，xy@vm1



将ssh root用户登录禁用

```bash
# 切换用户 vm2机器上面
su xy
cd /home/xy/
ssh-keygen

# 手动在用户xy的ssh文件夹中创建authorized_keys文件，默认是没有的
vim .ssh/authorized_keys #将ssh-rsa密钥填进去

# 更改权限，否则没法使用密钥登录
chmod 755 .ssh/
drwxr-xr-x 2 xy   xy   4096 Jan 18 12:40 .ssh/

chmod 644 .ssh/authorized_keys

# 回到root权限下
exit 

vim /etc/ssh/sshd_config #修改ssh配置文件

PermitRootLogin no #禁止root通过ssh登录（默认就是no）
PermitEmptyPasswords no #禁止密码为空的帐号登录（默认就是这个）
PasswordAuthentication no #禁止使用密码登录 （默认是yes:可以使用密码登录）

# 重启ssh
systemctl restart sshd
此时就可以以xy用户进入服务器，su root切换权限
```

**注意：重启之后，当前窗口不要关闭（当前窗口还保持着连接），新开个窗口使用xy用户连接，登录成功没有问题了，再关闭当前窗口，以防改错配置，进不去服务器！**

上面操作之后，发现在vm1上面 ssh vm2_ip，就只能登录到xy用户了，不能登录到root用户了，之前都是直接登录到root用户的。ssh root@vm2_ip，会让你输入密码了，输入密码之后，提示

Permission denied, please try again. 

因为已经禁止root用户登录了。