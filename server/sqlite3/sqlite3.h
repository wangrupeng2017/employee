/************************************************
 * Name: sqlite3.h
 * Date: 2020-08-08
 * Description: sqlite3基础封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/

#ifndef __SQLITE3_H__
#define __SQLITE3_H__

#include "../../common/common_base.h"

// 员工信息结构体
typedef struct EmployeeInfo {
	uint  empno;
	char  name[EMPLOYEE_NAME_SIZE];
	char  pwd[EMPLOYEE_PWD_SIZE];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[EMPLOYEE_DEPARTMENT_SIZE];
} EmployeeInfo;

// 日志信息结构体
typedef struct LogInfo {
	uint empno;
	char description[LOG_DESCRIBE_SIZE];
	char time[LOG_TIME_SIZE];
} LogInfo;

// 初始化sqlite3
int initSQL(char path[FILE_PATH_SIZE], sqlite3 *db);
// 关闭sqlite3
int closeSQL(sqlite3 *db);

// 校验信息
int checkEmployeeInfo(char name[EMPLOYEE_NAME_SIZE], char pwd[EMPLOYEE_PWD_SIZE], EmployeeInfo *info);
// 创建员工
int createEmployeeInfo(EmployeeInfo *info, char token[LOGIN_TOKEN_SIZE]);
// 删除员工
int deleteEmployeeInfo(uint empno);
// 修改员工
int modifyEmployeeInfo(EmployeeInfo *info);
// 查询员工
int queryEmployeeInfo(uint empno, char name[EMPLOYEE_NAME_SIZE], EmployeeInfo info[QUERY_EMPLOYEE_COUNT]);

// 创建日志
int createLogInfo(LogInfo *info);
// 查询日志
int queryLogInfo(uint empno, char time[LOG_TIME_SIZE], LogInfo info[QUERY_LOG_COUNT]);

// 创建登录信息
int createLoginStateInfo(EmployeeInfo *info, char token[LOGIN_TOKEN_SIZE]);
// 删除登录信息
int deleteLoginStateInfo(char token[LOGIN_TOKEN_SIZE]);
// 校验登录信息
int checkLoginStateInfo(char token[LOGIN_TOKEN_SIZE]);


#endif //__SQLITE3_H__
