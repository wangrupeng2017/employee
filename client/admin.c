/*******************************************************
 * File Name   : admin.c
 * Version     : V1.0
 * Author      : yuanbingxu
 * Organization: https://github.com/xuyuanbing
 * Create Time : 2020-08-09 00:09:23
 * Description : 
 *******************************************************/
#include "admin.h"
#include "other.h"

/*
 * description : 管理员业务
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int doAdminBusiness(int file_descriptor, LoginResultModel * login_model)
{
	int ret = 0; //在在菜单向循环，非0 则返回到登陆页面 
	int choose = 0;
	do
	{
		showAdminMenu();
		ret = gotoAdminChoose(file_descriptor, getAdminMenuChoose(), login_model);
	} while (ret);

	return ret;
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
	printf("**************  1:添加用户  2:删除用户 3:信息查询 **********\n");
	printf("**************  4:修改用户  5:查询操作 0:退出 **************\n");
	printf("************************************************************\n");
}

// 解析菜单输入
int gotoAdminChoose(int file_descriptor, int userChoose, LoginResultModel * login_model)
{
	int ret = -1;
	switch(userChoose){
	case 1:
		adminAddBusiness(file_descriptor, login_model);
		break;
	case 2:
		adminDeleteBusiness(file_descriptor, login_model);
		break;
	case 3:
		adminQueryBusiness(file_descriptor, login_model);
		break;
	case 4:
		adminModifyBusiness(file_descriptor, login_model);
		break;
	case 5:
		adminQueryLogsBusiness(file_descriptor, login_model);
		break;
	case 0:
	ret = employeeQuitBusiness(file_descriptor, login_model);
		break;
	default :
		break;
	}

	return ret;
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
	while( getchar() != '\n');
	
	return choose - '0';
}


/*
 * description : 添加用户业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		loginresultmodel * login_model
 * @param [out]: 
 * @return     : 返回值: 0:新建用户成功 !0:新建用户失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int adminAddBusiness(int file_descriptor, LoginResultModel * login_model)
{
	int ret = -1; 
	EmployeeCreateModel create_model = {0};
	EmployeeCreateResult create_result = {0};
	getAdminAddModel(&create_model);

	ret = sendAdminAddRequest(file_descriptor, &create_model, &create_result);
	 
	return ret;
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
{ /*{{{*/

	// TODO create_model 中的 token 没有处理
	char tmp [20] = {0};
	printf("请输入员工的姓名：");
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	strncpy(create_model->name, tmp, sizeof(create_model->name) - 1);
	printf("请输入员工的密码：");
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	strncpy(create_model->pwd, tmp, sizeof(create_model->pwd) - 1);
	printf("请输入员工的性别："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));

	if(strncmp(tmp, STR_MALE, sizeof(STR_MALE))){
		create_model->sex = Female;
	}else{
		create_model->sex = Male;
	}

	printf("请输入员工的年龄："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	create_model->age = (uchar) atoi(tmp);
	printf("请输入员工的工资："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	create_model->salary = (uint) atoi(tmp);
	printf("请输入员工的部门："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	strncpy(create_model->department, tmp, sizeof(create_model->department) - 1);
	return 0;
}/*}}}*/


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
	RequestInfo req = {
		.type = EmployeeAdd,
		.size = sizeof(EmployeeCreateModel)
	};
	ResponseInfo res = {0};
	
	ret = request(file_descriptor, &req, sizeof(req), create_model, sizeof(EmployeeCreateModel),
			 &res, sizeof(res), result, sizeof(EmployeeCreateResult));

	if( ret ){
		printf("%s\n", res.message);
	}else{
		printf("******************************************\n");
		printf("************** 姓名:%s\n", result->name);
		printf("************** 性别:%s\n", result->pwd);
		printf("************** 年龄:%d\n\n", result->empno);
	}

	return ret ;
}

/*
 * description : 删除用户业务
 * function    : 
 * @param [ in]: int file_descriptor 
 * 		LoginResultModel * login_model
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int adminDeleteBusiness(int file_descriptor, LoginResultModel * login_model)
{
	EmployeeDeleteModel delete_model = {0};

	getEmployeeNumber(&delete_model.empno);
	printf("%s:%s:%d\n", __FILE__, __func__, __LINE__);
	return sendEmployeeDeleteRequest(file_descriptor, &delete_model);
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
void getEmployeeNumber(uint *employee_number)
{
	char tmp [32] = {0};
	printf("请输入员工的工号："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	*employee_number = (uint)atoi(tmp);
}

/*
 * description : 发送删除用户请求, 接收查询结果
 * function    : 
 * @param [ in]: 
 *  	int file_descriptor
 *  	EmployeeDeleteModel *delete_model 删除信息
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendEmployeeDeleteRequest (int file_descriptor, EmployeeDeleteModel *delete_model)
{
	RequestInfo req = {
		.type = EmployeeDelete,
		.size = sizeof(EmployeeDeleteModel)
	};
	
	ResponseInfo res = {0};
	EmployeeCreateResult res_model = {0};
	// 输入参的基本校验 忽 TODO 
	int ret = -1;
	ret = request(file_descriptor, &req, sizeof(req), delete_model, sizeof(EmployeeDeleteModel),
			&res, sizeof(ResponseInfo), &res_model, sizeof(res_model));
	if( ret ){
		printf("%s\n", res.message);
	}
	return ret ;
}





// 6.修改用户业务:::
int adminModifyBusiness(int file_descriptor, LoginResultModel *login_model)
{
	EmployeeQueryModel  query_model = {0};
	getEmployeeName(query_model.name, sizeof(query_model.name));
	int ret = sendAdminEmployeeQueryRequest(file_descriptor, &query_model);
	if( ret ){
		return ret;
	}

	EmployeeModifyModel modify_model = {0};
	getEmployeeNumber(&modify_model.empno);
	getEmployeeNewinfo(&modify_model);
	ret = sendAdminModifyRequest(file_descriptor, &modify_model);
	return ret ;
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

int sendAdminEmployeeQueryRequest(int file_descriptor, EmployeeQueryModel *req_data)
{
	int ret = 0;
	RequestInfo         req_head = {0};
	ResponseInfo        res_head = {0};
	EmployeeQueryResult res_data[20] = {0};
	req_head.type = EmployeeQuery;
	req_head.size = sizeof(req_data);

	// 发送查询请求
	ret = request(file_descriptor, 
			&req_head, sizeof(req_head), req_data, sizeof(EmployeeQueryModel),
			&res_head, sizeof(res_head), res_data, sizeof(res_data));

	// 查询结果员工数量
	int employee_count = res_head.size/sizeof(EmployeeQueryResult);
	if (employee_count == 0){
		printf("未查询到指定条件的员工信息, 请换个条件试一试\n");
		return FuncException;
	}

	// 打印员工信息
	int i = 0;
	for (i=0; i<employee_count; i++)
	{
		EmployeeQueryResult e = res_data[i];
		printf("[%d]\t%d\t%s\t%s\t%s\t%d\t%d\t%s\n", 
				i+1, e.empno, e.name, e.pwd, e.sex==Male?"男":"女",
				e.age, e.salary, e.department);
	}

	return FuncNormal;
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
int getEmployeeNewinfo(EmployeeModifyModel *modify_model)
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
		strncpy(modify_model->name, tmp, sizeof(modify_model->name) - 1);
		break;
	case 2:
		printf("请输入员工的密码：");
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		strncpy(modify_model->pwd, tmp, sizeof(modify_model->pwd) - 1);
		break;
	case 3:
		printf("请输入员工的性别："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		modify_model->sex = (uchar) atoi(tmp);
		break;
	case 4:
		printf("请输入员工的年龄："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		modify_model->age = (uchar) atoi(tmp);
		break;
	case 5:
		printf("请输入员工的工资："); 
		bzero(tmp, sizeof(tmp));
		getDataFgets(tmp, sizeof(tmp));
		modify_model->salary = (uint) atoi(tmp);
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


/*
 * description : 格式化信息修改数据, 发送信息修改请求, 接收处理结果
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		EmployeeModifyModel*model
 * @param [out]: 
 * @return     : 0:修改成功 !0:修改出错
 * @Author     : xuyuanbing
 * @Other      : 
 */

int sendAdminModifyRequest(int file_descriptor, EmployeeModifyModel *modify_model)
{
	RequestInfo req = {
		.type = EmployeeModify,
		.size = sizeof(EmployeeModifyModel)
	};
	
	ResponseInfo res = {0};
	EmployeeQueryResult res_model = {0};
	int ret = request(file_descriptor, &req, sizeof(req), modify_model, sizeof(EmployeeModifyModel),
			&res, sizeof(res), &res_model, sizeof(res_model)); 
	if( ret ){
		printf("%s\n", res.message);
	}

	return ret;
}

