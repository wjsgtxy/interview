配置RXE网卡

```bash
sudo rdma link add rxe_0 type rxe netdev enp0s8
```

实际上面生成的网卡名称不叫rxe_0，而是叫rocep0s8，不知道为什么，后面发现p0s8是网卡名称后4位

```bash
# 查看设备列表
root@xyvm2:/usr# ibv_devices
    device                 node GUID
    ------              ----------------
    rocep0s8            0a0027fffedabfb9

# 查看设备详细信息
root@xyvm2:/usr# ibv_devinfo
hca_id: rocep0s8
        transport:                      InfiniBand (0)
        fw_ver:                         0.0.0
        node_guid:                      0a00:27ff:feda:bfb9
        sys_image_guid:                 0a00:27ff:feda:bfb9 #dz dabfb9是mac后6位
        vendor_id:                      0x0000
        vendor_part_id:                 0
        hw_ver:                         0x0 // hardware version
        phys_port_cnt:                  1
                port:   1
                        state:                  PORT_ACTIVE (4)
                        max_mtu:                4096 (5)
                        active_mtu:             1024 (3)
                        sm_lid:                 0
                        port_lid:               0
                        port_lmc:               0x00
                        link_layer:             Ethernet
                        
root@xyvm2:/usr# ifconfig enp0s8
enp0s8: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.56.102  netmask 255.255.255.0  broadcast 192.168.56.255
        ether 08:00:27:da:bf:b9  txqueuelen 1000  (Ethernet)
        RX packets 2006488  bytes 128936386 (128.9 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 2005975  bytes 128838663 (128.8 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
              
```



perftest测试：

```bash
# 101机器上
ib_send_bw -d rocep0s8
# 102机器上
ib_send_bw -d rocep0s8 192.168.56.101
```







| 软件包名          | 主要功能                           |
| ----------------- | ---------------------------------- |
| libibverbs1       | ibverbs动态链接库                  |
| ibverbs-utils     | ibverbs示例程序                    |
| librdmacm1        | rdmacm动态链接库                   |
| libibumad3        | ibumad动态链接库                   |
| ibverbs-providers | ibverbs各厂商用户态驱动（包括RXE） |
| rdma-core         | 文档及用户态配置文件               |

查看包的安装位置：

