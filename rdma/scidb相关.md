```bash
# 查看用户
cat /etc/passwd
# 当前登录用户
root@xy-virtual-machine:~# who
xy       :0           2022-06-22 10:02 (:0)
root     pts/0        2022-06-22 10:58 (192.168.80.1)
root     pts/1        2022-06-22 14:39 (192.168.80.1)

# 查看用户组
cat /etc/group
# 清晰显示用户，不会显示 nologin, halt, shutdown等用户
cat /etc/passwd|grep -v nologin|grep -v halt|grep -v shutdown|awk -F":" '{ print $1"|"$3"|"$4 }'|more

# 判断是否存在用户
id -u postgres
103
id -u scidb
id: scidb: no such user
```





#### PG常用命令

教程：https://www.postgresqltutorial.com/

```bash
# 连接数据库, 默认的用户postgres和数据库postgres
psql -U user -d dbname

# 或者, 先切换到postgres用户
su username
#或者 sudo -i -u postgres
psql

# 退出psql
\q
```



数据库相关

```bash
# 查看所有数据库
select * from pg_database;
# 列举数据库，相当于mysql的show databases
\l

# 切换数据库，相当于mysql的use dbname
\connect DBNAME
# 或者
\c DBNAME
------------------------------------------------------------
postgres=# \c scidb;
You are now connected to database "scidb" as user "postgres".
scidb=#

# 创建数据库： 
create database dbname; 
# 删除数据库： 
drop database dbname;  
```

表相关

```bash
# 列举表，相当于mysql的show tables
\dt

# 查看表结构，相当于desc tblname,show columns from tbname
\d tblname

# 查看索引 
\di
```



其他

```bash
# 显示 PostgreSQL 的使用和发行条款
\copyright
# 显示或设定用户端字元编码
\encoding [字元编码名称]
# SQL 命令语法上的说明，用 * 显示全部命令，比如 \h select就可以查看select的语法说明
\h [名称] 
# 提示用户设定内部变数
\prompt [文本] 名称
# 改密码                  
\password [USERNAME]
```

备份

```bash
# 可以使用pg_dump和pg_dumpall来完成，比如备份sales数据库： 
pg_dump drupal>/opt/Postgresql/backup/1.bak  TODO，好像不太行
```

pg_dump说明：http://postgres.cn/docs/9.6/app-pgdump.html



#### 修改pgsql配置文件：

```bash
# 配置文件位置，这个文件配置客户端校验 PostgreSQL Client Authentication Configuration File
/etc/postgresql/9.3/main/pg_hba.conf

# 在运行的系统上修改这个文件之后，需要操作才能生效
pg_ctl reload

# 总的配置文件
/etc/postgresql/9.3/main/postgresql.conf
# 最后添加
listen_addresses='*'
# 重启pgsql
sudo /etc/init.d/postgresql restart
```





### SciDB表

