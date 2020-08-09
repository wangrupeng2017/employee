/************************************************
 * Name: mysqlite3.h
 * Date: 2020-08-08
 * Description: sqlite3基础封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/

#ifndef __MYSQLITE3_H__
#define __MYSQLITE3_H__

#include <sqlite3.h>

#include "common_base.h"

#define DB_PATH  "./employee.db"
#define TRY_SQLITE_ERROR(expr, describe, db, ...)  if (expr) { fprintf(stderr, "%s:%s\n", describe, sqlite3_errmsg(db)); __VA_ARGS__; exit(-1); }

#define CREATE_EMPLOEEY_TABLE "CREATE TABLE if not exists employee (no integer PRIMARY KEY autoincrement, name char, password char, age int, sex char, department char, salary int, role char);"
#define CREATE_LOGIN_TABLE "CREATE TABLE if not exists token(no integer PRIMARY KEY, login_time int, login_status char);"
#define CREATE_LOG_TABLE "CREATE TABLE if not exists operation_log(id integer PRIMARY KEY autoincrement, no int, description int, time int);"

// 员工信息结构体
typedef struct EmployeeInfo {
	uint  empno;
	char  name[EMPLOYEE_NAME_SIZE];
	char  pwd[EMPLOYEE_PWD_SIZE];
	uchar sex;
	uchar age;
	uint  salary;
	char  department[EMPLOYEE_DEPARTMENT_SIZE];
	uchar role;
} EmployeeInfo;

// 日志信息结构体
typedef struct LogInfo {
	uint empno;
	char description[LOG_DESCRIBE_SIZE];
	long time;
} LogInfo;

// 初始化sqlite3
int initSQL(sqlite3 **out);
// 关闭sqlite3
int closeSQL();

// 校验信息
int checkEmployeeInfo(char name[EMPLOYEE_NAME_SIZE], char pwd[EMPLOYEE_PWD_SIZE], EmployeeInfo *info);
// 创建员工
int createEmployeeInfo(EmployeeInfo *info, uint *empno);
// 删除员工
int deleteEmployeeInfo(uint empno);
// 修改员工
int modifyEmployeeInfo(EmployeeInfo *info);
// 查询员工
int queryEmployeeInfo(uint empno, char name[EMPLOYEE_NAME_SIZE], EmployeeInfo info[QUERY_EMPLOYEE_COUNT]);

// 创建日志
int createLogInfo(LogInfo *info);
// 查询日志
int queryLogInfo(char time[LOG_TIME_SIZE], LogInfo info[QUERY_LOG_COUNT]);

// 创建登录信息
int createLoginStateInfo(EmployeeInfo *info);
// 删除登录信息
int deleteLoginStateInfo(uint empno);
// 校验登录信息
int checkLoginStateInfo(uint empno);


#endif //__MYSQLITE3_H__
