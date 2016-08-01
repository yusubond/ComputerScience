## OAI项目开发

### 1.实现UE与eNodeB的消息互通

实现思路：
- 采用socket通信方式，以upd协议创建套接字，UE做服务端，eNodeB做客户端；
- 在核心网EPC中的pgw网元做端口映射；

具体实现：

1. [UE服务端源代码](./ue_server.c)
2. [eNodeB客户端源代码](./enb_client.c)
3. 利用iptables工具在pgw上做端口映射，例如UE(IP:192.188.0.2; PORT:2345),eNodeB(IP:10.103.14.28; PORT:2345),pgw(IP:10.103.12.228)

`sudo iptables -t nat -A PREROUTING --dst 10.103.12.228 -p udp --port 2345 -j DNAT --to-destination 192.188.0.2:2345`