```bash
-----------------------------------------------------
scidb=# \dt
                  List of relations
 Schema |            Name            | Type  | Owner
--------+----------------------------+-------+-------
 public | array                      | table | scidb
 public | array_attribute            | table | scidb
 public | array_dimension            | table | scidb
 public | array_distribution         | table | scidb
 public | array_residency            | table | scidb
 public | array_version              | table | scidb
 public | array_version_lock         | table | scidb
 public | cluster                    | table | scidb
 public | instance                   | table | scidb
 public | latest_array_version       | table | scidb
 public | libraries                  | table | scidb
 public | membership                 | table | scidb
 public | namespace_members          | table | scidb
 public | namespaces                 | table | scidb
 public | role_members               | table | scidb
 public | role_namespace_permissions | table | scidb
 public | roles                      | table | scidb
 public | users                      | table | scidb
(18 rows)
-----------------------------------------------------

-----------------------------------------------------
scidb=# \d instance
                   Table "public.instance"
       Column       |            Type             | Modifiers
--------------------+-----------------------------+-----------
 instance_id        | bigint                      | not null
 membership_id      | bigint                      | default 0
 host               | character varying           |
 port               | integer                     |
 online_since       | timestamp without time zone |
 base_path          | character varying           |
 server_id          | integer                     |
 server_instance_id | integer                     |
Indexes:
    "instance_pkey" PRIMARY KEY, btree (instance_id)
    "instance_host_port_key" UNIQUE CONSTRAINT, btree (host, port)
    "instance_host_server_id_server_instance_id_key" UNIQUE CONSTRAINT, btree (host, server_id, server_instance_id)
    "instance_server_id_server_instance_id_key" UNIQUE CONSTRAINT, btree (server_id, server_instance_id)
Check constraints:
    "instance_base_path_non_unique" CHECK (check_base_path(base_path))
    "instance_server_id_host_unique" CHECK (check_server_id_host(server_id, host))
Referenced by:
    TABLE "array_residency" CONSTRAINT "array_residency_instance_id_fkey" FOREIGN KEY (instance_id) REFERENCES instance(instance_id)
    TABLE "array_version_lock" CONSTRAINT "array_version_lock_coordinator_id_fkey" FOREIGN KEY (coordinator_id) REFERENCES instance(instance_id)
    TABLE "array_version_lock" CONSTRAINT "array_version_lock_instance_id_fkey" FOREIGN KEY (instance_id) REFERENCES instance(instance_id)
-----------------------------------------------------
```



**users表目前只有一个用户 scidbadmin**

```bash
scidb=# select * from users;
 id |    name    | method |                                         password                                         | salt
----+------------+--------+------------------------------------------------------------------------------------------+------
  1 | scidbadmin | raw    | eUCUk3B57IVO9ZfJB6CIEHl/0lxrWg/7PV8KytUNY6kPLhTX2db48GHGHoizKyH+uGkCfNTYZrJgKzjWOhjuvg== |
(1 row)
```

**roles表也只有一个角色acidbadmin**

```bash
scidb=# select * from roles;
 id |    name
----+------------
  1 | scidbadmin
(1 row)
```

**角色-用户 关系**

```bash
scidb=# select * from role_members;
 role_id | user_id
---------+---------
       1 |       1
(1 row)
```



docker启动scidb时的提示：

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

有一个集群：

```bash
scidb=# select * from cluster;
             cluster_uuid             | metadata_version
--------------------------------------+------------------
 f9d016e8-520c-11ea-b480-0242ac110003 |                5
```

有两个实例：

```bash
scidb=# select * from instance;
 instance_id | membership_id |   host    | port |       online_since        |        base_path         | server_id | server_instance_id
-------------+---------------+-----------+------+---------------------------+--------------------------+-----------+--------------------
           0 |             0 | 127.0.0.1 | 1239 | 2020-02-18 05:10:27.32034 | /opt/scidb/19.3/DB-scidb |         0 |                  0
           1 |             0 | 127.0.0.1 | 1240 | 2020-02-18 05:10:27.67094 | /opt/scidb/19.3/DB-scidb |         0 |                  1
(2 rows)

```



#### 配置文件

```bash
# 位置
/opt/scidb/19.11/etc/config.ini # ini文件分号是注释，注意要另起一行才行，不支持行间注释
/opt/scidb/19.3/etc/config.ini 
#---------------------------------------
[scidb]
# base path下面的0代表server0，server0下面的0,1代表instance0和1
base-path=/opt/scidb/19.3/DB-scidb
base-port=1239
db_user=scidb
install_root=/opt/scidb/19.3
# 19.3里面默认的日志级别是warn，改成debug试试
logconf=/opt/scidb/19.3/share/scidb/log4cxx.properties
# 里面放了很多其他的so文件
pluginsdir= /opt/scidb/19.3/lib/scidb/plugins
# server-id=IP, number of worker instances
server-0=127.0.0.1,1
execution-threads=4
result-prefetch-threads=3
result-prefetch-queue-size=1
operator-threads=1
sg-send-queue-size=4
sg-receive-queue-size=4
#---------------------------------------
```



