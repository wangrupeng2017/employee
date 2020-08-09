#define _GNU_SOURCE 1
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "mysqlite3.h"
#include "socket.h"


#define POLL_TIMEOUT  (1000*5)
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
	int server_fd = createServer(atoi(argv[1]));
	if (server_fd < 0) return FuncError;
	
	// poll服务器相关
	struct pollfd *pollfds = NULL;
	int nfds = 0;

	struct pollfd pfd = {0};
	pfd.fd = server_fd;
	pfd.events = POLLIN;
	nfds = set_pollfd(&pollfds, nfds, pfd);

	while(1)
	{
		int ret = poll(pollfds, nfds, POLL_TIMEOUT);
		//TRY_ERROR(ret==0, "poll() timeout", continue);
		TRY_PERROR(ret==-1, "poll():", return FuncError);

		int i       = 0;
		int sockfd  = 0;
		int revents = 0;
		for (i=0; i<nfds; i++)
		{
			sockfd  = pollfds[i].fd;
			revents = pollfds[i].revents;

			if (revents == 0)
				continue;

			// 客户端强制退出事件
			if (revents&POLLRDHUP)
			{
				printf("[%d] 客户端已退出\n", sockfd);
				nfds = del_pollfd(&pollfds, nfds, pollfds[i]);
				i -= 1;
				continue;
			}
			
			// 有新客户端连接
			if ((sockfd==server_fd) && (revents&POLLIN))
			{
				// 接受客户端连接
				int client_fd = acceptConnect();
				if (client_fd<0) continue;

				// 客户端sockfd添加到poll中
				struct pollfd client_pfd = {0};
				client_pfd.fd = client_fd;
				client_pfd.events = POLLIN;
				nfds = set_pollfd(&pollfds, nfds, client_pfd);
				continue;
			}

			// 收到客户端消息
			if (revents&POLLIN)
			{
				ret = receviesMessage(pollfds[i].fd);
				// 断开客户端连接
				if (ret<0)
				{
					nfds = del_pollfd(&pollfds, nfds, pollfds[i]);
					i -= 1;
				}
			}
		}
	}


	// 关闭数据库
	closeSQL();
	// 关闭服务器socket
	closeServer();
	return 0;
}



