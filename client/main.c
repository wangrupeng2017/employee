
#include "employee.h"
#include <stdlib.h>
#define SERVER_IP "127.0.0.1"
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
	if( argc > 3 || argc == 3 ){
		fd = connectServer(argv[1], atoi(argv[2]));
	}else{
		fd = connectServer(SERVER_IP, SERVER_PORT);
	}

	TRY_ERROR(fd < 0,"连接服务器失败");

	LoginResult login_result;
	LoginModel login_model;
loop_menu_label:
	bzero(&login_model, sizeof(LoginModel));
	bzero(&login_result, sizeof(LoginResult));
	ret = loginBusiness(fd, &login_model, &login_result);// 这里面直接就exit()

	if( ret ) goto loop_menu_label;

	switch(login_result.role){
	case EmployeeRole:
		doEmployeeBusiness(fd, &login_result);
		break;
	case AdminRole:
		doAdminBusiness(fd, &login_result);
		break;
	default :
		close(fd);
		exit(FuncException);
	}
	goto loop_menu_label;

}