scidb的docker容器中，root用户目录下面，有一个 .pgpass文件，内容是

> 127.0.0.1:5432:scidb:scidb:Y2FhZTBiNjU5NDk5NzFkNzg5ZjNhZDNh



启动scidb

```bash
/opt/scidb/19.11/bin/scidbctl.py start <CLUSTER_NAME>
/opt/scidb/19.3/bin/scidbctl.py start
--------------------------------------------------------------------------
[scidbctl] Starting SciDB cluster scidb ... # 注意，这里启动的就是集群scidb,也就是config.ini最上面的[scidb]
[scidbctl-0-0-scidb] Starting s0-i0 on server 127.0.0.1
[scidbctl-0-1-scidb] Starting s0-i1 on server 127.0.0.1
[scidbctl] Started 2 instances, waiting up to 30 seconds for cluster sync
[scidbctl] Cluster is ready
--------------------------------------------------------------------------
```

确认scidb在运行：

```bash
iquery -aq "list('instances')"
--------------------------------------------------------------------------
root@9a5ae7fc5a66:~# iquery -aq "list('instances')"
{No} name,port,instance_id,online_since,instance_path
{0} '127.0.0.1',1239,0,'2020-02-18 05:10:27','/opt/scidb/19.3/DB-scidb/0/0'
{1} '127.0.0.1',1240,1,'2020-02-18 05:10:28','/opt/scidb/19.3/DB-scidb/0/1'
--------------------------------------------------------------------------
```

停止scidb

```bash
root@9a5ae7fc5a66:/opt/scidb/19.3/bin# ./scidbctl.py stop
[127.0.0.1] Stopping local scidb instances
[scidbctl] Waiting up to 10 seconds for cluster scidb to stop...
[scidbctl] Cluster scidb stopped
```

检查服务是否在各个服务器上面运行

```bash
scidbctl.py --config <config.ini> status <CLUSTER_NAME>
--------------------------------------------------------------------------
root@9a5ae7fc5a66:/opt/scidb/19.3/bin# scidbctl.py --config /opt/scidb/19.3/etc/config.ini status scidb
PID     PPID    CMD             SERVER
836     834     SciDB-0-0-scidb 127.0.0.1
835     832     SciDB-0-1-scidb 127.0.0.1
--------------------------------------------------------------------------
```

日志文件

```bash
# 目录
/opt/scidb/19.3/DB-scidb/0/0
```



```bash
docker inspec scidb_容器id
------------------------------------
env环境变量：
"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/scidb/19.3/bin",
"SCIDB_VER=19.3",
"SCIDB_NAME=scidb",
"SCIDB_INSTALL_PATH=/opt/scidb/19.3"

```





### SciDB-SQL语句

```bash
# 首先插入了一张表
iquery -aq "store(build(<num:double>[x=0:8,1,0, y=0:9,1,0], random()),dz_random_numbers)"
# 查询该表结构
SELECT * FROM show(dz_random_numbers); 
iquery -aq "SELECT * FROM show(dz_random_numbers)" # 报语法错误
--------------------------------------------------------------------
root@9a5ae7fc5a66:/opt/scidb/19.3/bin# iquery
AQL% SELECT * FROM show(dz_random_numbers);
{i} schema,distribution
{0} 'dz_random_numbers<num:double> [x=0:8:0:1; y=0:9:0:1]','hashed'
--------------------------------------------------------------------

# 在AQL中可以直接查询该表
AQL% select * from dz_random_numbers;
```





### SciDB代码流程

entry: runSciDB开始运行，第一个日志 221行：Start SciDB instance



日志中查找用这个 runSciDB: preloading injecterror

**NetworkManager::run() 之后**

Creating ClientCommManager on port 5239

Creating CcmConnection

