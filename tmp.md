TODO 

1. 很多conn会close，看看什么情况
2. 心跳消息，5秒一连接好像



是否需要绑定到conn，还是instance 之间维护 rdma conn



net库变成动态库





Sanitizer 消毒剂的意思





scidb conn建立链接过程：

connectAsync->回调connectAsyncInternal -> async_resolve callback:onResolve -> _socket.async_connect  -> onConnect 中间会调用 configConnectedSocket();  setRemoteIpAndPort();   -》向peer 发送 sendMessage(logonMsg) ,同时authDoneInternal()
