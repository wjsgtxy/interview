##### docker相关：

```
docker run -d --name fwd_sig registry/fwd_sig:v1 /c-init-sig 
```

之前已经启动一次了，重新启动的时候，会报错：Error response from daemon: Conflict. The container name "/fwd_sig" is already in use by container e3274a72e8d62a0f3022d3201405ce586147b3031c1232452d001ee41fb9c938. You have to remove (or rename) that container to be able to reuse that name..（容器名被占用，须移除或重命名后才能使用这个容器名。）

解决办法：

```bash
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



```bash
# 下载镜像, 镜像名称：tag, tag默认为latest 镜像会分层下载： docker image 的核心 联合文件系统
docker pull tomcat:8 
```



容器命令：

```bash
# 新建容器并启动
docker run 镜像id 
# 启动容器
docker start 容器id \ docker start 容器名称
# 重启容器
docker restart 容器id 
# 停止当前正在运行的容器 
docker stop 容器id
# 强制停止当前容器
docker kill 容器id 

# 列出所有正在运行的容器
docker ps \ docker container list
# 列出所有容器，不管是否运行
docker ps -a
# 退出容器，会直接停止容器
exit
# 删除指定容器
docker rm 容器id 
# 容器退出但不停止
ctrl+p+q
```



#### docker image 导入导出

```bash
# 查看要导出的image
docker images

# 导出
docker save image_id > file_name.tar

# 导入
docker load < file_name.tar
-----------------------------------------------------------------------------------
REPOSITORY                            TAG       IMAGE ID       CREATED       SIZE
<none>                                <none>    fb5ba79cfbd2   2 years ago   3.92GB
devcafe/cmake-cookbook_ubuntu-18.04   latest    47e992d8adf9   3 years ago   2.37GB

# 导入的image是没有REPOSITORY和TAG的，可以自己手动添加
docker tag fb5ba79cfbd2 rvernica/scidb:19.3-ext
-----------------------------------------------------------------------------------
REPOSITORY                            TAG        IMAGE ID       CREATED       SIZE
rvernica/scidb                        19.3-ext   fb5ba79cfbd2   2 years ago   3.92GB
devcafe/cmake-cookbook_ubuntu-18.04   latest     47e992d8adf9   3 years ago   2.37GB
```



#### docker scidb相关

```bash
docker run -it --name="scidb" fb5b /bin/bash
-----------------------------------------------------------------------
 * Starting OpenBSD Secure Shell server sshd                    [ OK ]
 * Starting PostgreSQL 9.3 database server                      [ OK ]
shim: SciDB HTTP service started on port(s) 8080,8083s with web root [/var/lib/shim/wwwroot], talking to SciDB on port 1239
[scidbctl] Starting SciDB cluster scidb ...
[scidbctl-0-0-scidb] Starting s0-i0 on server 127.0.0.1
[scidbctl-0-1-scidb] Starting s0-i1 on server 127.0.0.1
[scidbctl] Started 2 instances, waiting up to 30 seconds for cluster sync
[scidbctl] Cluster is ready

```



#### 容器拷贝

```bash
docker cp 容器id:容器内路径 主机目的路径
----------------------------------------
docker cp 9a5a:/opt/scidb/19.3/DB-scidb/0/0/*.log /home/log # 这种不行，只能指定具体的文件名称
docker cp 9a5a:/opt/scidb/19.3/DB-scidb/0/0/scidb.log /home/log
```



##### 进入容器

```bash
# docker exec -it 容器id /bin/bash
docker start 9a5a
# 进入scidb容器
docker exec -it 9a5a /bin/bash 
```

