## Docker Deep Dive

### 准备工作

+ 安装Ubuntu Server 16.04 LTS

  + 下载最新的Ubuntu Server 16.04 LTS
  + 用VirtualBox创建Ubuntu Server的虚拟机

+ 安装Docker引擎
  + 参考[网上教程](https://docs.docker.com/engine/installation/linux/ubuntu/)进行安装即可
  + 添加用户至docker组
  + 配置Docker网络，将客户端与服务端分离

说明：docker必须以root用户执行;需要绑定`/var/run/docker.sock`。

**添加用户至docker组**

```
# 查看docker引擎的权限，发现docker属于root用户(rw)，docker所在组(rw)，其他用户没有读写权限
# 以root用户执行
$ ls -l /run
# 查看docker用户组内的用户，并将当前用户加入docker用户组
$ cat /etc/group
docker:x:117
# 以当前用户执行，用户名为subond
$ sudo gpasswd -a subond docker
Adding user subond to group docker
$ cat /etc/group
docker:x:117:subond
```

**配置Docker网络**

```
# 停止docker server的服务，然后修改docker.server文件
# 文件为/lib/systemd/system/docker.server
$ service docker stop
# 修改ExecStart=/usr/bin/docker -d -H fd://
# 为ExecStart=/usr/bin/docker -d -H <HOST_IP>:2375
# 重启docker服务
$ service docker start

# 添加环境变量
$ exprot DOCKER_HOST="tcp://<HOST_IP>:2375"
```
