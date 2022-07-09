## Postgressql相关





```bash
# 查看状态
systemctl status postgresql
-----------------------------------------------
● postgresql.service - PostgreSQL RDBMS
   Loaded: loaded (/lib/systemd/system/postgresql.service; enabled; vendor preset: enabled)
   Active: active (exited) since Fri 2022-07-08 09:14:54 CST; 10min ago
  Process: 1737 ExecStart=/bin/true (code=exited, status=0/SUCCESS)
 Main PID: 1737 (code=exited, status=0/SUCCESS)

7月 08 09:14:54 xy-virtual-machine2 systemd[1]: Starting PostgreSQL RDBMS...
7月 08 09:14:54 xy-virtual-machine2 systemd[1]: Started PostgreSQL RDBMS.

# 启动服务
# systemctl start postgresql-10 没作用，说找不到postgressql-10.service
systemctl start postgresql

# 停止
systemctl stop postgresql

# 禁用自启动
systemctl disable postgresql

# 启用自启动
systemctl enable postgresql
```

