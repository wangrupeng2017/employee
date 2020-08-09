/************************************************
 * Name: socket.c
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

#include "socket.h"
#include "business.h"

// 服务器端文件描述符
static int server_fd = -1;

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

	
	server_fd = sockfd;
	return sockfd;
}

/*
 * function:    closeServer
 * description: 关闭socket服务器
 * @return      0
 */
int closeServer()
{
	close(server_fd);
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
	int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
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
int receviesMessage(int cfd)
{
	// 接收请求头
	RequestInfo info;
	int ret = recv(cfd, &info, sizeof(RequestInfo), 0);
	if (ret<=0) return FuncException;
	//TRY_ERROR(ret<=0, "recv() error", return FuncException);
	
	// 根据类型调用对应业务处理
	switch(info.type)
	{
		case Login: 		ret = loginHandler(cfd, &info); 		break;
		case Quit:	 		ret = quitHandler(cfd, &info);			break;
		case EmployeeQuery:	ret = employeeQueryHandler(cfd, &info);	break;
		case EmployeeModify:ret = employeeModifyHandler(cfd, &info);break;
		case EmployeeAdd:	ret = employeeAddHandler(cfd, &info);	break;
		case EmployeeDelete:ret = employeeDeleteHandler(cfd, &info);break;
		case LogsQuery:		ret = logsQueryHandler(cfd, &info);		break;
	}

	return ret;
}

/*
 * function:    responseMessage
 * description: 响应结果
 * @param [ in] 
 * 	 cfd      : 客户端sockfd
 * @return      0:处理成功 !0:处理出错
 */
int responseMessage(int cfd, ResponseInfo *info, void *data)
{
	// 发送请求头
	int ret = send(cfd, info, sizeof(ResponseInfo), 0);
	TRY_ERROR(ret<=0, "send() error", return FuncException);
	// 发送请求体	
	if ((info->size > 0) && (data != NULL))
	{
		ret = send(cfd, data, info->size, 0);
		TRY_ERROR(ret<=0, "send() error", return FuncException);
	}

	return FuncNormal;
}

/*
 * function:    设置fds中的pfd信息
 * @param [out] fds pollfd集合
 * @param [ in] length fds长度
 * @param [ in] pfd pollfd信息
 * @return      正常:fds的长度 错误:<0
 */
int set_pollfd(struct pollfd**fds, int length, struct pollfd pfd)
{/*{{{*/
	// 参数检查
	TRY_ERROR(fds==NULL, "param error", return FuncError);


	int i = 0;
	int insert_index = length;
	for (i=0; i<length; i++)
	{
		// 修改pfd信息
		if ((*fds)[i].fd == pfd.fd)
		{
			(*fds)[i].events = pfd.events;
			(*fds)[i].revents = pfd.revents;
			return length;
		}

		// 插入新pfd节点
		if ((*fds)[i].fd > pfd.fd)
		{
			insert_index = i;
			break;
		}
	}

	// 新增pfd节点
	int pfd_size = sizeof(struct pollfd);
	struct pollfd *out_fds = (struct pollfd*)malloc(pfd_size * (length+1));
	for (i=0; i<=length; i++)
	{
		if (i<insert_index) memcpy(&out_fds[i], &(*fds)[i], pfd_size);
		else if (i==insert_index) memcpy(&out_fds[i], &pfd, pfd_size);
		else if (i>insert_index) memcpy(&out_fds[i], &(*fds)[i-1], pfd_size);
	}

	if (*fds) free(*fds);
	*fds = out_fds;
	return length+1;
}/*}}}*/

/*
 * function:    从fds中删除pfd信息
 * @param [out] fds pollfd集合
 * @param [ in] length fds长度
 * @param [ in] pfd pollfd信息
 * @return      正常:fds的长度 错误:<0
 */
int del_pollfd(struct pollfd**fds, int length, struct pollfd pfd)
{/*{{{*/
	TRY_ERROR(fds==NULL, "param error", return FuncError);

	int i = 0;
	int delete_index = -1;
	for (i=0; i<length; i++)
	{
		// 要删除的节点
		if ((*fds)[i].fd == pfd.fd)
		{
			delete_index = i;
			break;
		}

		// 未找到
		if ((*fds)[i].fd > pfd.fd)
			break;
	}

	// 没有要删除的节点
	if (delete_index == -1)
		return length;

	
	// 删除pfd节点
	int pfd_size = sizeof(struct pollfd);
	struct pollfd *out_fds = (struct pollfd*)malloc(pfd_size * (length-1));
	for (i=0; i<length; i++)
	{
		if (i<delete_index) memcpy(&out_fds[i], &(*fds)[i], pfd_size);
		else if (i==delete_index) continue;
		else if (i>delete_index) memcpy(&out_fds[i-1], &(*fds)[i], pfd_size);
	}

	if (*fds) free(*fds);
	*fds = out_fds;
	return length-1;
}/*}}}*/