```bash
# ibverbs动态链接库
root@xyvm2:/home/# dpkg -L libibverbs1
-----------输出begin---------------
/.
/usr
/usr/lib
/usr/lib/x86_64-linux-gnu
/usr/lib/x86_64-linux-gnu/libibverbs.so.1.8.28.0
/usr/share
/usr/share/doc
/usr/share/doc/libibverbs1
/usr/share/doc/libibverbs1/changelog.Debian.gz
/usr/share/doc/libibverbs1/copyright
/usr/share/doc/libibverbs1/libibverbs.md
/usr/lib/x86_64-linux-gnu/libibverbs.so.1
-----------输出end-----------------

# 自己通过dpkg -l 发现还安装了libibverbs-dev, 提示是：Development files for the libibverbs library
root@xyvm2:/home/# dpkg -L libibverbs-dev
-----------输出begin---------------
...
/usr/include/infiniband/mlx5_api.h # 安装了很多头文件
/usr/include/infiniband/mlx5_user_ioctl_verbs.h
/usr/include/infiniband/mlx5dv.h
/usr/include/infiniband/opcode.h
/usr/include/infiniband/sa-kern-abi.h
/usr/include/infiniband/sa.h
/usr/include/infiniband/tm_types.h
/usr/include/infiniband/verbs.h
/usr/include/infiniband/verbs_api.h
/usr/lib
/usr/lib/x86_64-linux-gnu
/usr/lib/x86_64-linux-gnu/libbnxt_re-rdmav25.a # 还安装了很多静态库文件
/usr/lib/x86_64-linux-gnu/libcxgb4-rdmav25.a
/usr/lib/x86_64-linux-gnu/libefa.a
/usr/lib/x86_64-linux-gnu/libhfi1verbs-rdmav25.a
/usr/share/man/man3/ibv_modify_qp.3.gz # 安装了很多man文档，解释ibv的api等
...
-----------输出end-----------------

# ibverbs示例程序 这些事例程序，全部是rdma项目里面libibverbs目录里面examples子目录里面的程序，该目录里面的CMakeLists.txt里面也包含了这些可执行文件，最后生成一个静态库static ibverbs_tools
root@xyvm2:/home/# dpkg -L ibverbs-utils
-----------输出begin---------------
/usr/bin/ibv_asyncwatch
/usr/bin/ibv_devices # 查看设备的程序
/usr/bin/ibv_devinfo # 查看设备详细信息
/usr/bin/ibv_rc_pingpong
/usr/bin/ibv_srq_pingpong
/usr/bin/ibv_uc_pingpong
/usr/bin/ibv_ud_pingpong
/usr/bin/ibv_xsrq_pingpong
/usr/share/doc/ibverbs-utils
/usr/share/doc/ibverbs-utils/copyright
...
/usr/share/man/man1/ibv_asyncwatch.1.gz
/usr/share/man/man1/ibv_devices.1.gz
/usr/share/man/man1/ibv_devinfo.1.gz
/usr/share/man/man1/ibv_rc_pingpong.1.gz
/usr/share/man/man1/ibv_srq_pingpong.1.gz
/usr/share/man/man1/ibv_uc_pingpong.1.gz
/usr/share/man/man1/ibv_ud_pingpong.1.gz
/usr/share/man/man1/ibv_xsrq_pingpong.1.gz
/usr/share/doc/ibverbs-utils/changelog.Debian.gz
-----------输出end-----------------

# rdmacm动态链接库
root@xyvm2:/home/# dpkg -L librdmacm1
-----------输出begin---------------
/usr/lib/x86_64-linux-gnu
/usr/lib/x86_64-linux-gnu/librdmacm.so.1.2.28.0
/usr/lib/x86_64-linux-gnu/rsocket
/usr/lib/x86_64-linux-gnu/rsocket/librspreload.so
/usr/share/doc/librdmacm1
/usr/share/doc/librdmacm1/copyright
/usr/share/doc/librdmacm1/librdmacm.md
/usr/lib/x86_64-linux-gnu/librdmacm.so.1
/usr/lib/x86_64-linux-gnu/rsocket/librspreload.so.1
/usr/lib/x86_64-linux-gnu/rsocket/librspreload.so.1.0.0
/usr/share/doc/librdmacm1/changelog.Debian.gz
-----------输出end-----------------

# ibumad动态链接库
root@xyvm2:/home/# dpkg -L libibumad3
-----------输出begin--------------- 简化了
/usr/lib/x86_64-linux-gnu/libibumad.so.3.1.28.0
/usr/share/doc/libibumad3/copyright
-----------输出end-----------------

# ibverbs各厂商用户态驱动（包括RXE）
root@xyvm2:/home/# dpkg -L ibverbs-providers
-----------输出begin--------------- 简化了
/etc/libibverbs.d/mlx4.driver 厂商驱动
/etc/libibverbs.d/mlx5.driver
/etc/libibverbs.d/mthca.driver
...
/usr/lib/x86_64-linux-gnu/libibverbs/librxe-rdmav25.so
/usr/lib/x86_64-linux-gnu/libibverbs/libvmw_pvrdma-rdmav25.so
/usr/lib/x86_64-linux-gnu/libmlx4.so.1.0.28.0
/usr/lib/x86_64-linux-gnu/libmlx5.so.1.12.28.0
...
-----------输出end-----------------

# 文档及用户态配置文件
root@xyvm2:/home/# dpkg -L rdma-core
-----------输出begin---------------
...
/etc/rdma/modules/infiniband.conf
/etc/rdma/modules/iwarp.conf
/etc/rdma/modules/iwpmd.conf
/etc/rdma/modules/opa.conf
/etc/rdma/modules/rdma.conf
/etc/rdma/modules/roce.conf
...
/usr/share/doc/rdma-core/MAINTAINERS.gz
/usr/share/doc/rdma-core/README.md.gz
/usr/share/doc/rdma-core/rxe.md
...
-----------输出end-----------------
```



查看包的一些信息：

```bash
root@xyvm2:/home/# dpkg -s libibverbs-dev
Package: libibverbs-dev
Status: install ok installed
Priority: optional
Section: libdevel
Installed-Size: 1702
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Architecture: amd64
Multi-Arch: same
# Source: rdma-core
Version: 28.0-1ubuntu1
# Depends: ibverbs-providers (= 28.0-1ubuntu1), libibverbs1 (= 28.0-1ubuntu1), libnl-3-dev, libnl-route-3-dev
Description: Development files for the libibverbs library
 libibverbs is a library that allows userspace processes to use RDMA
 "verbs" as described in the InfiniBand Architecture Specification and
 the RDMA Protocol Verbs Specification.  iWARP ethernet NICs support
 RDMA over hardware-offloaded TCP/IP, while InfiniBand is a
 high-throughput, low-latency networking technology.  InfiniBand host
 channel adapters (HCAs) and iWARP NICs commonly support direct
 hardware access from userspace (kernel bypass), and libibverbs
 supports this when available.
 .
 This package is needed to compile programs against libibverbs1.
 It contains the header files and static libraries (optionally)
 needed for compiling.
Homepage: https://github.com/linux-rdma/rdma-core
Original-Maintainer: Benjamin Drung <benjamin.drung@cloud.ionos.com>

```

