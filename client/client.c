/*******************************************************
 * File Name   : client.c
 * Version     : V1.0
 * Author      : yuanbingxu
 * Organization: https://github.com/xuyuanbing
 * Create Time : 2020-08-08 08:18:14
 * Description : 员工管理系统，客户端
 *******************************************************/
#include "client.h"

#define TRY_ERROR(expr, fname, ...)  if (expr) { perror(fname); __VA_ARGS__; exit(1); }

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
	printf("********************  很高兴为您服务  **********************\n");
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
	printf("********************  很高兴为您服务  **********************\n");
	printf("**************  1:添加用户  2:删除用户 3:信息查询 **********\n");
	printf("**************  4:修改用户  5:查询操作 0:退出 **************\n");
	printf("************************************************************\n");
}

// 普通用户的业务
void doEmployeeBusiness(int file_descriptor, LoginModel * login_model)
{
	showEmployeeMenu();
	gotoEmployeeChoose(getHomeMenuChoose(), file_descriptor, LoginModel);

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
void gotoEmployeeChoose(int file_descriptor, int userChoose, LoginModel * login_model)
{
	// 就三种情况， 1. 继续上一级服务， 2.退出服务
	int ret = -1;
	switch(userChoose){
	case 1:
		// 用户查询自己的信息，无论是否查询到都因该返回上一级菜单
		employeeQueryBusiness(file_descriptor, login_model);
		break;
	case 2:

		break;
	case 3:
		break;
	case 0:
		break;
	default :
		break;
	}
}

/*
 * description : 普通员工信息查询业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		LoginModel * login_model
 * @param [out]: 
 * @return     : 0:正常退出  !0:错误退出
 * @Author     : xuyuanbing
 * @Other      : 
 */
int employeeQueryBusiness(int file_descriptor, LoginModel * login_model)
{
	EmployeeQueryModel query_model = {0};
	query_model.name  = login_model->name;
	query_model.empno = login_model->empno;
	EmployeeQueryResult query_Result = {0};

    int ret = sendEmployeeQueryRequest(&query_model, &query_result);
	if(ret){
		return ret;
	}
	printf("******************************************\n");
	printf("************** 姓名:%s\n", query_Result.name);
	printf("************** 性别:%s\n", query_result.sex);
	printf("************** 年龄:%d\n", query_result.age);
	printf("************** 工资:%d\n", query_result.salary);
	printf("************** 部门:%s\n", query_result.department);
	
	return ret;
}
	
/*
 * description : 发送信息查询请求, 接收查询结果
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		EmployeeQueryModel* query_model
 * @param [out]: 
 *	    EmployeeQueryResult *query_Result
 * @return     : 0:正常退出  !0:错误退出
 * @Author     : xuyuanbing
 * @Other      : 
 */
	int sendEmployeeQueryRequest(int file_descriptor, EmployeeQueryModel* query_model,
			EmployeeQueryResult *query_Result)
	{
		int ret = -1;
		RequestInfo req = {
			.type = EmployeeQuery,
			.size = sizeof(EmployeeQueryModel)
		};
		ResponseInfo res = {0};
		ret = request(file_descriptor,&req, sizeof(req), &query_model, sizeof(EmployeeQueryModel),
				&res, sizeof(res), query_Result, sizeof(EmployeeQueryResult));
		return ret;
	}

5.普通员工信息修改业务:::
int employeeModifyBusiness(int file_descriptor, LoginModel * login_model)
{

}
返回值: 0:正常退出  !0:错误退出
    5.1: 交互获取用户信息 用户名/密码/性别/年龄:::
/*
 * description : 交互获取用户信息 用户名/密码/性别/年龄
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * 		EmployeeModifyModel*model
 * @return     : 0:信息获取成功 !0:获取错误或用户取消操作
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeModifyModel(EmployeeModifyModel*model)
{
	char tmp [20] = {0};
	printf("请输入员工的姓名：");
	getDataFgets(tmp, sizeof(tmp));
	strncpy(model->name, tmp, sizeof(model->name) - 1);
	printf("请输入员工的密码：");
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	strncpy(create_model->pwd, tmp, sizeof(create_model->pwd) - 1);
	printf("请输入员工的性别："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	create_model.sex = (ucahr) atoi(tmp);
	printf("请输入员工的年龄："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	create_model.age = (uchar) atoi(tmp);

	return 0;
}
    5.2: 格式化信息修改数据, 发送信息修改请求, 接收处理结果:::
int sendEmployeeModifyRequest(int file_descriptor, EmployeeModifyModel*model){
	RequestInfo req = {
		.type = EmployeeModify,
		.size = sizeof(EmployeeModifyModel)
	};
	
	ResponseInfo res = {0};
	EmployeeQueryResult res_model = {0};
	request(file
}:
    参数: @model:用户信息
    返回值: 0:修改成功 !0:修改出错
    5.3: 解析处理结果
        5.3.1: 修改失败显示错误信息, Goto普通用户菜单
        5.3.2: 更新用户本地信息

6.普通员工退出业务:::
    int employee_quit_business(void);
    6.1: 二次确认退出(默认yes):::
        6.1.1: yes: Goto首页菜单
            6.1.1.1: 格式化退出请求信息, 发送退出请求:::
            int send_quit_request(struct QuitModel *model,  struct ResponseInfo*out);
           参数: @model:退出信息  @out:退出结果
           返回值: 0:成功 !0:出错
            6.1.1.2: 解析结果, 出错显示错误, 成功Goto首页菜单
        6.1.2: no: Goto普通用户菜单



/*
 * description : 登录业务
 * function    : 
 * @param [ in]: 
 * 		int fd, 
 * 		LoginModel *login_model
 * @param [out]: 
 * 		LoginResultModel *result_model
 * @return     : 
 *    0:登陆成功 !0: 登陆失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int loginBusiness(int fd, LoginModel *login_model, LoginResultModel *result_model)
{
	int ret = -1;
	getLoginModel(&login_model)
	ret = sendLoginRequest(fd, &login_model, &result_model)

	return ret;
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
	RequestInfo req = {
		.type = Login,
		.size = sizeof(LoginModel)
	};
	ResponseInfo res = {0};
	LoginResultModel res_model = {0};

	int ret = FuncException;
	ret = request(file_descriptor, &req, sizeof(RequestInfo), &model, sizeof(LoginModel),
			&res, sizeof(ResponseInfo), &res_model, sizeof(LoginModel));
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
	ret = send(file_descriptor, (void *)request_head, request_head_size,0);
	TRY_PERROR(ret == FuncException, "发送请求头");

	// 发送请求数据
	ret = send(file_descriptor, (void *)request_data, request_data_size,0);
	TRY_PERROR(ret == FuncException, "发送请求数据");
	
	// 接受反回头
	ret = recv(file_descriptor, (void *) response_head, response_head_size, 0);
	TRY_PERROR(ret == FuncException, "接收请求头数据");

	// 接受返回的数据
	ret = recv(file_descriptor, (void *) response_data, response_data_size, 0);
	TRY_PERROR(ret == FuncException, "接收请求数据");

	return FuncNormal;
}

