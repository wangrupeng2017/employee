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
#include "store_models.h"

typedef char(resmessage_t)[200];

// 角色类型
typedef enum RoleType {
	EmployeeRole = 1,   //普通员工
	AdminRole    = 2,   //管理员
} RoleType;

// 性别类型
typedef enum SexType {
	Male   = 1,         //男
	Female = 2,         //女
} SexType;

// 请求类型
typedef enum RequestType {
	Login          = 1, //登录
	Quit           = 2, //退出
	EmployeeQuery  = 3, //查询员工
	EmployeeModify = 4, //修改员工
	EmployeeAdd    = 5, //添加员工
	EmployeeDelete = 6, //删除员工
	LogsQuery      = 7, //日志查询
	SignIn         = 8, //签到
	SignInInfo     = 9, //签到查询
} RequestType;

// 响应结果
typedef enum ResultStatus {
	Success = 0,        //处理成功
	Failed  = 1,        //处理出错
} ResultStatus;

// 请求头信息
typedef struct RequestInfo {
	RequestType type;   //请求类型
	uint        size;   //请求体大小
} RequestInfo;

// 响应头信息
typedef struct ResponseInfo {
	RequestType  type;    //响应类型
	uint         size;    //响应体大小
	ResultStatus result;  //处理结果
	resmessage_t message; //结果描述
} ResponseInfo;

// 登录系统请求体
typedef struct LoginModel {
	ename_t name;         //用户名 
	epwd_t  pwd;          //密码
} LoginModel;

// 登录系统响应体
typedef EmployeeInfo LoginResult;

// 退出系统请求/响应结构
typedef struct QuitModel {
	uint empno;           //员工号
} QuitModel;

// 查询员工请求/响应结构
typedef struct EmployeeQueryModel {
	ename_t name;         //用户名
	uint empno;           //员工号
} EmployeeQueryModel;
// 查询员工响应体
typedef EmployeeInfo EmployeeQueryResult;

// 修改员工请求
typedef EmployeeInfo EmployeeModifyModel;

// 创建新员工请求体
typedef EmployeeInfo EmployeeCreateModel;
// 创建新员工响应体
typedef EmployeeInfo EmployeeCreateResult; 

// 删除员工请求体
typedef struct EmployeeDeleteModel {
	uint empno;           //员工号
} EmployeeDeleteModel;

// 日志查询请求体
typedef struct LogQueryModel {
	stime_t date;         //员工号
} LogQueryModel;
// 日志查询响应体
typedef struct LogQueryResult {
	uint empno;           //员工号
	opdesc_t describe;    //操作描述
	stime_t date;         //操作时间
} LogQueryResult;


#endif //__SOCKET_MODELS_H__
