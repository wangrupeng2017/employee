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

typedef unsigned char uchar;
typedef unsigned int  uint;

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
	char         message[20];
} ResponseInfo;

// 登录系统请求/响应结构
typedef struct LoginModel {
	char name[20];
	char pwd[20];
} LoginModel;
typedef struct LoginResult {
	char token[20];
	char name[20];
	uint empno;
} LoginResultModel;

// 退出系统请求/响应结构
typedef struct QuitModel {
	char token[20];
} QuitModel;

// 查询员工请求/响应结构
typedef struct EmployeeQueryModel {
	char token[20];
	char name[20];
	uint empno;
} EmployeeQueryModel;
typedef struct EmployeeQueryResult {
	uint  empno;
	char  name[20];
	char  pwd[20];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[20];
} EmployeeQueryResult;

// 修改员工请求/响应结构
typedef struct EmployeeModifyModel {
	char  token[20];
	uint  empno;
	char  name[20];
	char  pwd[20];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[20];
} EmployeeModifyModel; 

// 创建新员工请求/响应结构
typedef struct EmployeeCreateModel {
	char  token[20];
	char  name[20];
	char  pwd[20];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[20];
} EmployeeCreateModel;
typedef struct EmployeeCreateResult {
	uint  empno;
	char  name[20];
	char  pwd[20];
} EmployeeCreateResult;

// 删除员工请求/响应结构
typedef struct EmployeeDeleteModel {
	char token[20];
	uint empno;
} EmployeeDeleteModel;

// 日志查询请求/响应结构
typedef struct LogQueryModel {
	char token[20];
	char date[20];
} LogQueryModel;
typedef struct LogQueryResult {
	char name[20];
	char describe[50];
	char date[20];
} LogQueryResult;


#endif //__SOCKET_MODELS_H__
