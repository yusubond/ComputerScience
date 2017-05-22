## Vagrant使用技巧

### 更新box内核版本

```
# 登录进去后，更新至指定版本的内核
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo dpkg -i path/to/*.deb
# 然后，利用vagrant重载或者借助virtualbox进行重启