```bash
root@xyvm2:/home/# dpkg -s librdmacm1
Package: librdmacm1
Status: install ok installed
Priority: optional
Section: libs
Installed-Size: 204
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Architecture: amd64
Multi-Arch: same
Source: rdma-core
Version: 28.0-1ubuntu1
# Depends: libc6 (>= 2.17), libibverbs1 (>= 20), libnl-3-200 (>= 3.2.7)
Description: Library for managing RDMA connections
 librdmacm is a library that allows applications to set up reliable
 connected and unreliable datagram transfers when using RDMA adapters.
 It provides a transport-neutral interface in the sense that the same
 code can be used for both InfiniBand and iWARP adapters.  The
 interface is based on sockets, but adapted for queue pair (QP) based
 semantics: communication must use a specific RDMA device, and data
 transfers are message-based.
 .
 librdmacm only provides communication management (connection setup
 and tear-down) and works in conjunction with the verbs interface
 provided by libibverbs, which provides the interface used to actually
 transfer data.
 .
 # This package contains the shared library. #dz 没有包含头文件
Homepage: https://github.com/linux-rdma/rdma-core
Original-Maintainer: Benjamin Drung <benjamin.drung@cloud.ionos.com>

```



### 问题：（网络上的问题）

两个虚拟机用的都是Host-Only 连的是vboxnet0 ip为192.168.1.3 混杂为全部允许，两台虚拟机任何一台ping vboxnet0的包可以抓到，但是两台虚拟机之间ping 就抓不到包

别人的解答：

应该抓不到，因为wireshark是基于底层驱动的，而两个虚拟机的包可能都没有经过底层驱动。

这个问题和我一样，我可以抓到vm->host之间的包，抓不到vm->vm之间的包



#### rdma-core python安装

```bash
# 全部
apt-get install build-essential cmake gcc libudev-dev libnl-3-dev libnl-route-3-dev ninja-build pkg-config valgrind python3-dev cython3 python3-docutils pandoc

# 分开
apt-get install build-essential cmake gcc 
apt install libudev-dev libnl-3-dev libnl-route-3-dev ninja-build valgrind 
apt install python3-dev cython3 python3-docutils pandoc
apt install pkg-config
```

上面的其他的都能安装，但是pkg-config一致失败，后面把这个单独安装就可以了

#### [pkg-config原理及用法](https://www.cnblogs.com/sddai/p/10266624.html)



```
export PYTHONPATH=$PYTHONPATH:"/home/rdmacore/rdma-core-master/build/python"
```





#### 执行测试框架

```
./run_tests.py --dev rocep0s8 -v test_qp
```



```
# server端 192.168.56.102
./rdma_test.py -d rocep0s8 -o write -n 5

# client端 192.168.56.101
./rdma_test.py -d rocep0s8 -o write -n 5 192.168.56.102
```





#### ping-sr-0 test

编译：直接make编译然后链接还是失败了，因为client.o这些目标文件默认在链接库的后面，手动放到前面就OK了。

```bash
# 链接client
gcc client.o utilities.o process_options.o c_d_control_struct.o c_d_id.o bind_client.o c_d_queue_pair.o c_d_connect.o c_d_buffers.o send_recv.o process_cm_events.o process_completions.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/  -lrdmacm -libverbs     -o client

# 链接server
gcc server.o utilities.o process_options.o c_d_control_struct.o c_d_id.o bind_listener.o c_d_connect.o c_d_queue_pair.o c_d_buffers.o send_recv.o agent.o process_cm_events.o process_completions.o  -lrt  -L/home/rdmacore/rdma-core-master/build/lib/  -lrdmacm -libverbs    -o server
```

102上面放server，101上面放client

```bash
# 将102机器上面编译成功的client程序放到101机器上
scp ./client root@192.168.56.102:/root

# server
./server -a 127.0.0.1
./server -a 127.0.0.1 -p 12001 -c 2000 -t -x
./server -a 127.0.0.1 -t -x

# 在101上面运行client
./client -a 192.168.56.102
./client -a 192.168.56.102 -p 12001 -c 2000 -t -x
./client -a 192.168.56.102 -t -x


```



server结果：

