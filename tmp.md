0420
rdma_getaddrinfo

https://linux.die.net/man/3/rdma_getaddrinfo

```
# 获取ip:port对应的地址结构，返回res
#include <rdma/rdma_cma.h>
int rdma_getaddrinfo (char *node, char *service, struct rdma_addrinfo *hints, struct rdma_addrinfo **res);
```



###### rdma_create_ep

https://linux.die.net/man/3/rdma_create_ep

```
# 创建一个rdma_cm_id
#include <rdma/rdma_cma.h>
int rdma_create_ep (struct rdma_cm_id **id, struct rdma_addrinfo *res, struct ibv_pd *pd, struct ibv_qp_init_attr *qp_init_attr);
```



```
rdma_reg_msgs - register data buffer(s) for sending or receiving messages.

#include <rdma/rdma_verbs.h>
struct ibv_mr * rdma_reg_msgs (struct rdma_cm_id *id, void *addr, size_t length);

Arguments
id
A reference to a communication identifier where the message buffer(s) will be used.
addr
The address of the memory buffer(s) to register.
length
The total length of the memory to register.
```



```bash
rdma_post_recv(3) - Linux man page
Name
rdma_post_recv - post a work request to receive an incoming message.
Synopsis
#include <rdma/rdma_verbs.h>
int rdma_post_recv (struct rdma_cm_id *id, void *context, void *addr, size_t length, struct ibv_mr *mr);

Arguments
id
A reference to a communication identifier where the message buffer will be posted.
context # 用户定义的context
User-defined context associated with the request.
addr
The address of the memory buffer to post.
length
The length of the memory buffer.
mr
A registered memory region associated with the posted buffer.

Description
Posts a work request to the receive queue of the queue pair associated with the rdma_cm_id. The posted buffer will be queued to receive an incoming message sent by the remote peer.
Return Value
Returns 0 on success, or -1 on error. If an error occurs, errno will be set to indicate the failure reason.
Notes
The user is responsible for ensuring that a receive buffer is posted and large enough to contain all sent data before the peer posts the corresponding send message. The message buffer must have been registered before being posted, with the mr parameter referencing the registration. The buffer must remain registered until the receive completes.
# 要先有qp才能post msg， qp可通过rdma_create_ep或者rdma_create_qp绑定
Messages may be posted to an rdma_cm_id only after a queue pair has been associated with it. A queue pair is bound to an rdma_cm_id after calling rdma_create_ep or rdma_create_qp, if the rdma_cm_id is allocated using rdma_create_id.

# 注意，用户定义的context会在后面返回，通过wr_id
The user-defined context associated with the receive request will be returned to the user through the work completion wr_id, work request identifier, field.
```



```
rdma_get_request(3) - Linux man page
Name
rdma_get_request - Retrieves the next pending connection request event.
Synopsis
#include <rdma/rdma_cma.h>
int rdma_get_request (struct rdma_cm_id *listen, struct rdma_cm_id **id);

Arguments
listen
Listening rdma_cm_id.
id
rdma_cm_id associated with the new connection.
Description
Retrieves a connection request event. If no requests are pending, the call will block until an event is received.
Return Value
Returns 0 on success, or -1 on error. If an error occurs, errno will be set to indicate the failure reason.
Notes
This call may only be used on listening rdma_cm_id's operating synchronously. On success, a new rdma_cm_id representing the connection request will be returned to the user. The new rdma_cm_id will reference event information associated with the request until the user calls rdma_reject, rdma_accept, or rdma_destroy_id on the newly created identifier. For a description of the event data, see rdma_get_cm_event.
If QP attributes are associated with the listening endpoint, the returned rdma_cm_id will also reference an allocated QP.
```



gdb调试rdma_client 

```
scp ./rdma_client root@192.168.56.101:/home/rdma/rdma_examples
scp ./rdma_server root@192.168.56.101:/home/rdma/rdma_examples

# 101 server
/home/rdma/rdma_examples/rdma_server


# 102 client
./rdma_client -s 192.168.56.101
gdbserver :1234 ./rdma_client -s 192.168.56.101

gdbserver :1234 ./rdma_server

```

gdb调试rdma_server

```
# 101 client
/home/rdma/rdma_examples/rdma_client -s 192.168.56.102

# 102 server
gdbserver :1234 ./rdma_server
```

先运行server，不运行client，发现server 端阻塞在了 rdma_get_request这里等待连接，clion就定在这里了，什么都不显示了，直到运行client



