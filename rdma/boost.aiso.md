boost.asio 中 io_service.run也会创建几个线程，但是这些线程不会访问用户代码，所以你的异步操作，任然是只会在调用io_service.run的线程中被调用。

三种模型：

1. 一个io_service，单线程，这样所有的异步操作，都在一个线程中执行，但是第二个异步操作需要等待第一个操作完成之后才能执行。
2. 一个io_service，但是多个线程中都调用了这个io_service的run方法，那么这些线程都可以处理绑定到这个ioservice上面的异步操作，看谁空闲谁就处理新来的异步操作。
3. 多个ioservice，多个线程，socket绑定到不同的service上面，同时这些service run在不同的线程中，那么socket对应的异步操作，也只会在对应绑定的线程中执行。



我看scidb中 client和engine之间的ccm通信，用的就是第一种方式。