Network manager is started on 127.0.0.1:1239 instance #s0-i0 (0)

之后会在消息类型上面搞一个订阅者 

Subscribe [scidb::InstanceLiveness] (id = 1) # 这段代码在 notification.h 207行



之后开始接收 startAccept(); 这里就会初始化 newConnection，去连接对端。 这个startAccept会调用一个异步接收的，会接收请求，请求在 handleAccept里面处理，处理之后，又会调用 startAccept继续接收请求，形成了一个不断接收请求的循环。



之后就是io_service的main loop

LOG4CXX_DEBUG(logger, "Start connection accepting and async message exchanging");

    // main loop
    _ioService.run();





readmessage调用流程：

networkmanager.startAccept() 里面会构造一个newConnection

```c++
// todo 不知道为什么这个connection只用了三个参数，connection需要4个参数的，还差一个对方的instance_id
std::shared_ptr<Connection> newConnection =
           std::make_shared<Connection>(*this, getNextConnGenId(), _selfInstanceID);
```



NetworkManager::handleAccept -> newConnection->start() 然后 startAccept -> startAccept里面 又会循环调用 _acceptor.async_accept（回调函数是handleAccept，形成一个循环了），所以突破点是上面的 conn->start()，这个函数里面会读取socket里面的数据，之后就是下面readmessage的流程了：

```c++
_networkManager.getIOService().post(std::bind(&Connection::readMessage,shared_from_this()));
```

readMessage -> asio::async_read -> 异步读到了数据 handleReadMessage，这个里面会读取header中的getRecordSize() 获取头结点数据大小 -> handleReadRecordPart 里面会看 是否binary_size>0, 大于0就读取binary_size大小的数据 -> 之后会调用NetworkManger:handleMessage -> 然后分发消息NetworkManager::dispatchMessage start -> 我这个消息是 有Conn但是没有session的，表示现在还在验证和登录，所以会 发一个job AuthMessageHandleJob-> 处理这个job,会调用他的dispatch 方法，根据消息类型 switch，现在消息类型是 mtAuthLogon -> AuthMessageHandleJob::handleAuthLogon()







connecttion中有session，session中可以知道是否来自于client



#### BaseConnection:

sendAndReadMessage: 是个**同步**的消息发送和接受的函数

receive函数是直接使用的 read函数读取的，没有用boost的一些函数，有点奇怪： 不对，下面read只有两个参数？TODO 这个应该也是boost的函数，因为里面的socket也是 boost::asio::ip::tcp::socket _socket;

```
// 函数原型   Ssize_t read(int fd,void *buf,size_t nbyte)
size_t readBytes = read(_socket, boost::asio::buffer(&resultDesc->_messageHeader,
                                                             sizeof(resultDesc->_messageHeader)));
```



#### Connection：

这个是用于NetworkManager之间一部消息的传递使用的，继承自 baseconnection.





### scidb源码安装之后运行

make install 之后，目前安装在 /usr/local/bin里面，和docker里面的/opt/scidb/19.3/bin里面的内容是一样的

直接运行，会没有配置文件

```bash
root@xy-virtual-machine:/usr/local/bin# scidbctl.py start
[scidbctl] [E] Missing config file /usr/local/etc/config.ini
```



```bash
mkdir /opt/scidb/19.11/etc
cd /opt/scidb/19.11/etc
vi config.ini
--------------------------------------------------------------------------
[scidb]
base-path=/opt/scidb/19.3/DB-scidb
base-port=1239
db_user=scidb
install_root=/opt/scidb/19.3
logconf=/opt/scidb/19.3/share/scidb/log4cxx.properties
pluginsdir= /opt/scidb/19.3/lib/scidb/plugins
server-0=127.0.0.1,1
execution-threads=4
result-prefetch-threads=3
result-prefetch-queue-size=1
operator-threads=1
sg-send-queue-size=4
sg-receive-queue-size=4
```

