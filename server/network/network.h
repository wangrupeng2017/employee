/************************************************
 * Name: socket.h
 * Date: 2020-08-08
 * Description: socket服务器功能
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/

#ifndef __SOCKET_H__
#define __SOCKET_H___

#include "common_base.h"
#include "socket_models.h"

#define SERVER_BACKLOG 100

// 创建socket服务器
int createServer(uint port);
// 关闭socket服务器
int closeServer();

// 接受客户端连接
int acceptConnect();
// 接收消息
int receviesMessage(int cfd, RequestInfo *head, void **data);
// 响应结果
int responseMessage(int cfd, ResponseInfo *head, void *data);



#endif //__SOCKET_H__
