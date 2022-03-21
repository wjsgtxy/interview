RoCE配置

配置RXE网卡

```bash
sudo rdma link add rxe_0 type rxe netdev enp0s8
```

实际上面生成的网卡名称不叫rxe_0，而是叫rocep0s8，不知道为什么

```bash
# 查看设备
ibv_devices
# 查看整个虚拟RDMA设备的信息：
ibv_devinfo -d rocep0s8
```



perftest测试：

```bash
# 101机器上
ib_send_bw -d rocep0s8
# 102机器上
ib_send_bw -d rocep0s8 192.168.56.101
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