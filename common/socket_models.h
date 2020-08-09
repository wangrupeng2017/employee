/************************************************
 * Name: socket_models.h
 * Date: 2020-08-08
 * Description: Socket通信结协议结构体
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/

#ifndef __SOCKET_MODELS_H__
#define __SOCKET_MODELS_H__

#include "common_base.h"


// 角色类型
typedef enum RoleType {
	EmployeeRole = 1,
	AdminRole    = 2,
} RoleType;

// 性别类型
typedef enum SexType {
	Male   = 1,
	Female = 2,
} SexType;

// 请求类型
typedef enum RequestType {
	Login          = 1,
	Quit           = 2,
	EmployeeQuery  = 3,
	EmployeeModify = 4,
	EmployeeAdd    = 5,
	EmployeeDelete = 6,
	LogsQuery      = 7,
	SignIn         = 8,
	SignInInfo     = 9,
} RequestType;

// 响应结果
typedef enum ResultStatus {
	Success = 0,
	Failed  = 1,
} ResultStatus;

// 请求头信息
typedef struct RequestInfo {
	RequestType type;
	uint        size;
} RequestInfo;
// 响应头信息
typedef struct ResponseInfo {
	RequestType  type;
	uint         size;
	ResultStatus result;
	char         message[RES_MESSAGE_SIZE];
} ResponseInfo;

// 登录系统请求/响应结构
typedef struct LoginModel {
	char name[EMPLOYEE_NAME_SIZE];
	char pwd[EMPLOYEE_PWD_SIZE];
} LoginModel;
typedef struct LoginResult {
	char name[EMPLOYEE_NAME_SIZE];
	uint empno;
	uchar role;
} LoginResultModel;

// 退出系统请求/响应结构
typedef struct QuitModel {/*{{{*/
	uint empno;
} QuitModel;/*}}}*/

// 查询员工请求/响应结构
typedef struct EmployeeQueryModel {/*{{{*/
	char name[EMPLOYEE_NAME_SIZE];
	uint empno;
	uint query_empno;
} EmployeeQueryModel;/*}}}*/
typedef struct EmployeeQueryResult {
	uint  empno;
	char  name[EMPLOYEE_NAME_SIZE];
	char  pwd[EMPLOYEE_PWD_SIZE];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[EMPLOYEE_DEPARTMENT_SIZE];
	uchar role;
} EmployeeQueryResult;

// 修改员工请求/响应结构
typedef struct EmployeeModifyModel {
	uint  empno;
	char  name[EMPLOYEE_NAME_SIZE];
	char  pwd[EMPLOYEE_PWD_SIZE];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[EMPLOYEE_NAME_SIZE];
} EmployeeModifyModel; 

// 创建新员工请求/响应结构
typedef struct EmpldoyeeCreateModel {/*{{{*/
	char  name[EMPLOYEE_NAME_SIZE];
	char  pwd[EMPLOYEE_PWD_SIZE];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[EMPLOYEE_DEPARTMENT_SIZE];
} EmployeeCreateModel;/*}}}*/
typedef struct EmployeeCreateResult {/*{{{*/
	uint  empno;
	char  name[EMPLOYEE_NAME_SIZE];
	char  pwd[EMPLOYEE_PWD_SIZE];
} EmployeeCreateResult;/*}}}*/

// 删除员工请求/响应结构
typedef struct EmployeeDeleteModel {
	uint empno;
} EmployeeDeleteModel;

// 日志查询请求/响应结构
typedef struct LogQueryModel {
	char date[LOG_TIME_SIZE];
} LogQueryModel;
typedef struct LogQueryResult {
	uint empno;
	char describe[LOG_DESCRIBE_SIZE];
	char date[LOG_TIME_SIZE];
} LogQueryResult;


#endif //__SOCKET_MODELS_H__
