
#include "client.h"
#include <stdlib.h>
#define SERVER_IP "192.168.1.103"
#define SERVER_PORT 6666

/*
 * description : 这个目前是为了测试用的，后期再修改
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int main(int argc, const char *argv[])
{
#if 1
	int ret = -1;
	int fd;

	fd = connectServer(SERVER_IP, SERVER_PORT);
	TRY_ERROR(fd < 0,"连接服务器失败");

	LoginResultModel login_result = {0};
	LoginModel login_model = {0};
	ret = loginBusiness(fd, &login_model, &login_result);
	TRY_ERROR(ret, "登陆失败", goto error_label1);
	if(login_result.role == EmployeeRole){
		// 普通用户的操作
		doEmployeeBusiness(
	}else{
		// 管理员的操作
	}

	return FuncNormal;

#endif
error_label1:
	close(fd);
	return FuncException;
}
