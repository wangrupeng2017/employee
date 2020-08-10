#define _GNU_SOURCE 1
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "iopoll.h"
#include "store.h"
#include "network.h"

#define POLL_TIMEOUT  (1000*5)

// 服务端socket描述符
static int g_serverfd = -1;

// poll回调方法
void iopoll_call(int fd, int revents)
{
	// 客户端强制退出事件
	if (revents&POLLRDHUP)
	{
		printf("[%d] 客户端已退出\n", fd);
		iopollDel(fd);
		return;
	}

	// 有新客户端连接
	if ((fd==g_serverfd) && (revents&POLLIN))
	{
		// 接受客户端连接
		int client_fd = acceptConnect();
		if (client_fd<0) return;

		// 客户端sockfd添加到poll中
		iopollAdd(client_fd, POLLIN);
		return;
	}

	// 收到客户端消息
	if (revents&POLLIN)
	{
		int ret = businessEntrance(fd);
		if (ret < 0) 
		{
			close(fd);
			iopollDel(fd);
		}
	}
}


int main(int argc, const char *argv[])
{
	// 参数检查
	if (argc != 2)
	{
		printf("启动时请指定端口号(eg): %s 9999\n", argv[0]);
		return -1;
	}

	// 创建数据库实例
	sqlite3 *db = NULL;
	int ret = initSQL(&db);
	if (ret < 0) return FuncError;

	// 创建socket服务
	g_serverfd = createServer(atoi(argv[1]));
	if (g_serverfd < 0) return FuncError;
	
	// 添加server_fd到poll中
	iopollAdd(g_serverfd, POLLIN);

	while(1)
	{
		int ret = iopollWait(POLL_TIMEOUT, iopoll_call);
		//TRY_ERROR(ret==0, "poll() timeout", continue);
		TRY_PERROR(ret==-1, "poll():", return FuncError);
	}


	// 关闭数据库
	closeSQL();
	// 关闭服务器socket
	closeServer();
	return 0;
}



