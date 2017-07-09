## 虚拟网络技术

+ Linux网桥：Linux网桥是一个可以连接不同网络设备的虚拟设备，利用Linux内核中的实现，类似hub设备
+ OVS网桥：软件定义网络中支持OpenFlow协议的一种连接不同网络设备的虚拟设备，利用流表规则对数据报进行转发策略的设计
+ VETH：虚拟网络设备对，一般成对使用，用于连接两个不同网络命名空间的网络设备
+ br-int：bridge-integration，Neutron中综合网桥，表示实现主要内部网络功能的网桥
+ br-ex：bridge-external，Neutron中外部网桥，负责跟外部网络通信的网桥
+ GRE：General Routing Encapsulation，一种通过封装来实现隧道的方式。在Neutron中一般是基于L3的gre
+ TAP设备：模拟一个二层的网络设备，可以接受和发送二层网包
+ TUN设备：模拟一个三层的网络设备，可以接受和发送三层网包
+ VLAN：虚拟Lan，同一个物理Lan下用标签实现隔离
+ VXLAN：一套利用 UDP 协议作为底层传输协议的Overlay实现。一般认为作为VLAN技术的延伸或替代者。
+ namespace：用来实现隔离的一套机制，不同namespace中的资源之间彼此不可见