```
root@xyvm2:/home/training/OFA_Training_suite-of-examples/ping-sr-0# ./server -a 127.0.0.1 -p 12001 -c 2000 -t -x
server: option server_name "127.0.0.1"
server: option server_port 12001
server: option count 2000
server: option data_size 8
server: our_create_control_struct created our_control 0x563fbbd80880
server: rdma_create_id created cm_id 0x563fbbd8a5b0
server: rdma_create_id returned cm_id->channel 0x563fbbd80d90
server: rdma_create_id assigned fd 3
server: rdma_bind_addr ok
server: rdma_bind_addr returned cm_id -> channel 0x563fbbd80d90
server: rdma_listen ok
server: rdma_listen returned cm_id -> channel 0x563fbbd80d90
server: listener awaiting next cm event 4 (RDMA_CM_EVENT_CONNECT_REQUEST) our_control 0x563fbbd80880
server: listener got cm event 4 (RDMA_CM_EVENT_CONNECT_REQUEST) cm_id 0x563fbbd8a970 our_control 0x563fbbd80880 status 0
server: our_create_control_struct created our_control 0x563fbbd8b080
server: our_migrate_id migrated cm_id 0x563fbbd8a970
server: ibv_alloc_pd allocated protection domain 0x563fbbd814f0
server: our_create_cq cm_id->verbs->num_comp_vectors 2
server: ibv_create_cq created completion queue 0x563fbbd8b2e0
server: ibv_create_cq returned cqe 63
server: ibv_create_cq returned completion queue channel (nil)
server: rdma_create_qp created queue pair 0x563fbbd8b390
server: rdma_create_qp max_send_wr 16
server: rdma_create_qp max_recv_wr 16
server: rdma_create_qp max_send_sge 4
server: rdma_create_qp max_recv_sge 4
server: rdma_create_qp max_inline_data 0
server: our_setup_agent_buffers n_user_data_bufs 1
server: rdma_accept accepted cm_id 0x563fbbd8a970
server: Agent accepted our_control 0x563fbbd8b080
server: 2000 recv completions, 2000 send completions
server: 9076.354558 Messages per second
server: 0.580887 Megabits per second
server: 220.353 microseconds per round-trip
server: 0.440705569 seconds elapsed time
server: 0.329633000 seconds user time (74.7966%)
server: 0.110508000 seconds kernel time (25.0752%)
server: rdma_disconnect disconnected cm_id 0x563fbbd8a970
server: rdma_destroy_qp destroyed queue pair 0x563fbbd8b390
server: ibv_destroy_cq destroyed completion queue 0x563fbbd8b2e0
server: ibv_dealloc_pd deallocated protection domain 0x563fbbd814f0
server: rdma_destroy_id destroyed cm_id 0x563fbbd8a970
server: our_destroy_control_struct destroyed our_control 0x563fbbd8b080
server: rdma_destroy_id destroyed cm_id 0x563fbbd8a5b0
server: our_destroy_control_struct destroyed our_control 0x563fbbd80880
```



client结果：

```
root@xyvm1:~# ./client -a 192.168.56.102 -p 12001 -c 2000 -t -x
client: option server_name "192.168.56.102"
client: option server_port 12001
client: option count 2000
client: option data_size 8
client: our_create_control_struct created our_control 0x5555892c4880
client: rdma_create_id created cm_id 0x5555892ce5b0
client: rdma_create_id returned cm_id->channel 0x5555892c4d90
client: rdma_create_id assigned fd 3
client: ibv_alloc_pd allocated protection domain 0x5555892c54f0
client: our_create_cq cm_id->verbs->num_comp_vectors 2
client: ibv_create_cq created completion queue 0x5555892ceea0
client: ibv_create_cq returned cqe 63
client: ibv_create_cq returned completion queue channel (nil)
client: rdma_create_qp created queue pair 0x5555892cef50
client: rdma_create_qp max_send_wr 16
client: rdma_create_qp max_recv_wr 16
client: rdma_create_qp max_send_sge 4
client: rdma_create_qp max_recv_sge 4
client: rdma_create_qp max_inline_data 0
client: our_setup_client_buffers n_user_data_bufs 2
client: rdma_connect connected cm_id 0x5555892ce5b0
client: Client connected our_control 0x5555892c4880
client: 2000 recv completions, 2000 send completions
client: 9073.556862 Messages per second
client: 0.580708 Megabits per second
client: 220.421 microseconds per round-trip
client: 0.440841454 seconds elapsed time
client: 0.210760000 seconds user time (47.8086%)
client: 0.229234000 seconds kernel time (51.9992%)
client: rdma_disconnect disconnected cm_id 0x5555892ce5b0
client: rdma_destroy_qp destroyed queue pair 0x5555892cef50
client: ibv_destroy_cq destroyed completion queue 0x5555892ceea0
client: ibv_dealloc_pd deallocated protection domain 0x5555892c54f0
client: rdma_destroy_id destroyed cm_id 0x5555892ce5b0
client: our_destroy_control_struct destroyed our_control 0x5555892c4880
```





