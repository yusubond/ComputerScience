## TCP:传输控制协议

一个TCP连接由一个4元组唯一确定：本地IP,本地端口号，远端IP,远端端口号。

### TCP首部

TCP提供一种 **面向连接的**、**可靠** 的 **字节流服务**。面向连接意味着两个使用TCP应用的应用(客户端，服务端)在彼此交换数据之前必须建立TCP连接。TCP通过以下方式来提供可靠性：

1）应用数据被分割成TCP认为最适合发送的数据块。(这与UDP完全不同，应用数据产生的数据报长度将保持不变。)

2）超时重传策略。即TCP发出一个段后，启动一个定时器，等待目的地址确认收到这个报文段。如果不能及时收到一个确认，将重发这个报文段。

3）TCP收到发自TCP连接另一端的数据，将发送一个确认。(注意，不是立即发送，而是推迟几分之一秒)

4）TCP将保持 **首部和数据的校验和**。这是一个 **端到端的校验和**，目的是检测数据在传输过程中的任何变化。如果收到的校验和有差错，则丢弃该报文段和不确认收到报文段，从而出发超时重传。

5）如果必要，TCP将对收到的数据进行重新排序，以正确的顺序交给应用层。

6）丢弃重复的数据。(因为IP数据报会出现重复。)

7）流量控制。即TCP连续的每一方都有 **固定大小的缓冲空间**，TCP接收端只允许另一端发送收端缓冲区能接纳的数据。

![TCP-HEAD](http://on64c9tla.bkt.clouddn.com/Comput/tcp_head.png)

+ 每个TCP段都包含源端口和目的端口，用于寻找发端和收端的应用进程。(IP地址和端口号唯一确定一个TCP连接)  
+ 序号用来标识从TCP发端到收端发送的数据字节流，表示这个报文段中的第几个数据字节。(数据排序使用，序号是32bit的无符号数)  
+ 确认序号用于收端确认收到的数据字节，为已成功收到的数据字节序号加1。只有在ACK标志位为1时，确认序号才有效。  
+ TCP提供 **全双工服务**，*即数据能够在两个方向上独立地进行传输，连接的每一端必须保持每个方向上的传输数据序号*。  
+ 首部长度给出首部中32bit字的数目。TCP最多可有60字节的首部，没有任选字段，**正常的长度是20字节**。  
+ 6个标志比特。
   * URG  紧急指针有效
   * ACK  确认序号有效
   * PSH  接收方应该尽快将这个报文段交给应用层
   * RST  重建连接
   * SYN  同步序号用来发起一个连续
   * FIN  发端完成发送任务  
+ 流量控制，通过每一端的窗口大小来提供。  
+ 检验和覆盖整个TCP报文段：TCP首部和TCP数据。一定是发端计算和存储，并由收端进行验证。
+ 最常见的可选字段是最长报文大小。

### TCP的建立和终止

建立：**三次握手**

![three-way-handshake](http://on64c9tla.bkt.clouddn.com/Comput/three-way-handshake.png)

第一次握手：Client将标志位SYN（即同步信号）置为1，随机产生一个值seq=J，并将该数据包发送给Server，Client进入SYN_SENT状态，等待Server确认。

第二次握手：Server收到数据包后由标志位SYN=1（即同步信号置1）知道Client请求建立连接，Server将标志位SYN（用于建立服务端-客户端的连接的同步信号）和ACK（用于确认客户端-服务端的连接有效）都置为1，ack=J+1，随机产生一个值seq=K，并将该数据包发送给Client以确认连接请求，Server进入SYN_RCVD状态。

第三次握手：Client收到确认后，检查ack是否为J+1，ACK是否为1，如果正确则将标志位ACK置为1，ack=K+1，并将该数据包发送给Server，Server检查ack是否为K+1，ACK是否为1，如果正确则连接建立成功，Client和Server进入ESTABLISHED状态，完成三次握手，随后Client与Server之间可以开始传输数据了。

终止：**四次挥手**

![four-way-wavehand](http://on64c9tla.bkt.clouddn.com/Comput/four-way-wavehand.png)

第一次挥手：Client发送一个FIN（用于关闭Client到Server的数据传送），Client进入FIN_WAIT_1状态。

第二次挥手：Server收到FIN后，发送一个ACK=1（用于确认结束连接的请求）给Client，确认序号为收到序号+1（与SYN相同，一个FIN占用一个序号），Server进入CLOSE_WAIT状态；Client收到ACK=1的消息后，检查序号是否+1？是，则进入FIN_WAIT_2状态。

第三次挥手：Server发送一个FIN（用于关闭Server到Client的数据传送）Server进入LAST_ACK状态。

第四次挥手：Client收到FIN后，Client进入TIME_WAIT状态，接着发送一个ACK给Server，确认序号为收到序号+1，Server进入CLOSED状态，完成四次挥手。

通常一方完成主动关闭而另一方完成被动关闭。**因为TCP连接是全双工，所以每个方向必须单独进行关闭**。

```
客户端状态转移：连接状态(主动断开) -> FIN_WAIT_1        ->        FIN_WAIT_2   -> TIME_WAIT.

服务端状态转移：连接状态(被动断开)         ->       CLOSE_WAIT        ->  LAST_ACK       -> CLOSED.
```
**最大报文段长度** MSS

最大报文段长度表示TCP传往另一端的最大块数据的长度。当一个连接建立时，连接的双方都要告知各自的MSS值。

TCP的半关闭：TCP连接的一端在结束它的发送后还能接收来自另一端数据的能力。

> 为什么要有TIME_WAIT状态？

链接：https://blog.csdn.net/qq_41804181/article/details/81941783

### TCP状态变迁图

![tcp-status](http://on64c9tla.bkt.clouddn.com/Comput/tcp-status.png)
