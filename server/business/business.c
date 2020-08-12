/************************************************
 * Name: business.c
 * Date: 2020-08-08
 * Description: 业务处理方法
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "business.h"
#include "store.h"


// 业务处理入口
int businessEntrance(int fd)
{
	// 接收请求数据
	RequestInfo  req_head = {0};
	void *       req_data = NULL;
	ResponseInfo res_head = {0};
	void *       res_data = NULL;
	int ret = receviesMessage(fd, &req_head, &req_data);
	if (ret < 0)
	{
		printf("[%d] 客户端已退出\n", fd);
		return FuncException;
	}

	// 校验登录信息
	int empno = -1;
	if (req_head.type != Login)
	{
		empno = checkLoginStateInfo(fd);
		if (empno<0) 
		{
			if (req_data) free(req_data);

			printf("[%d]:非法访问, 请登录后重试\n", fd);
			return FuncException;
		}
	}
	

	res_head.type   = req_head.type;
	res_head.result = Failed;
	switch(req_head.type)
	{
		case Login: 		
			printf("[%d]:登录系统\n", fd);
			ret = loginHandler(fd, &req_head, req_data, &res_head, &res_data); 		
			break;
		case Quit:	 		
			printf("[%d]:退出系统\n", fd);
			saveLogs(empno, "退出系统");
			ret = quitHandler(fd, &req_head, req_data, &res_head, &res_data);			
			break;
		case EmployeeQuery:	
			printf("[%d]:查询员工信息\n", fd);
			saveLogs(empno, "查询员工信息");
			ret = employeeQueryHandler(fd, &req_head, req_data, &res_head, &res_data);	
			break;
		case EmployeeModify:
			printf("[%d]:修改员工信息\n", fd);
			saveLogs(empno, "修改信息");
			ret = employeeModifyHandler(fd, &req_head, req_data, &res_head, &res_data);
			break;
		case EmployeeAdd:	
			printf("[%d]:创建新员工信息\n", fd);
			saveLogs(empno, "创建新员工");	
			ret = employeeAddHandler(fd, &req_head, req_data, &res_head, &res_data);	
			break;
		case EmployeeDelete:
			printf("[%d]:删除员工信息\n", fd);
			saveLogs(empno, "删除员工");
			ret = employeeDeleteHandler(fd, &req_head, req_data, &res_head, &res_data);
			break;
		case LogsQuery:		
			printf("[%d]:查询操作日志\n", fd);
			saveLogs(empno, "日志查询");
			ret = logsQueryHandler(fd, &req_head, req_data, &res_head, &res_data);		
			break;
		case SignIn:		
			printf("[%d]:员工签到\n", fd);
			saveLogs(empno, "员工签到");
			ret = signInHandler(fd, &req_head, req_data, &res_head, &res_data);		
			break;
		case SignInInfo:		
			printf("[%d]:查询签到信息\n", fd);
			saveLogs(empno, "查询签到信息");
			ret = signInInfoHandler(fd, &req_head, req_data, &res_head, &res_data);		
			break;
	}
	if (ret < 0)
	{
		if (req_data) free(req_data);
		return ret;
	}
	
	ret = responseMessage(fd, &res_head, res_data);
	if (req_data) free(req_data);
	if (res_data) free(res_data);
	return ret;
}

/*
 * function:    loginHandler
 * description: 登录请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int loginHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	LoginModel *model   = req_data;
	EmployeeInfo einfo  = {0};
	LoginResult result  = {0};

	// 校验登录信息
	int ret = checkEmployeeInfo(model->name, model->pwd, &einfo);
	if (ret < 0) return FuncError;
	if (einfo.empno == 0) goto LOGIN_FAILE_LABEL;

	// 创建登录信息
	ret = createLoginStateInfo(fd, &einfo);
	if (ret < 0) return FuncError;

	// 返回登录成功信息
	res_head->size   = sizeof(result);
	res_head->result = Success;
	strcpy(res_head->message, "登录成功");	
	
	result    = einfo;
	*res_data = malloc(sizeof(LoginResult));
	memcpy(*res_data, &result, sizeof(LoginResult));
	return FuncNormal;

LOGIN_FAILE_LABEL:
	strcpy(res_head->message, "用户名或密码错误");	
	return FuncNormal;
}

/*
 * function:    quitHandler
 * description: 退出请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int quitHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	QuitModel *model = req_data;

	// 删除登录信息
	int ret = deleteLoginStateInfo(model->empno);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 返回登录成功信息
	res_head->result = Success;
	strcpy(res_head->message, "退出成功");	
	return FuncNormal;

SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    employeeQueryHandler
 * description: 员工查询请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeQueryHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	EmployeeQueryModel *model            = req_data;
	EmployeeInfo result[QUERY_LOG_COUNT] = {0};

	// 查询员工信息
	int ret = queryEmployeeInfo(model->empno, model->name, result, QUERY_LOG_COUNT);
	if (ret < 0) goto SERVER_ERR_LABEL;


	// 返回查询成功信息
	res_head->result = Success;
	res_head->size   = sizeof(EmployeeInfo) * ret;
	strcpy(res_head->message, "查询成功");	

	*res_data = malloc(sizeof(EmployeeInfo) * ret);
	memcpy(*res_data, &result, sizeof(EmployeeInfo) * ret);

	return FuncNormal;
SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    employeeModifyHandler
 * description: 员工修改请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeModifyHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	EmployeeModifyModel *model = req_data;
	EmployeeInfo employee      = {0};

	int ret = modifyEmployeeInfo(model);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 返回查询成功信息
	res_head->result = Success;
	strcpy(res_head->message, "修改成功");	
	return FuncNormal;

SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    employeeAddHandler
 * description: 员工添加请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeAddHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	EmployeeCreateModel *model  = req_data;
	EmployeeCreateResult result = {0};
	EmployeeInfo employee       = *model;
	employee.role               = 1;
	uint empno                  = 0;	


	int ret = createEmployeeInfo(&employee, &empno);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 返回创建成功消息
	res_head->result = Success;
	res_head->size   = sizeof(result);
	strcpy(res_head->message, "创建成功");
	
	// 响应体
	result       = employee;
	result.empno = empno;
	*res_data = malloc(sizeof(result));
	memcpy(*res_data, &result, sizeof(result));
	return FuncNormal;

SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    employeeDeleteHandler
 * description: 员工删除请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeDeleteHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	EmployeeDeleteModel *model = req_data;

	int ret = deleteEmployeeInfo(model->empno);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 返回创建成功消息
	res_head->result = Success;
	strcpy(res_head->message, "删除成功");
	return FuncNormal;

SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    logsQueryHandler
 * description: 日志查询请求处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int logsQueryHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	LogQueryModel *model = req_data;
	LogQueryResult result[QUERY_LOG_COUNT] = {0};

	int ret = queryLogInfo(model->date, result);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 返回查询成功消息
	res_head->result = Success;
	res_head->size   = sizeof(LogQueryResult) * ret;
	strcpy(res_head->message, "查询成功");

	*res_data = malloc(res_head->size);
	memcpy(*res_data, result, res_head->size);
	return FuncNormal;
	
SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    signInHandler
 * description: 员工签到处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int signInHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	// 从登录信息取出员工号
	int empno = checkLoginStateInfo(fd);
	if (empno < 0) return FuncError;
	
	// 查询签到信息
	int signin_status;
	int ret = checkSigninInfo(empno, &signin_status);
	if (ret < 0) return FuncError;
	if (signin_status) goto Already_SignIn_Label;
	
	// 新增签到信息
	ret = createSigninInfo(empno);
	if (ret < 0) goto SERVER_ERR_LABEL;
	
	// 返回签到成功结果
	res_head->result = Success;
	strcpy(res_head->message, "签到成功");
	return FuncNormal;

Already_SignIn_Label:
	strcpy(res_head->message, "您已签到，无法重复签到");
	return FuncNormal;
SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

/*
 * function:    signInInfoHandler
 * description: 查询签到信息处理函数
 * @param [ in] 
 * 	 fd   : 客户端fd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int signInInfoHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data)
{
	// 从登录信息取出员工号
	int empno = checkLoginStateInfo(fd);
	if (empno < 0) return FuncError;

	// 查询员工本月签到次数
	uint signin_count = 0;
	int ret = querySigninCount(empno, &signin_count);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 查询员工信息
	EmployeeInfo employee = {0};
	ret = queryEmployeeInfo(empno, NULL, &employee, 1);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 格式化响应信息
	char message[200] = "";
	uint salary = employee.salary;
	sprintf(message, "您本月签到%d次，预计能够拿到%.2f元", signin_count, (double)salary*signin_count/30);
	
	// 返回签到成功结果
	res_head->result = Success;
	strcpy(res_head->message, message);
	return FuncNormal;
SERVER_ERR_LABEL:
	strcpy(res_head->message, "服务错误");	
	return FuncNormal;
}

