/************************************************
 * Name: network.c
 * Date: 2020-08-08
 * Description: socket服务器功能
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "network.h"
#include "store.h"
#include "business.h"

// 服务器端文件描述符
static int g_serverfd = -1;

/*
 * function:    createServer
 * description: 创建socket服务器
 * @param [ in] 
 * 	 port     : 服务器端口号
 * @return      服务器端socketfd
 */
int createServer(uint port)
{
	// 创建socket套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	TRY_ERROR(sockfd==-1, "socket() error", return FuncError);

	// 服务端地址信息初始化
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定地址信息
    int ret = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	TRY_ERROR(ret==-1, "bind() error", return FuncError);

	// 监听端口
    ret = listen(sockfd, SERVER_BACKLOG);
	TRY_ERROR(ret==-1, "listen() error", return FuncError);

	
	g_serverfd = sockfd;
	return sockfd;
}

/*
 * function:    closeServer
 * description: 关闭socket服务器
 * @return      0
 */
int closeServer()
{
	close(g_serverfd);
	return FuncNormal;
}

/*
 * function:    closeServer
 * description: 接受客户端连接
 * @return      客户端sockfd
 */
int acceptConnect()
{
	// 接受连接请求
	struct sockaddr_in client_addr = {0};
	socklen_t addrlen = sizeof(client_addr);
	int client_fd = accept(g_serverfd, (struct sockaddr*)&client_addr, &addrlen);
	TRY_ERROR(client_fd==-1, "accept() error", return FuncException);

	// 打印连接信息
	char client_ip[20] = {0};
	int client_port = ntohs(client_addr.sin_port);
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
	printf("%s:%d 已连接\n", client_ip, client_port);
	
	return client_fd;
}

/*
 * function:    closeServer
 * description: 接收/处理 客户端消息
 * @param [ in] 
 * 	 cfd      : 客户端sockfd
 * @return      0:处理成功 !0:处理出错
 */
int receviesMessage(int cfd, RequestInfo *head, void **data)
{
	// 接收请求头
	int ret = recv(cfd, head, sizeof(RequestInfo), 0);
	if (ret<=0) return FuncException;
	//TRY_ERROR(ret<=0, "recv() error", return FuncException);
	// 接受请求体
	if (head->size>0 && data!=NULL)
	{
		*data = malloc(head->size);
		bzero(*data, head->size);

		ret = recv(cfd, *data, head->size, 0);
		if (ret<=0) 
		{
			free(*data);
			return FuncException;
		}
	}

	return FuncNormal;
}

/*
 * function:    responseMessage
 * description: 响应结果
 * @param [ in] 
 * 	 cfd      : 客户端sockfd
 * @return      0:处理成功 !0:处理出错
 */
int responseMessage(int cfd, ResponseInfo *head, void *data)
{
	// 发送请求头
	int ret = send(cfd, head, sizeof(ResponseInfo), 0);
	TRY_ERROR(ret<=0, "send() error", return FuncException);

	// 发送请求体	
	if ((head->size > 0) && (data != NULL))
	{
		ret = send(cfd, data, head->size, 0);
		TRY_ERROR(ret<=0, "send() error", return FuncException);
	}

	return FuncNormal;
}

