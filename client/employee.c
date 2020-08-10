/*******************************************************
 * File Name   : employee.c
 * Version     : V1.0
 * Author      : yuanbingxu
 * Organization: https://github.com/xuyuanbing
 * Create Time : 2020-08-08 23:57:04
 * Description : 
 *******************************************************/
#include "employee.h"

// 普通用户的业务
/*
 * description : 普通用户的业务
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int doEmployeeBusiness(int file_descriptor, LoginResultModel * login_model)
{
	int ret = 0; //在在菜单向循环，非0 则返回到登陆页面 
	int choose = 0;
	do
	{
		showEmployeeMenu();
		choose = getHomeMenuChoose();
		ret = gotoEmployeeChoose(file_descriptor, choose, login_model);
	} while (ret);

	return ret;
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
 * description : 解析菜单输入
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 0: 退出到登陆页面，!0:返回上一级 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int gotoEmployeeChoose(int file_descriptor, int userChoose, LoginResultModel * login_model)
{
	// 就三种情况， 1. 继续上一级服务， 2.退出服务
	int ret = -1;
	switch(userChoose){
	case 1:
		// 用户查询自己的信息，无论是否查询到都因该返回上一级菜单
		employeeQueryBusiness(file_descriptor, login_model);
		break;
	case 2:
		employeeModifyBusiness(file_descriptor, login_model);
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
int employeeQueryBusiness(int file_descriptor, LoginResultModel * login_model)
{/*{{{*/
	EmployeeQueryModel query_model = {0};
	strncpy(query_model.name, login_model->name, sizeof(query_model.name));
	query_model.empno = login_model->empno;
	EmployeeQueryResult query_result = {0};

    int ret = sendEmployeeQueryRequest(file_descriptor, &query_model, &query_result);
	if(ret){
		return ret;
	}
	printf("******************************************\n");
	printf("************** 姓名:%s\n", query_result.name);
	printf("************** 性别:%s\n", query_result.sex==Male?"男":"女");
	printf("************** 年龄:%d\n", query_result.age);
	printf("************** 工资:%d\n", query_result.salary);
	printf("************** 部门:%s\n", query_result.department);
	
	return ret;
}/*}}}*/

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
{/*{{{*/
	int ret = -1;
	RequestInfo req = {
		.type = EmployeeQuery,
		.size = sizeof(EmployeeQueryModel)
	};
	ResponseInfo res = {0};
	ret = request(file_descriptor,&req, sizeof(req), query_model, sizeof(EmployeeQueryModel),
			&res, sizeof(res), query_Result, sizeof(EmployeeQueryResult));
	return ret;
}/*}}}*/

/*
 * description : 普通员工信息修改业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		LoginModel * login_model
 * @param [out]: 
 * @return     : 0:修改成功 !0:修改出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int employeeModifyBusiness(int file_descriptor,  LoginResultModel* login_model)
{
	EmployeeModifyModel modify_model = {0};
	modify_model.empno = login_model->empno;	
	getEmployeeModifyModel(&modify_model);
	int ret = sendEmployeeModifyRequest(file_descriptor, &modify_model);
	return ret;
}

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
	strncpy(model->pwd, tmp, sizeof(model->pwd) - 1);
	printf("请输入员工的性别："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	model->sex = (uchar) atoi(tmp);
	printf("请输入员工的年龄："); 
	bzero(tmp, sizeof(tmp));
	getDataFgets(tmp, sizeof(tmp));
	model->age = (uchar) atoi(tmp);

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
int sendEmployeeModifyRequest(int file_descriptor, EmployeeModifyModel*model)
{
	RequestInfo req = {
		.type = EmployeeModify,
		.size = sizeof(EmployeeModifyModel)
	};
	
	ResponseInfo res = {0};
	EmployeeQueryResult res_model = {0};
	int ret = request(file_descriptor, &req, sizeof(req), model, sizeof(EmployeeModifyModel),
			&res, sizeof(res), &res_model, sizeof(res_model)); 
	return ret;
}


