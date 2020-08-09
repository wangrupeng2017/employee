#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket_models.h"

#define TRY_ERROR(expr, fname, ...)  if (expr) { perror(fname); __VA_ARGS__; exit(1); }

#define SERVER_PORT 9999
#define SERVER_IP   "127.0.0.1"
#define QUIT        "quit"

/**
* tcp-客户端
* 1.创建socket套接字(socket)流式套接字
* 2.连接服务端(connect)
* 3.收/发消息(recv/send)
* 4.关闭套接字(close)
*/
int main(int argc, const char *argv[])
{
    // 1.创建socket套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    TRY_ERROR(sockfd==-1, "socket");

    // 格式化服务端地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(SERVER_PORT);
    int ret = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);
    TRY_ERROR(ret==-1, "inet_pton", goto error_label);

    // 2.连接服务端
    ret = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    TRY_ERROR(ret==-1, "connect", goto error_label);
/** // 登录
	RequestInfo req = {
		.type = Login,
		.size = sizeof(LoginModel)
	};
	LoginModel model = {
		.name = "123",
		.pwd  = "123"
	};
	ResponseInfo res = {0};
	LoginResultModel res_model = {0};
**/
/** // 退出
	RequestInfo req = {
		.type = Quit,
		.size = sizeof(QuitModel)
	};
	QuitModel model = {
		.empno = 1
	};
	ResponseInfo res = {0};
	LoginResultModel res_model = {0};
**/
/** // 查询
	RequestInfo req = {
		.type = EmployeeQuery,
		.size = sizeof(EmployeeQueryModel)
	};
	EmployeeQueryModel model = {
		.name  = "",
		.empno = 1
	};
	ResponseInfo res = {0};
	EmployeeQueryResult res_model = {0};
**/
// 修改
	RequestInfo req = {
		.type = EmployeeModify,
		.size = sizeof(EmployeeModifyModel)
	};
	EmployeeModifyModel model = {
		.empno = 2,
		.name  = "王五",
		.pwd   = "123",
		.sex   = 1,
		.age   = 10,
		.salary = 1000,
		.department = "网络部"
	};
	ResponseInfo res = {0};
	EmployeeQueryResult res_model = {0};

/** // 创建
	RequestInfo req = {
		.type = EmployeeAdd,
		.size = sizeof(EmployeeCreateModel)
	};
	EmployeeCreateModel model = {
		.name  = "张三",
		.pwd   = "123",
		.sex   = 1,
		.age   = 10,
		.salary = 1000,
		.department = "网络部"
	};
	ResponseInfo res = {0};
	EmployeeCreateResult res_model = {0};
**/
/** // 删除
	RequestInfo req = {
		.type = EmployeeDelete,
		.size = sizeof(EmployeeDeleteModel)
	};
	EmployeeDeleteModel model = {
		.empno = 1
	};
	ResponseInfo res = {0};
	EmployeeCreateResult res_model = {0};
**/
/** // 日志查询
	RequestInfo req = {
		.type = LogsQuery,
		.size = sizeof(LogQueryModel)
	};
	LogQueryModel model = {
		.date = "2018-09-10"
	};
	ResponseInfo res = {0};
	LogQueryResult res_model = {0};
**/
    while(1)
    {
		// 发送消息
		ret = send(sockfd, &req, sizeof(req), 0);
		if (ret <=0 )
		{/*{{{*/
			puts("发送请求头失败");
			return -1;
		}/*}}}*/
		puts("发送请求头成功");

		ret = send(sockfd, &model, sizeof(model), 0);
		if (ret <=0 )
		{/*{{{*/
			puts("发送请求体失败");
			return -1;
		}/*}}}*/
		puts("发送请求体成功");


		// 接受处理结果
		bzero(&res, sizeof(ResponseInfo));
		ret = recv(sockfd, &res, sizeof(ResponseInfo), 0);
		if (ret <=0 )
		{/*{{{*/
			puts("接收响应头失败");
			return -1;
		}/*}}}*/
		printf("%s, %d, %d\n", res.message, ret, sizeof(ResponseInfo));
		if (res.size > 0)
		{/*{{{*/
			ret = recv(sockfd, &res_model, sizeof(res_model), 0);
			if (ret <=0 )
			{
				puts("接收响应头失败");
				return -1;
			}
		}/*}}}*/


		

		sleep(20);
    }

    // 4.关闭套接字
    close(sockfd);
    return 0;
error_label:
    close(sockfd);
    return -1;
}
