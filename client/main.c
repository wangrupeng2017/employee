
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
	printf("main connect failed fd = %d\n", fd);
	if( fd < 0 ){
		exit(1);
	}
	ret = loginBusiness(fd);
	printf(" login_business ret = %d\n", ret);
#else
	LoginModel model;
	getLoginModel(&model);

#endif
	return 0;
}
