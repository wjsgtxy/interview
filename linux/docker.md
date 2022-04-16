##### docker相关：

```
docker run -d --name fwd_sig registry/fwd_sig:v1 /c-init-sig 
```

之前已经启动一次了，重新启动的时候，会报错：Error response from daemon: Conflict. The container name "/fwd_sig" is already in use by container e3274a72e8d62a0f3022d3201405ce586147b3031c1232452d001ee41fb9c938. You have to remove (or rename) that container to be able to reuse that name..（容器名被占用，须移除或重命名后才能使用这个容器名。）

解决办法：

```
# 先查看所有的容器
docker ps -a
# 删除之前的容器就OK了
docker rm e327

# 或者直接运行之前的容器
docker start e327

# 如果要停止某个容器的运行
docker stop e327
```



```
# 追踪某个线程
strace -p 15909
```

