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

#define _GNU_SOURCE 1
#include <poll.h>

#include "common_base.h"
#include "socket_models.h"

#define SERVER_BACKLOG 100

// 创建socket服务器
int createServer(uint port);
// 关闭socket服务器
int closeServer();

// 接受客户端连接
int acceptConnect();
// 接收/处理 客户端消息
int receviesMessage(int cfd);
// 响应结果
int responseMessage(int cfd, ResponseInfo *info, void *data);

// Poll操作
int set_pollfd(struct pollfd**fds, int length, struct pollfd pfd);
int del_pollfd(struct pollfd**fds, int length, struct pollfd pfd);

#endif //__SOCKET_H__
