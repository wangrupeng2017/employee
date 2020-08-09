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
#include "mysqlite3.h"


/*
 * function:    saveLogs
 * description: 记录日志
 * @param [ in] 
 * 	 empno    : 操作者员工号
 * 	 message  : 日志内容
 */
void saveLogs(uint empno, char message[50])
{
	LogInfo info = {0};
	info.empno = empno;
	info.time  = time(NULL);
	strcpy(info.description, message);

	createLogInfo(&info);
}


/*
 * function:    loginHandler
 * description: 登录请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int loginHandler(int sockfd, RequestInfo *info)
{/*{{{*/
	LoginModel model   = {0};
	EmployeeInfo einfo = {0};
	LoginResultModel result = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};

	// 接受登录数据
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	// 校验登录信息
	ret = checkEmployeeInfo(model.name, model.pwd, &einfo);
	if (ret < 0) return FuncError;
	if (einfo.empno == 0) goto LOGIN_FAILE_LABEL;

	// 创建登录信息
	ret = createLoginStateInfo(&einfo);
	if (ret < 0) return FuncError;

	// 记录操作日志
	saveLogs(einfo.empno, "登录系统");

	// 返回登录成功信息
	res.size   = sizeof(result);
	res.result = Success;
	strcpy(res.message, "登录成功");	
	
	result.empno = einfo.empno;
	result.role  = einfo.role;
	strcpy(result.name, einfo.name);
	return responseMessage(sockfd, &res, &result);

LOGIN_FAILE_LABEL:
	strcpy(res.message, "用户名或密码错误");	
	ret = responseMessage(sockfd, &res, NULL); 
	return ret;
}/*}}}*/

/*
 * function:    quitHandler
 * description: 退出请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int quitHandler(int sockfd, RequestInfo *info)
{/*{{{*/
	QuitModel model = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};

	// 接收退出消息
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	// 删除登录信息
	ret = deleteLoginStateInfo(model.empno);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 记录日志
	saveLogs(model.empno, "退出系统");

	// 返回登录成功信息
	res.result = Success;
	strcpy(res.message, "退出成功");	
	return responseMessage(sockfd, &res, NULL);

SERVER_ERR_LABEL:
	strcpy(res.message, "服务错误");	
	return responseMessage(sockfd, &res, NULL); 
}/*}}}*/

/*
 * function:    employeeQueryHandler
 * description: 员工查询请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeQueryHandler(int sockfd, RequestInfo *info)
{/*{{{*/
	EmployeeQueryModel model = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};
	EmployeeInfo result[QUERY_LOG_COUNT] = {0};

	// 接收查询消息
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	// 查询员工信息
	ret = queryEmployeeInfo(model.empno, model.name, result);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 记录日志
	saveLogs(model.empno, "查询员工信息");

	// 返回查询成功信息
	res.result = Success;
	res.size   = sizeof(EmployeeInfo) * ret;
	strcpy(res.message, "查询成功");	
	return responseMessage(sockfd, &res, result);

SERVER_ERR_LABEL:
	// 返回服务错误
	strcpy(res.message, "服务错误");	
	return responseMessage(sockfd, &res, NULL); 
}/*}}}*/

/*
 * function:    employeeModifyHandler
 * description: 员工修改请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeModifyHandler(int sockfd, RequestInfo *info)
{/*{{{*/
	EmployeeModifyModel model = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};
	EmployeeInfo employee = {0};

	// 接收修改消息
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	employee.empno  = model.empno;
	employee.sex    = model.sex;
	employee.age    = model.age;
	employee.salary = model.salary;
	strcpy(employee.name, model.name);
	strcpy(employee.pwd, model.pwd);
	strcpy(employee.department, model.department);
	ret = modifyEmployeeInfo(&employee);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 记录日志
	saveLogs(model.empno, "修改信息");

	// 返回查询成功信息
	res.result = Success;
	strcpy(res.message, "修改成功");	
	return responseMessage(sockfd, &res, NULL);

SERVER_ERR_LABEL:
	strcpy(res.message, "服务错误");	
	return responseMessage(sockfd, &res, NULL); 
}/*}}}*/

/*
 * function:    employeeAddHandler
 * description: 员工添加请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeAddHandler(int sockfd, RequestInfo *info)
{/*{{{*/
	EmployeeCreateModel model = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};
	EmployeeCreateResult result = {0};
	EmployeeInfo employee = {0};
	uint empno = 0;	

	// 接收创建消息
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	employee.sex    = model.sex;
	employee.age    = model.age;
	employee.salary = model.salary;
	strcpy(employee.name, model.name);
	strcpy(employee.pwd, model.pwd);
	strcpy(employee.department, model.department);
	ret = createEmployeeInfo(&employee, &empno);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 记录日志
	saveLogs(empno, "创建新员工");	

	// 返回创建成功消息
	res.result = Success;
	res.size   = sizeof(result);
	strcpy(res.message, "创建成功");
	
	result.empno = empno;
	strcpy(result.name, model.name);
	strcpy(result.pwd, model.pwd);
	return responseMessage(sockfd, &res, &result);

SERVER_ERR_LABEL:
	strcpy(res.message, "服务错误");	
	return responseMessage(sockfd, &res, NULL); 
}/*}}}*/

/*
 * function:    employeeDeleteHandler
 * description: 员工删除请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int employeeDeleteHandler(int sockfd, RequestInfo *info)
{/*{{{*/
	EmployeeDeleteModel model = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};

	// 接收删除消息
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	ret = deleteEmployeeInfo(model.empno);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 记录日志
	saveLogs(model.empno, "删除员工");

	// 返回创建成功消息
	res.result = Success;
	strcpy(res.message, "删除成功");
	return responseMessage(sockfd, &res, NULL);

SERVER_ERR_LABEL:
	// 返回服务错误
	strcpy(res.message, "服务错误");	
	return responseMessage(sockfd, &res, NULL); 
}/*}}}*/

/*
 * function:    logsQueryHandler
 * description: 日志查询请求处理函数
 * @param [ in] 
 * 	 sockfd   : 客户端sockfd
 * 	 info     : 请求头信息
 * @return      0:处理成功 !0:处理出错
 */
int logsQueryHandler(int sockfd, RequestInfo *info)
{
	LogQueryModel model = {0};
	ResponseInfo res = {
		.type    = info->type,
		.size    = 0,
		.result  = Failed,
		.message = ""
	};
	LogQueryResult result[QUERY_LOG_COUNT] = {0};

	// 接收查询日志消息
	int ret = recv(sockfd, &model, sizeof(model), 0);
	TRY_ERROR(ret<=0, "recv() error", return FuncError);

	ret = queryLogInfo(model.date, result);
	if (ret < 0) goto SERVER_ERR_LABEL;

	// 返回查询成功消息
	res.result = Success;
	res.size   = sizeof(LogQueryResult) * ret;
	strcpy(res.message, "查询成功");
	return responseMessage(sockfd, &res, result);


SERVER_ERR_LABEL:
	// 返回服务错误
	strcpy(res.message, "服务错误");	
	return responseMessage(sockfd, &res, NULL); 
}