调试rping

```
scp ./rping root@192.168.56.101:/home/rdma/rdma_examples

# 101
./rping -s -vVd -C 2

# 102
./rping -c -C 2 -vVd -a 192.168.56.101
```



rping client 处理的event，client就这3个event

```
RDMA_CM_EVENT_ADDR_RESOLVED
RDMA_CM_EVENT_ROUTE_RESOLVED
RDMA_CM_EVENT_ESTABLISHED
```



server端event

```
RDMA_CM_EVENT_CONNECT_REQUEST
RDMA_CM_EVENT_ESTABLISHED
RDMA_CM_EVENT_DISCONNECTED
```









## rdma

struct ibv_ah * ibv_create_ah（ struct ibv_pd * pd， struct ibv_ah_attr * attr）  

ibv_create_ah 创建了一个 AH。 AH 包含到达远程目的地的所有必要数据。在 连接传输模式（ RC， UC）中， AH 与队列对（ QP）相关联。在数据报传输模式 （ UD）中， AH 与工作请求（ WR）相关联  



ibv_get_cq_event 等待在指示的完成信道（ CC）上有发送通知。请注意，这是 一个阻塞操作。用户应该分配指向 struct ibv_cq 的指针和要传递给函数的 void。他们将在返回时填入适当的值。用户有责任释放这些指针。 发送的每个通知必须使用 ibv_ack_cq_events 操作进行确认。由于 ibv_destroy_cq 操作等待所有事件被确认，因此如果没有正确确认任何事件， 它将挂起。 一旦在 CC 上发送完成队列（ CQ）的通知，该 CQ 现在被“撤防”并且将不再向 CC 发送通知，直到通过对 ibv_req_notify_cq 操作的新调用再次重新启动它。 该操作仅通知用户 CQ 具有要处理的完成队列条目（ CQE），它实际上不处理 CQE。用户应使用 ibv_poll_cq 操作来处理 CQE。  



3.6.11 ibv_init_ah_from_wc 

模板： int ibv_init_ah_from_wc（ struct ibv_context * context， uint8_t port_num， struct ibv_wc * wc， struct ibv_grh * grh， struct ibv_ah_attr * ah_attr） 

输入参数： context 来自 ibv_open_device 的 struct ibv_context。这应该是收到完成队 列条目（ CQE）的设备。 port_num 在 wc 上收到 CQE 的 port_num 物理端口号（ 1..n） wc 从 ibv_poll_cq 收到 CQE grh 来自数据包的 grh 全局路由头（ GRH）（参见说明）

输出参数： ah_attr 地址句柄（ AH）属性 

返回值： 成功时为 0，错误时为-1。如果调用失败，则将 errno 设置为指示失败的原 因。  

> ibv_init_ah_from_wc 使用必要的属性初始化 AH 以生成对接收的数据报的响 应。用户应分配 struct ibv_ah_attr 并将其传入。如果合适，还应传入接收到 的数据包中的 GRH。在 UD 连接上，接收数据包的前 40 个字节可能包含 GRH。 CQE 的 IBV_WC_GRH 标志表示该标头是否存在。如果 UD 连接上的数据包中不存 在 GRH，则数据包的前 40 个字节是未定义的。 当函数 ibv_init_ah_from_wc 完成时，将填写 ah_attr，然后可以在 ibv_create_ah 函数中使用 ah_attr。用 户负责释放 ah_attr。 或者，可以使用 ibv_create_ah_from_wc 代替此操作。  



模板： int rdma_accept（ struct rdma_cm_id * id， struct rdma_conn_param * conn_param） 

输入参数： id RDMA 通信标识符 conn_param 可选的连接参数（在 rdma_connect 下描述） 

输出参数： 没有 

返回值： 成功时为 0，错误时为-1。如果调用失败，则将 errno 设置为指示失败的原 因。 

描述： 从侦听端调用 rdma_accept 以接受连接或数据报服务查找 请求。 与套接字接 受例程不同，在侦听 rdma_cm_id 时不调用 rdma_accept。相反，在调用 rdma_listen 之后，用户等待 RDMA_CM_EVENT_CONNECT_REQUEST 事件发生。连 接请求事件为用户提供了新创建的 rdma_cm_id，类似于新套接字，但 rdma_cm_id 绑定到特定的 RDMA 设备。**在新的 rdma_cm_id 上调用 rdma_accept。**  

















