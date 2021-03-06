/*******************************************************
 * File Name   : client/client2.c
 * Version     : V1.0
 * Author      : yuanbingxu
 * Organization: https://github.com/xuyuanbing
 * Create Time : 2020-08-09 20:37:22
 * Description : 
 *******************************************************/
#include "client.h"

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
int connectServer(const char *ip, int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	TRY_ERROR(fd == -1, "open socket tcp");


	// 创建结构体
	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	int ret = inet_pton(AF_INET, ip, (void *)&server_address.sin_addr.s_addr);
	TRY_ERROR(ret == -1, "string ip convert", goto error_label);

	ret = connect(fd, (struct sockaddr *)&server_address, sizeof(server_address));
	TRY_ERROR( ret == -1, "connect service", goto error_label);

	return fd;

error_label:
	close(fd);
	return -1;
}

/*
 * description : 登录业务
 * function    : 
 * @param [ in]: 
 * 		int fd, 
 * 		LoginModel *login_model
 * @param [out]: 
 * 		LoginResult *result_model
 * @return     : 
 *    0:登陆成功 !0: 登陆失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int loginBusiness(int fd, LoginModel *login_model, LoginResult *result_model)
{
	int ret = FuncException;
	showLoginMenu();
login_label:
	switch(getHomeMenuChoose()){
	case LOGIN_CHOOSE:
		getLoginModel(login_model);
		ret = sendLoginRequest(fd, login_model, result_model);
		break;
	case LOGOUT_CHOOSE:
		exit(FuncNormal);
	default:
		printf("输入错误！\n");
		goto login_label;
	}

	return ret;
}

// 打印登陆菜单
void showLoginMenu(void)
{
	printf("***************************************************\n");
	printf("***********   欢迎使用员工管理系统   **************\n");
	printf("***********  1:登陆系统  0:退出系统  **************\n");
	printf("***************************************************\n");
}

// 接收菜单输入(注意:输入格式并清除输入缓冲区)
int getHomeMenuChoose(void)
{
	char choose = -1;
	printf("请输入您需要的服务：");
	while(!(choose = getchar()));
	while(getchar() != '\n'); //去掉空格
	return choose - '0';
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

	memset(word, 0, sizeof(word));
	printf("请输入您的用户名：");
	getDataFgets(word, sizeof(word));
	strncpy(model->name, word, sizeof(model->name) - 1);

	printf("请输入您的密码：");
	memset(word, 0, sizeof(word));
	getDataFgets(word, sizeof(word));
	strncpy(model->pwd, word, sizeof(model->pwd) - 1);

	return 0;
}

/*
 * description : 发送登录请求, 接收结果
 * function    : sendLoginRequest
 * @param [ in]: 
 * 		LoginModel *model 用户登陆信息
 * @param [out]: 
 *  	LoginResult *out 服务器端返回的信息
 * @return     : 
 * 		int 0: 发送登陆请求成功
 * 			1: 发送登陆请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
static int sendLoginRequest(int file_descriptor, LoginModel *model, LoginResult *login_result_model)
{
	RequestInfo req = {
		.type = Login,
		.size = sizeof(LoginModel)
	};
	ResponseInfo res = {0};

	int ret = FuncException;
	ret = request(file_descriptor, &req, sizeof(RequestInfo), model, sizeof(LoginModel),
			&res, sizeof(ResponseInfo), login_result_model, sizeof(LoginResult));
	if( res.result == Failed ){
		printf("%s\n", res.message);
	}

	return ret;
}

/*
 * description : 通用的请求服务端的方法 不是放在这个文件的 TODO
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		void * request_data 请求信息
 * 		size_t request_data_size
 * 		size_t response_data_size
 * @param [out]: 
 * 		void * response_data 请求的返回结果
 * @return     : 
 * 		0:请求成功 !0:请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */

int request(int file_descriptor, void * request_head, 
		size_t request_head_size, void * request_data,
		size_t request_data_size, void * response_head,
		size_t response_head_size, void * response_data, size_t response_data_size)
{
	// 输入参的基本校验 忽 TODO 
	int ret = FuncException;
	// 发送请求头
	ret = send(file_descriptor, request_head, request_head_size,0);
	TRY_PERROR(ret == FuncException, "发送请求头");

	// 发送请求数据
	ret = send(file_descriptor, request_data, request_data_size,0);
	TRY_PERROR(ret == FuncException, "发送请求数据");

	// 接受反回头
	ret = recv(file_descriptor, response_head, response_head_size, 0);
	TRY_PERROR(ret == FuncException, "接收请求头数据");
	ResponseInfo * response_info = response_head;

	if((response_info->result == Success) && (response_info->size > 0)){
		// 接受返回的数据
		ret = recv(file_descriptor, response_data, response_info->size, 0);
		TRY_PERROR(ret == FuncException, "接收请求数据");
	}

	return response_info->result;
}

void getDataFgets(char * data, size_t size)
{
	fgets(data, size, stdin);
	if(strlen(data) < size -1){
		data[strlen(data) -1] = '\0'; // fgets 自动在最后添加一个 '\n'
	}
}

/*
 * description : 普通员工退出业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor 
 * 		LoginModel *login_model
 * @param [out]: 
 * @return     : 返回值: 0:成功 !0:出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int employeeQuitBusiness(int file_descriptor, LoginResult *login_model)
{
	printf("您如果确认退出请输入(yes):");
	char temp [12] = {0};
	fgets(temp, sizeof(temp), stdin);
	// 用户不退去了
	if(strncasecmp(temp, YES, strlen(YES))){
		return FuncException;
	}

	QuitModel quit_model = {0};
	quit_model.empno = login_model->empno;
	int ret = sendQuitRequest(file_descriptor, &quit_model);

	return ret;
}


/*
 * description : 退出请求信息, 发送退出请求
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor 
 * 		QuitModel *quit_model 
 * @return     : 返回值: 0:成功 !0:出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendQuitRequest(int file_descriptor, QuitModel *quit_model)
{
	RequestInfo req = {
		.type = Quit,
		.size = sizeof(QuitModel)
	};
	ResponseInfo res = {0};
	int ret = request(file_descriptor, &req, sizeof(req), quit_model, sizeof(QuitModel), 
			          &res, sizeof(res), NULL, 0);
	return ret;
}



