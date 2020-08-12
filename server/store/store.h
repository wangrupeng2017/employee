/************************************************
 * Name: store.h
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
#include "common_time.h"
#include "socket_models.h"
#include "store_models.h"

#define DB_PATH  "./employee.db"

#define CREATE_EMPLOEEY_TABLE "CREATE TABLE if not exists employee(no integer PRIMARY KEY autoincrement, name txt, password txt, department txt, age int, sex int, salary int, role int);"
#define CREATE_LOGIN_TABLE "CREATE TABLE if not exists token(sockfd integer PRIMARY KEY, no integer, login_time int, login_status txt);"
#define CREATE_LOG_TABLE "CREATE TABLE if not exists operation_log(id integer PRIMARY KEY autoincrement, no int, description txt, time int);"
#define CREATE_SIGNIN_TABLE "CREATE TABLE if not exists signin(id integer PRIMARY KEY autoincrement, no int, time int);"
#define DELETE_ADMIN_DATA "DELETE FROM employee WHERE no=1;"
#define INSERT_ADMIN_DATA "INSERT INTO employee(no, name, password, department, age, sex, salary, role) VALUES(1, 'admin', 'admin', '行政部门', 1, 1, 1, 2);"
#define INSERT_TEST_DATA "INSERT INTO employee(name, password, department, age, sex, salary, role) VALUES('test', 'test', '网络部', 1, 1, 1, 1);"

// 初始化sqlite3
int initSQL(sqlite3 **out);
// 关闭sqlite3
int closeSQL();


// 查询方法封装
typedef void (*dataFormatCall) (char **row, void *outdata);
int execQuerySql(char *sql, dataFormatCall call, int datasize, void *outdatas);
// 格式化日志信息
void formatLogCall(char **row, void *outdata);
// 格式化查询总数方法
void formatCountCall(char **row, void *outdata);
// 格式化员工号信息
void formatEmpnoCall(char **row, void *outdata);
// 格式化员工信息
void formatEmployeeCall(char **row, void *outdata);


// 校验信息
int checkEmployeeInfo(ename_t name, epwd_t pwd, EmployeeInfo *info);
// 创建员工
int createEmployeeInfo(EmployeeInfo *info, uint *empno);
// 删除员工
int deleteEmployeeInfo(uint empno);
// 修改员工
int modifyEmployeeInfo(EmployeeInfo *info);
// 查询员工
int queryEmployeeInfo(uint empno, ename_t name, EmployeeInfo info[QUERY_EMPLOYEE_COUNT]);

// 创建日志
int createLogInfo(OperationLogInfo *info);
// 查询日志
int queryLogInfo(stime_t time, LogQueryResult info[QUERY_LOG_COUNT]);
// 记录日志
void saveLogs(uint empno, char message[50]);

// 创建登录信息
int createLoginStateInfo(int sockfd, EmployeeInfo *info);
// 删除登录信息
int deleteLoginStateInfo(uint empno);
// 校验登录信息
int checkLoginStateInfo(int sockfd);

// 检查是否签到
int checkSigninInfo(int empno, int *out);
// 新增签到信息
int createSigninInfo(int empno);
// 查询本月签到情况
int querySigninCount(int empno, uint *out);

#endif //__MYSQLITE3_H__
