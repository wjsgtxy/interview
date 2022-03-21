Mysql 学习
慢查询日志设置
show variables like 'slow_query%';
SHOW VARIABLES LIKE 'long_query_time';
set local long_query_time = 2;

还可以这么查询 select @@global.long_query_time;
local的当前session就可以直接查到，如果设置的是global，需要重新开一个session，才能查看到设置的新的值

如果查询时使用的是show variables的话, 会发现设置并没有生效, 除非重新登录再查看. 这是因为使用show variables的话就等同于使用show session variables, 查询的是会话变量, 只有使用show global variables查询的才是全局变量. 如果仅仅想修改会话变量的话, 可以使用类似set wait_timeout=10;或者set session wait_timeout=10;这样的语法. 

当前只修改了正在运行的MySQL实例参数, 但下次重启mysqld又会回到默认值, 所以别忘了修改参数文件:
# vi /etc/my.cnf

[mysqld]

wait_timeout=10

如果要修改会话变量值, 可以指定"SESSION"或者"@@session."或者"@@"或者"LOCAL"或者"@@local.", 或者什么都不使用. 



锁表与解锁表
   LOCK TABLE `table` [READ|WRITE]
解锁
    UNLOCK TABLES;
LOCK TABLES为当前线程锁定表。 UNLOCK TABLES释放被当前线程持有的任何锁。当线程发出另外一个LOCK TABLES时，或当服务器的连接被关闭时，当前线程锁定的所有表会自动被解锁。 

如果一个线程获得在一个表上的一个READ锁，该线程和所有其他线程只能从表中读。 如果一个线程获得一个表上的一个WRITE锁，那么只有持锁的线程READ或WRITE表，其他线程被阻止。	


在同一个连接当中，执行了一次lock table之后，如果再次执行lock table，那么之前被锁住的表就自动解锁了。
lock table city read;
select * from city;
show open tables;
lock table country read;
show open tables; 这个时候会发现 city表已经被解锁了
unlock tables;

SELECT * FROM table LIMIT [offset,] rows | rows OFFSET offset
LIMIT 子句可以被用于强制 SELECT 语句返回指定的记录数。LIMIT 接受一个或两个数字参数
mysql> SELECT * FROM table LIMIT 5,10; // 检索记录行 6-15

//为了检索从某一个偏移量到记录集的结束所有的记录行，可以指定第二个参数为 -1： 
mysql> SELECT * FROM table LIMIT 95,-1; // 检索记录行 96-last.

//如果只给定一个参数，它表示返回最大的记录行数目： 
mysql> SELECT * FROM table LIMIT 5; //检索前 5 个记录行

//换句话说，LIMIT n 等价于 LIMIT 0,n。

注意：要引用外部查询的表，多数情况下是在WHERE里面进行的相关子查询，或者SELECT子句里(可见23题方法2)
SQL相关子查询与非相关子查询
https://blog.csdn.net/shiyong1949/article/details/80923083