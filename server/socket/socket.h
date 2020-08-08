#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "../../common/common_base.h"

// 创建socket服务器
int createServer(char ip[IP_SIZE], uint port);
// 关闭socket服务器
int closeServer(int sfd);

// 接受客户端连接
int acceptConnect(int sfd);
// 接收/处理 客户端消息, 第二个参数为请求处理方法
int receviesMessage(int cfd);

#endif //__SOCKET_H__
