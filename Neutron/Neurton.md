## Neutron介绍


### 常用的Linux网络技术

+ Linux网桥：Linux网桥是一个可以连接不同网络设备的虚拟设备，利用Linux内核中的实现，类似hub设备
+ OVS网桥：软件定义网络中支持OpenFlow协议的一种连接不同网络设备的虚拟设备，利用流表规则对数据报进行转发策略的设计
+ VETH：虚拟网络设备对，一般成对使用，用于连接两个不同网络命名空间的网络设备