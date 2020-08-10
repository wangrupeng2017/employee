
## 员工管理系统服务端
### 使用教程
>首先，进入server目录下执行make命令，会自动编译生成可执行程序main

>然后，执行可执行程序main并传入服务端口号，eg: ./main 9999

>程序启动会自动创建employee.db数据库，并==自动创建了admin账户，密码为admin==和==test账户，密码为test==

### 主要应用技术
>sqlite数据库

>socket网络通信

>poll多路复用

### 主要文件说明
>main.c 程序的入口，完成数据库/服务器的初始化

>network 对socket进行了基本的封装,提供了网络的基本操作方法

>store 对sqlite进行了基本的封装，提供了数据库的操作方法

>iopoll 对poll进行了基本封装，提供简便的IO监听操作

>business 对客户端业务请求的功能处理方法实现
