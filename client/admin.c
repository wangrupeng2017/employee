/*******************************************************
 * File Name   : admin.c
 * Version     : V1.0
 * Author      : yuanbingxu
 * Organization: https://github.com/xuyuanbing
 * Create Time : 2020-08-09 00:09:23
 * Description : 
 *******************************************************/
#include "admin.h"

// 管理员业务:
/*
 * description : 打印管理员菜单
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
 void show_admin_menu(void)
{
	printf("********************  请选择所需的服务  ********************\n");
	printf("**************  1:添加用户  2:删除用户 3:信息查询 **********\n");
	printf("**************  4:修改用户  5:查询操作 0:退出 **************\n");
	printf("************************************************************\n");
}

// 解析菜单输入
void gotoAdminChoose(int userChoose)
{
	switch(userChoose){
	case 1:
		break;
	case 2:

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

/*
 * description : 接收菜单输入
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : int 选择的菜单序号
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getAdminMenuChoose(void)
{
	char choose = -1;
	printf("请输入您需要的服务：");
	while(!(choose = getchar()));
	
	return choose - '0';
}

// TODO
#if 0
3.解析菜单输入
    3.1: 输入错误, 提示信息, Goto管理员菜单
    3.2: 选择添加用户, Goto添加用户业务
    3.3: 选择删除用户, Goto删除用户业务
    3.4: 选择修改用户, Goto修改用户业务
    3.5: 选择查询用户, Goto查询用户业务
    3.6: 选择查询操作日志, Goto操作日志查询业务
    3.7: 选择退出, Goto退出业务(return 0)
#endif

4.添加用户业务:::
int adminAddBusiness(void);


static void getDataFgets(char * data, size_t size)
{
	fgets(data, size, stdin);
	if(strlen(data) < size -1){
		data[strlen(word) -1] = '\0'; // fgets 自动在最后添加一个 '\n'
	}
}

/*
 * description : 交互获取用户信息 员工号(自动生成)/用户名/密码(初始:123)/性别/年龄/部门/工资
 * function    : getAdminAddModel
 * @param [ in]: 
 *     EmployeeCreateModel * create_model 请求信息
 * @param [out]: 
 * @return     : 
 *   返回值: 0:信息获取成功 !0:获取错误或用户取消操作
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getAdminAddModel(EmployeeCreateModel * create_model)
{
	// TODO create_model 中的 token 没有处理
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
	printf("请输入员工的工资："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	create_model.salary = (uint) atoi(tmp);
	printf("请输入员工的部门："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	strncpy(create_model->department, tmp, sizeof(create_model->department) - 1);
	return 0;
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

int request(int file_descriptor, void * request_data, size_t request_data_size , size_t response_data_size, void * response_data)
{
	// 输入参的基本校验 忽 TODO 
	int ret = -1;
	do
	{
		ret = send(file_descriptor, (void *)request_data, request_data_size, 0);
	} while (ret < 0 && EINTR == errno);

	do
	{
		ret = recv(file_descriptor, (void *) response_data, response_data_size, 0);
	} while (ret < 0 && EINTR == errno);
	if( ret < 0 ){
		perror(" send login model");
		return ret;
	}

	return 0;
}

/*
 * description : 格式化添加用户数据, 发送添加用户请求, 接收处理结果
 * function    : 
 * @param [ in]: 
 * 		EmployeeCreateModel*create_model 请求信息添加新员工
 * 		int file_descriptor
 * @param [out]: 
 * 		EmployeeCreateResult *result 请求添加用户的结果
 * @return     : 
 * 		0:请求成功 !0:请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendAdminAddRequest(int file_descriptor, EmployeeCreateModel *create_model, EmployeeCreateResult *result)
{
	// 输入参的基本校验 忽 TODO 
	int ret = -1;
	ret = request(file_descriptor, (void *)create_data, sizeof(EmployeeCreateModel),
			sizeof(EmployeeCreateResult), (void *)result);
	return ret ;

// 解析处理结果
// 添加失败显示错误信息, Goto管理员菜单
// 添加成功显示成功信息, Goto管理员菜单

}


/*
 * description : 交互获取用户信息(用户名)
 * function    : 
 * @param [ in]: 
 * 		char * name 用户名
 * 		size_t size
 * @param [out]: 
 * @return     : 
 * 		0:请求成功 !0:请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeName(char * name, size_t size)
{
	printf("请输入员工的姓名："); 
	bzero(name, size);
	getDataFgets(name, size);
	return 0;
}

/*
 * description : 交互获取用户选择的用户员工号
 * function    : 
 * @param [ in]: 
 * 		sdtio
 * @param [out]: 
 *   	int *employee_number 员工号
 * @return     : 
 *   0:信息获取成功 !0:获取错误或用户取消操作
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeNumber(uint *employee_number)
{
	char tmp [32] = [0];
	printf("请输入员工的工号："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(temp, sizeof(tmp));
	*employee_number = (uint)atoi(tmp);
	return 0
}

/*
 * description : 发送信息查询请求, 接收查询结果
 * function    : 
 * @param [ in]: 
 *  	int file_descriptor
 *  	EmployeeQueryModel * query_model 查询信息
 * @param [out]: 
 *  	 EmployeeQueryResult *query_Result 查询结果
 * @return     : 
 *  	0:发送请求成功 !0:发送请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendEmployeeQueryRequest(int file_descriptor, EmployeeQueryModel * query_model, EmployeeQueryResult *query_Result)
{

	// 输入参的基本校验 忽 TODO 
	int ret = -1;
	ret = request(file_descriptor, (void *)query_model, sizeof(EmployeeQueryModel),
			sizeof(EmployeeQueryResult), (void *)query_result);
	return ret ;
}

        5.4: 格式化删除用户数据, 发送删除用户请求, 接收查询结果:::
/*
 * description : 发送删除用户请求, 接收查询结果
 * function    : 
 * @param [ in]: 
 *  	int file_descriptor
 *  	EmployeeDeleteModel *delete_model 删除信息
 * @param [out]: 
 * 		ResponseInfo *result 请求结果
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendEmployeeDeleteRequest (int file_descriptor, EmployeeDeleteModel *delete_model,
		ResponseInfo *result)
{
	// 输入参的基本校验 忽 TODO 
	int ret = -1;
	ret = request(file_descriptor, (void *)delete_model, sizeof(EmployeeDeleteModel),
			sizeof(ResponseInfo), (void *)result);
	return ret ;
// 解析处理结果
// 删除失败显示错误信息, Goto管理员菜单
// 删除成功显示成功信息, Goto管理员菜单
}

5.删除用户业务:::
int adminDeleteBusiness(void)
{
// TODO int connectServer(char *ip, int port)
	int file_descriptor = -1;
  file_descriptor = connectServer(char *ip, int port);

	EmployeeQueryModel query_model;
	EmployeeQueryResult query_Result;
	EmployeeDeleteModel delete_model;
	ResponseInfo result;
	// TODO 这里所以的返回直我还每有测试过
	getEmployeeName(query_model.name, sizeof(query_model))
	getEmployeeNumber(&query_model.empno)
	sendEmployeeQueryRequest(file_descriptor, &query_model, &query_Result);
	sendEmployeeDeleteRequest(file_descriptor, &delete_model, &result);
	return 0;
}



6.修改用户业务:::
int adminModifyBusiness(void)
{
// TODO int connectServer(char *ip, int port)
	int file_descriptor = -1;
  file_descriptor = connectServer(char *ip, int port);
  
	EmployeeQueryModel query_model;
	EmployeeQueryResult query_Result;
	EmployeeModifyModel modify_model;
	ResponseInfo result;
	// TODO 这里所以的返回直我还每有测试过
	// 
	getEmployeeName(query_model.name, sizeof(query_model))
	sendEmployeeQueryRequest(file_descriptor, &query_model, &query_Result);
	// TODO ====== 这里需要 打印根据用户名查询到的所以数据供用户选择
	//
	//
	//
	getEmployeeNumber(&query_model.empno)
	// TODO 这个方法不用写了， 用 之前的方法
    	
    //  交互获取用户信息 用户名/密码/性别/年龄/部门/工资
	//  TODO 返回直没有判断
    getEmployeeNewinfo(&modify_model);

    6.5: 格式化信息修改数据, 发送信息修改请求, 接收处理结果:::
    EmployeeQueryResult modify_result;
    sendAdminModifyRequest(file_descriptor, &modify_model, struct EmployeeQueryResult *out);
    参数: @model:查询信息 @out:查询结果
    返回值: 0:发送请求成功 !0:发送请求出错
    6.6: 解析处理结果:::
        6.6.1: 修改失败显示错误信息, Goto管理员菜单
        6.6.2: 修改失败显示成功信息, Goto管理员菜单




/*
 * description : 格式化信息修改数据, 发送信息修改请求, 接收处理结果
 * function    : 
 * @param [ in]: 
 *  	int file_descriptor
 *  	EmployeeModifyModel*modify_model 修改的信息
 * @param [out]: 
 *  	EmployeeQueryResult *modiy_result 发送请求成功后返回的数据
 * @return     : 
 * 		0:发送请求成功 !0:发送请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendAdminModifyRequest(int file_descriptor, EmployeeModifyModel*modify_model, 
		EmployeeQueryResult *modify_result)
{
 	// 输入参的基本校验 忽 TODO 
	int ret = -1;
	ret = request(file_descriptor, (void *)modify_model, sizeof(EmployeeModifyModel),
			sizeof(EmployeeQueryResult), (void *)modify_result);
	return ret ;
// 解析处理结果:::
// 修改失败显示错误信息, Goto管理员菜单
// 修改失败显示成功信息, Goto管理员菜单

}

/*
 * description : 交互获取用户信息 用户名/密码/性别/年龄/部门/工资
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 *  	EmployeeModifyModel *modif_model 新的员工信息
 * @return     : 
 * 		0:信息获取成功 !0:获取错误或用户取消操作	
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeNewinfo(EmployeeModifyModel *modif_model)
{
	// TODO create_model 中的 token 没有处理
	char tmp [20] = {0};
	int choose  = 0;
	printf("********************  请选择要修改的服务  *********************\n");
	printf("**************  1:用户名   2:用户密码 3:用户性别 **************\n");
	printf("**************  4:用户年龄 5:用户工资 6:用户部门 **************\n");
	printf("*************************  0:退出 *****************************\n");
	printf("***************************************************************\n");
	// TODO 这个方法是在 client.c
	choose = getHomeMenuChoose();
	switch( choose ){
	case 1:
		printf("请输入员工的姓名：");
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		strncpy(modify_model->name, tmp, sizeof(modiyf_model->name) - 1);
		break;
	case 2:
		printf("请输入员工的密码：");
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		strncpy(modify_model->pwd, tmp, sizeof(modely_model->pwd) - 1);
		break;
	case 3:
		printf("请输入员工的性别："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		modify_model.sex = (ucahr) atoi(tmp);
		break;
	case 4:
		printf("请输入员工的年龄："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		modify_model.age = (uchar) atoi(tmp);
		break;
	case 5:
		printf("请输入员工的工资："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		modify_model.salary = (uint) atoi(tmp);
		break;
	case 6:
		printf("请输入员工的部门："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		strncpy(modify_model->department, tmp, sizeof(modify_model->department) - 1);
		break;
	case 0:
		return 1;
		break;
	default:
		break;
	}
	return 0;
}













	
7.查询用户业务:::
int admin_query_business(void);
    7.1: 交互获取用户信息(用户名):::
    int get_employee_name(char name[20]);
    参数: @name:用户名
    返回值: 0:信息获取成功 !0:获取错误或用户取消操作
    7.2: 格式化信息查询数据, 发送信息查询请求, 接收查询结果:::
    int send_admin_query_request(struct EmployeeQueryModel*model, struct EmployeeQueryResult *out);
    参数: @model:查询信息 @out:查询结果
    返回值: 0:发送请求成功 !0:发送请求出错
    7.3: 打印员工信息, Goto管理员菜单

8.日志查询业务:::
int admin_query_logs_business(void);
    8.1: 交互获取查询日期(默认填充当天日期):::
    int get_log_date(char date[20]);
    参数: @date:时间
    返回值: 0:信息获取成功 !0:获取错误或用户取消操作
    8.2: 格式化日志查询数据, 发送日志查询请求, 接收查询结果:::
    int send_admin_query_logs_request(struct OperationLogModel*model, struct OperationLogResult*out);
    参数: @model:查询信息 @out:查询结果
    返回值: 0:发送请求成功 !0:发送请求出错
    8.3: 打印日志信息, Goto管理员菜单

9.普通员工退出业务:::
int admin_quit_business(void);
    9.1: 二次确认退出(默认yes):::
        9.1.1: yes: Goto首页菜单
            9.1.1.1: 格式化退出请求信息, 发送退出请求:::
            int send_quit_request(struct QuitModel *model,  struct ResponseInfo*out);
           参数: @model:退出信息  @out:退出结果
           返回值: 0:成功 !0:出错
            9.1.1.2: 解析结果, 出错显示错误, 成功Goto首页菜单
        9.1.2: no: Goto普通用户菜单
