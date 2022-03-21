## apt源学习

官方Debian Repository Format：

https://wiki.debian.org/DebianRepository/Format#Origin

跟着阿里云的目录查看上面那篇文章：

http://mirrors.aliyun.com/ubuntu/



很好[debian软件源sources.list文件格式说明](https://www.cnblogs.com/chendeqiang/p/14312725.html)

##### [APT仓库目录和repository目录结构](https://www.cnblogs.com/gpysir/p/15160807.html)

### 配置文件路径：

/etc/apt/sources.list

/etc/apt/sources.list.d

### 阿里云源：

```bash
deb http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse

deb-src http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
```

整个结构是遵循以下格式：

```bash
deb/deb-src [ options ] uri suite [component1] [component2] [...]
```

deb 这一栏只有两种写法，分别为deb与deb-src，前者表示所指向的为存放 binary 格式 ( 编译好执行文件的软件套件 ) 软件套件的服务器位置，后者则为 sources 格式 ( 包含原始码的软件套件 ) 软件套件的服务器位置。 简言之，一般我们只想要安装该软件而不想要去重新编译它时，就在这一栏填入deb， 如果你想取得某软件的原始码 ( sources code ) ，就得加入deb-src， 当然也可以两行都写。

uri 指的就是软件套件来源位置，这些位置可以为系统内的某个位置 ( file ) 、可以为 CD-ROM ( cdrom ) 、可以为网络远程位置 ( http 、 ftp )..等等，当你下 apt-get 指令时，就会到这些位置去搜寻软件数据库。总括来说一共有 file 、 cdrom 、 http 、 ftp 、copy 、rsh 、ssh 等几个参数，当然，用的最多的是http/https/ftp，从网上指定源获取软件包。

suite(dz: 英文表示一套)指你打开uri链接，访问到dists目录下有个你对应linux发行版的套件名称目录（每个发行版对应的名称各不相同，ubuntu14.04为trusty），后面多个component1为suite目录下的多个指定组件库（上面示例为main、restricted、universe 、multiverse）



### 源中的目录：

/dists/ 目录包含"发行版"(distributions), 此处是获得 Debian 发布版本(releases)和已发布版本(pre-releases)的软件包的正规途径. 有些旧软件包及 packages.gz 文件仍在里面.

/pool/ 目录为软件包的物理地址. 软件包均放进一个巨大的 "池子(pool)", 按照源码包名称分类存放. 为了方便管理, pool 目录下按属性再分类("main", "contrib" 和 "non-free"), 分类下面再按源码包名称的首字母归档. 这些目录包含的文件有: 运行于各种系统架构的二进制软件包, 生成这些二进制软件包的源码包.你可以执行命令 apt-cache showsrc mypackagename, 查看 'Directory:' 行获知每个软件包的存放位置. 例如: apache 软件包存放在 pool/main/a/apache/ 目录中.另外, 由于lib*软件包数量巨大, 它们以特殊的方式归档: 例如, libpaper 软件包存放在 pool/main/libp/libpaper/.

还有一些目录:

/tools/:用于创建启动盘, 磁盘分区, 压缩/解压文件, 启动 Linux 的 DOS 下的小工.
/doc/:
基本的 Debian 文档, 如 FAQ, 错误报告系统指导等..
/indices/:
维护人员文件和重载文件.
/project/:
大部分为开发人员的资源, 如: project/experimental/ 本目录包含了处于开发中的软件包和工具, 它们均处于 alpha 测试阶段. 用户不应使用这些软件, 因为即使是经验丰富的用户也会被搞得一团糟.）



第三部分表示你的debian版本号（注意，不是某个软件的版本号，而是debian本身的版本号），也有人称这是对debian软件包的一种水平划分（这种说法还是比较形象的），具体这一项怎么写，可以参看http://mirrors.163.com/debian/dists/网页里面的内容（如下图），大致来说按版本的旧-->新程度有这么几大类：oldstable，stable，testing，sid



1.oldstable顾名思义，就是以前的稳定发行版了，在本例中oldstable就是Debian6.0.8目录的一个链接，两者内容完全一致。

2.stable是指现在的稳定发行版，对于每个debian来说，每个稳定发行版都会有一个代号，如debian 6叫做squeeze，debian 7叫做wheezy，由于在笔者写本文的时候稳定版是debian7，所以stable就是wheezy的一个链接。

3.testing指目前还暂时处于测试阶段的debian发行版，如wheezy往下发展就是jessie了，所以testing代指的就是jessie目录，它是jessie的一个链接。

4.unstable那就是不稳定版本了，但在这个目录清单里我们并没有看到unstable目录，其实它还有一个代号：sid，我们知道debian的发行版本名称大都来自玩具总动员中的角色名称，而sid就是那个专门破坏玩具的邻居家坏小孩，所以就把unstable这个不稳定的软件目录起名为sid了，这里面的软件一般都是最新的，稳定性、安全稳定值得商榷，如果你想做试验小白鼠，大可使用这里的软件。经过一段时间的测试，这里面的软件可能会进入下一个阶段testing目录从而出现在下一个debian发行版中，要么就夭折被直接抛弃了。



### 第四部分

好了，从水平的角度介绍完debian发行版，现在我们进入这些目录大致看一下里面的内容（以stable目录为例）：
我们会发现每个目录都会包括contrib、main、non-free等几个目录，那么这些目录又是什么东东呢？

以垂直的观点来看 Debian 软件的分布：main、contrib、non-free、non-us
因为 Debian 是非营利组织，但是组织架构严谨，有一套完善的软件管理方式。基于其对软件 free 度的一种坚持，对不同版权软件包的录入有一些限定。
下面是对它们的一些简要介绍：
main Debian 里最基本及主要且符合自由软件规范的软件 ( packages )。
contrib 这里头软件虽然可以在 Debian 里头运作，即使本身属于自由软件但多半却是相依于非自由 ( non-free ) 软件。
non-free 不属于自由软件范畴的软件。
non-us 这个分类里头的软件都来自非美国地区，当中可能有牵扯到专利、加密..等等问题。
marillat 对应 Christian Marillat 的软件仓库，包括mplayer, transcode等。
rareware 对应 rarewares.org 的软件仓库, 包括很多音效程序，如lame, musepack, beep media player等。
ustc 对应 debian@ustc 维护的一些软件包，如 mule-gbk, gaim-openq, scim, stardict dicts, patched xpdf, irssi, xmms。
java 对应 Blackdown java。包括 j2re, j2sdk ,mozilla java plugin。
firefly 对应打过firefly补丁的包，包括 fontconfig mozilla mozilla-firefox pango1.0 qt-x11-free xft
misc 对应其它无分类的包，包括 nvidia-kernel, winex3, rox, chmsee等