2022年3月25日11:28:13

usr/lib/x86_64_linux_gun 下面有

libibverbs.so.1.8.28.0

librdmacm.so.1.2.28.0

没有libmx5.so



rdma-core-master 目录下面build/lib 里面：

libibverbs.so.1.14.39.0

librdmacm.so.1.3.39.0

libmx5.so.1.22.39.0



自己编译rdmacore-master里面example的 rc-pingpong.c

```bash
# 链接的时候失败了,报了很多 undefined reference
gcc -I /home/rdmacore/rdma-core-master/build/include rc_pingpong.c -o rc

# 先编译, ok了
gcc -c -I /home/rdmacore/rdma-core-master/build/include rc_pingpong.c -o rc.o
gcc -c -I /home/rdmacore/rdma-core-master/build/include pingpong.c -o pingpong.o

# 再手动链接
ld rc.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -libverbs -o ibv_rc_pingpong

# 上面提示失败，发现少了pthread库，添加上，后面又提示少了libc库，添加上，后面又发现 rc.o中.text段有几个函数没有找到，发现还引用了pingpong.c中函数，就又编译了pingpong.o然后加上，最终链接代码如下：
root@xyvm2:/home/rdmacore/rdma-core-master/libibverbs/examples#  ld rc.o pingpong.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -libverbs -L/lib/x86_64-linux-gnu/ -lpthread -lc -o ibv_rc_pingpong

# 但是这个还是提示了一个warning: cannot find entry symbol _start; defaulting to 00XXXX, 但是是个warning，我就没管了，生成的程序 ibv_rc_pingpong 可以运行
# 注意，上面这个错误不能不管，不管的话，我运行 ibv_rc_pingpong, 其实根本没运行到这个文件夹里面的，而是运行的/usr/bin里面的那个ibv_rc_pingpong, 要使用 ./ibv_rc_pingpong才行！！！
# 另外，./ibv_rc_pingpong会提示 No such file or directory, 这是因为之前链接之后，没有_start入口，程序没法运行！！！ 要自己使用 gcc 来链接，不要自己使用 ld 链接！！！用gcc还可以少几个手动指定的库
gcc rc.o pingpong.o -lrt -L/home/rdmacore/rdma-core-master/build/lib/ -lrdmacm -libverbs -o ibv_rc_pp
```



### RDMA

##### ibv_rc_pingpong

直接执行程序是 /usr/bin目录下的ibv_rc_pingpong

```bash
# 101上面启动服务端
ibv_rc_pingpong
# 102上面启动客户端
ibv_rc_pingpong 192.168.56.101
```

运行这个程序，client提示:

> client read/write: Protocol not supported
> Couldn't read/write remote address

server提示：

> Failed to modify QP to RTR
> Couldn't connect to remote QP

这个是在调用ibv_modify_qp 后报的错，我记得错误号是 22，perror翻译这个错误号，就是上面的 ’Protocol not supported‘

百度，发现这程序有些bug：https://docs.oracle.com/cd/E56344_01/html/E54075/ibv-rc-pingpong-1.html

> The  network  synchronization  between  client  and   server instances is weak, and does not prevent incompatible options from being used on the two instances.  The method  used  for retrieving  work  completions  is  not strictly correct, and race conditions may cause failures on some systems.



后面看了知乎的文章：https://zhuanlan.zhihu.com/p/476407641

发现添加了一个 -g 指定 gid的参数，就可以成功了！！！

```bash
# 101上面启动服务端
ibv_rc_pingpong -g 1
# 102上面启动客户端
ibv_rc_pingpong -g 1 192.168.56.101
```

**ibv_devinfo最下面port1有两个gid, 试了下用gid 0不行，只能用gid 1才可以**



##### ibv_ud_pingpong

```bash
# serve
ibv_ud_pingpong
# client
ibv_ud_pingpong 192.168.56.101
```

server提示：

> Failed to create AH
> Couldn't connect to remote QP



##### rdma_client/rdma_server

```bash
# server
rdma_server -p 1993
# client
rdma_client -s 192.168.56.101 -p 1993
```

这个程序好像成功了，没有报错

> rdma_server: start
> rdma_server: end 0
