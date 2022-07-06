遇到的问题：

1. 已经使用了 mpi， 我自己用的openmpi，这个会怎么样，我看dense_liner_al 等用了这个mpi
2. mpi好像要重新开启线程 TODO
3. 双边demo逻辑





boost asio

acceptor有两种接收方式 

同步接收 acceptor.accept

异步接收 acceptor.async_accept





TODO 

客户端client 连接到 协调节点，也是一个engine

代码提示 client使用baseconn连接， engine使用 base的子类 conn连接。

client连接到哪里呢？连接到engine的 ccm, 而 engine之间用conn连接，连接到 networkmanager

_acceptor.async_accept 这个只有 ClientCommManager和 NetworkManager两个文件里面有

客户端的通信是由这个 ccm管理的：ClientCommManager ：A tool that listens for incoming client messages and processes them to execute queries on SciDB.





**baseconntion中的同步代码，先手动替换，然后在尝试动态整合进去。**



connection 中 异步连接使用的是 _socket.async_connect，相当于是客户端去连接。

上面的_acceptor.async_accept相当于就是服务端了

先将 client和 master之间的同步连接替换掉，然后替换instance之间的异步建立的连接。

TODO 多线程问题





scidb ccm启动流程：

entry.cpp中 runScidb() -> 启动ccmservice -> 启动ccmservice:impl , 实现里面会启动一个ccm service job，这个job会启动一个 包含线程数量为 1的线程池，然后往job里面放入一个 job，线程池会执行push_back进去的job的run方法，这个job的run方法里面， 会设置一些属性，然后初始化一个 ClientCommManager，这个manager里面就会调用io_service的run方法，一直接socket请求。



