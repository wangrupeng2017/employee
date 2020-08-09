/*******************************************************
 * File Name   : client.c
 * Version     : V1.0
 * Author      : yuanbingxu
 * Organization: https://github.com/xuyuanbing
 * Create Time : 2020-08-08 08:18:14
 * Description : 员工管理系统，客户端
 *******************************************************/
#include "client.h"

// 连接socket服务器 

/*
 * Description 		: 连接socket服务器
 * function 		:   connectServer 
 * @param [ in] 
 * 		char [20] ip 服务端ip 地址
 * 		int port     服务端端口号
 * @param [out] 
 * @return      
 * 		int 		 已连接的文件描述符
 * @Author      xuyuanbing
 * @Other       
 */
int connectServer(char *ip, int port)
{
	int fd = -1;
	if( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("open socket tcp");
		return fd;
	}


	// 创建结构体
	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	
	if(inet_pton(AF_INET, ip, (void *)&server_address.sin_addr.s_addr) != 1){
		perror("string ip convert");
		return -1;
	}

	if( connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0 ){
		perror("connect service");
		return -1;
	}

	return fd;
}

/*
 * function:    
 * @param [ in] 
 * @param [out] 
 * @return      
 * @Author      xuyuanbing
 * @Other       
 */

// 打印登陆菜单
void showLoginMenu(void)
{
	printf("***************************************************\n");
	printf("***********   欢迎登陆员工管理系统   **************\n");
	printf("***************************************************\n");
}

/*
 * description : 打印普通用户菜单
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
void showEmployeeMenu(void)
{
	printf("********************  请选择所需的服务  ********************\n");
	printf("**************  1:信息查询  2:信息修改 0:退出 **************\n");
	printf("************************************************************\n");
}

/*
 * description : 打印管理员菜单
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
void showAdminMenu(void)
{
	printf("********************  请选择所需的服务  ********************\n");
	printf("**************  1:添加用户  2:删除用户 3:信息查询 **************\n");
	printf("**************  4:修改用户  5:查询操作 0:退出 **************\n");
	printf("************************************************************\n");
}

// 打印首页菜单
void showHomeMenu(void)
{


}

// 接收菜单输入(注意:输入格式并清除输入缓冲区)
int getHomeMenuChoose(void)
{
	char choose = -1;
	printf("请输入您需要的服务：");
	while(!(choose = getchar()));
	
	return choose - '0';
}

// 解析菜单输入
void gotoChoose(int userChoose, unsigned char is_admin)
{
	switch(userChoose){
	case 1:
		if(is_admin){
			// TODO
		}
		break;
	case 2:
		if(is_admin){
			// TODO
		}

		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 0:
		break;
	default :
		break;
	}
}


// 登录业务:
int loginBusiness(int fd, LoginResultModel *result_model)
{
	int ret = 1;
	LoginModel login_model;
	if( Failed == getLoginModel(&login_model)){
		return Failed;
	}
	if(Failed == sendLoginRequest(fd, &login_model, &result_model)){
		return Failed;
	}

#if 0
	printf("login result modle, result_model.token= %s result_model.name= %s result_model.empno = %d\n",
			result_model.token, result_model.name, result_model.empno);
#endif
}


/*
 * description : 交互获取登录信息 用户名/密码 格式化登录数据
 * function    : getLoginModel
 * @param [ in]: 
 * @param [out]: 
 * 		LoginModel *model 用户登陆信息
 * @return     : 
 * 		int 0: 获取用户信息成功
 * 			1: 获取用户信息失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getLoginModel(LoginModel *model)
{
	char word[32] = {0};
	printf("请输入您的用户名：");
	fgets(word, sizeof(word), stdin);
	word[strlen(word) -1] = '\0'; // fgets 自动在最后添加一个 '\n' 
	strncpy(model->name, word, sizeof(model->name) - 1);

	printf("请输入您的密码：");
	memset(word, 0, sizeof(word));
	fgets(word, sizeof(word), stdin); 
	word[strlen(word) -1] = '\0';  // fgets 自动在最后添加一个 '\n'
	strncpy(model->pwd, word, sizeof(model->pwd) - 1);

	return 0;
}

/*
 * description : 发送登录请求, 接收结果
 * function    : sendLoginRequest
 * @param [ in]: 
 * 		LoginModel *model 用户登陆信息
 * @param [out]: 
 *  	LoginResultModel *out 服务器端返回的信息
 * @return     : 
 * 		int 0: 发送登陆请求成功
 * 			1: 发送登陆请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
static int sendLoginRequest(int fd, LoginModel *model, LoginResultModel *out)
{

	int ret = -1;
	do
	{
		ret = send(fd, (void *)model, sizeof(LoginModel), 0);
	} while (ret < 0 && EINTR == errno);

	do
	{
		ret = recv(fd, (void *) out, sizeof(LoginResultModel), 0);
	} while (ret < 0 && EINTR == errno);
	if( ret < 0 ){
		perror(" send login model");
	}

	return ret < 0 ? Failed: Success;
}

