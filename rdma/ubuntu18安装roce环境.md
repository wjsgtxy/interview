# Ubuntu18.04安装SoftRoce环境

第一步：安装rdma相关组件

```bash
sudo apt-get install libibverbs1 ibverbs-utils librdmacm1 libibumad3 ibverbs-providers rdma-core
```

第二步：安装perftest及iproute2

```bash
sudo apt-get install iproute2
sudo apt-get install perftest
```

第三步：rdma-rxe相关配置

```bash
modprobe rdma_rxe
```

我这一步提示 :

> modprobe: ERROR: could not insert 'rdma_rxe': Operation not permitted

后面发现是因为我之前禁用了ipv6，所以这个加入不了，把我之前添加的禁用ipv6的内核参数去掉，然后重启才行。

```bash
vi /etc/default/grub
sudo update-grub
reboot
```



检查rxe状态 

```bash
rxe_cfg status
----------------
    Name     Link  Driver  Speed  NMTU  IPv4_addr  RDEV  RMTU
    docker0  no    bridge
    ens32    yes   e1000
```

启动rxe_cfg 

```bash
rxe_cfg start 
```

把虚拟网卡加入，ifconfig查看网卡名，我这里是ens33

```bash
rxe_cfg add ens32
-------------------------------
    Name     Link  Driver  Speed  NMTU  IPv4_addr  RDEV  RMTU
    docker0  no    bridge
    ens32    yes   e1000                           rxe0  1024  (3)
```



将上面的命令添加到文件，要不已启动就失效了

```bash
modprobe rdma_rxe
rxe_cfg add ens32
```



###### perftest测试

```bash
# 192.168.40.129 
ib_send_bw -d rxe0
ib_send_bw -d rxe0 192.168.40.129 # 同一台机器可以吗？这里也是可以的，而且速度更快，有700MB/s, 两台虚拟机之间才50MB/s
# 192.168.40.130
ib_send_bw -d rxe0 192.168.40.129
```



###### rdma_client 和 rdma_server测试

```bash
# 先安装才有rdma_server命令
apt install rdmacm-utils
# 之前网卡模式选了 VMNET1 所以连接不了外网，重新添加了一张nat网卡就ok了
```

```bash
# server 129机器
rdma_server -p 1993
rdma_client -s 192.168.40.129 -p 1993 # 同一台机器可以吗？是可以的
# client 130机器
rdma_client -s 192.168.40.129 -p 1993
```





