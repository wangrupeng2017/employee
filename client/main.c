
#include "employee.h"
#include <stdlib.h>
#define SERVER_IP "192.168.1.103"
#define SERVER_PORT 9999

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

	int ret = -1;
	int fd;

	fd = connectServer(SERVER_IP, SERVER_PORT);
	TRY_ERROR(fd < 0,"连接服务器失败");

	LoginResultModel login_result = {0};
	LoginModel login_model = {0};
	do{
		ret = loginBusiness(fd, &login_model, &login_result);
		printf("%s==%s==%d\n", __FILE__, __func__, __LINE__);
		if(login_result.role == EmployeeRole){
			// 普通用户的操作
			doEmployeeBusiness(fd, &login_result);
		}else{
			// 管理员的操作
		}
		bzero(&login_model, sizeof(LoginModel));
		bzero(&login_result, sizeof(LoginResultModel));
	} while (ret);
	
	close(fd);
	return FuncNormal;
}
