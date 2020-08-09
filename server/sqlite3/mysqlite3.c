/************************************************
 * Name: mysqlite3.c 
 * Date: 2020-08-08
 * Description: sqlite3基础封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>


#include "mysqlite3.h"

// 数据库对象实例
static sqlite3* db_instance = NULL;
/*
 * function:    initSQL
 * description: 实例化sqlite3对象
 * @param [out] 
 *   out      : sqlite3对象实例
 * @return      0:处理成功 !0:处理出错
 */
int initSQL(sqlite3 **out)
{/*{{{*/
	// 打开数据库
	int ret = sqlite3_open(DB_PATH, &db_instance);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "打开数据库:", db_instance, return FuncError);

	// 创建数据表
	ret = sqlite3_exec(db_instance, CREATE_EMPLOEEY_TABLE, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格:", db_instance, return FuncError);
	ret = sqlite3_exec(db_instance, CREATE_LOGIN_TABLE, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格:", db_instance, return FuncError);
	ret = sqlite3_exec(db_instance, CREATE_LOG_TABLE, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格:", db_instance, return FuncError);

	*out = db_instance;
	return FuncNormal;
}/*}}}*/
/*
 * function:    closeSQL 
 * description: 关闭sqlite对象
 * @return      0:处理成功 !0:处理出错
 */
int closeSQL()
{/*{{{*/
	int ret = sqlite3_close(db_instance);
    TRY_SQLITE_ERROR(ret!=SQLITE_OK, "关闭数据库:", db_instance, return FuncError);
    puts("关闭数据库成功");

	return FuncNormal;
}/*}}}*/

/*
 * function:    checkEmployeeInfo
 * description: 校验员工信息
 * @param [ in] 
 * 	 name     : 用户名
 * 	 pwd      : 密码
 * @param [out] 
 * 	 info     : 用户信息
 * @return      0:处理成功 !0:处理出错
 */
int checkEmployeeInfo(char name[EMPLOYEE_NAME_SIZE], char pwd[EMPLOYEE_PWD_SIZE], EmployeeInfo *info)
{/*{{{*/
	char sql[256]  = "";
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;

	// 格式化SQL语句, 执行查询
	char *sql_format = "SELECT no, age, sex, salary, role, \
						name, password, department \
					    FROM employee \
					    WHERE name='%s' \
					    AND password='%s';";
	sprintf(sql, sql_format, name, pwd); printf("%s\n", sql);
	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "查询员工:", db_instance, return FuncError);

	
	// 格式化员工信息
	if (nRow > 0)
	{
		info->empno  = atoi(result[1*nColumn + 0]);
		info->age    = atoi(result[1*nColumn + 1]);
		info->sex    = atoi(result[1*nColumn + 2]);
		info->salary = atoi(result[1*nColumn + 3]);
		info->role   = atoi(result[1*nColumn + 4]);
		strcpy(info->name, result[1*nColumn + 5]);
		strcpy(info->pwd,  result[1*nColumn + 6]);
		strcpy(info->department, result[1*nColumn + 7]);
	}

	return FuncNormal;
}/*}}}*/

/*
 * function:    createEmployeeInfo
 * description: 创建员工信息
 * @param [ in] 
 * 	 info     : 员工信息
 * @param [out] 
 * 	 empno    : 员工号
 * @return      0:处理成功 !0:处理出错
 */
int createEmployeeInfo(EmployeeInfo *info, uint *empno)
{/*{{{*/
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;
	// 查询最大员工号
	int ret = sqlite3_get_table(db_instance, "SELECT no FROM employee ORDER BY no DESC LIMIT 1;", &result, &nRow, &nColumn, &pzErrmsg);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "查询最大员工号:", db_instance, return FuncError);
	(nRow == 0) ? (*empno = 1) : (*empno = atoi(result[1*nColumn + 0]) + 1);

	// 创建员工
	char sql[256] = "";
	char *sql_format = "INSERT INTO employee(no, age, sex, salary, role, name, password, department) \
					   VALUES('%ld', '%d', '%d', '%d', '%d', '%s', '%s', '%s');";
	sprintf(sql, sql_format, *empno, info->age, info->sex, info->salary, info->role, \
			info->name, info->pwd, info->department);
	ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建员工:", db_instance, return FuncError);

	return FuncNormal;
}/*}}}*/

/*
 * function:    deleteEmployeeInfo
 * description: 删除员工信息
 * @param [ in] 
 * 	 empno    : 员工号
 * @return      0:处理成功 !0:处理出错
 */
int deleteEmployeeInfo(uint empno)
{/*{{{*/
	char sql[256] = "";
	char *sql_format = "DELETE FROM employee \
					   WHERE no='%u';";
	sprintf(sql, sql_format, empno);
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "删除员工:", db_instance, return FuncError);

	return FuncNormal;
}/*}}}*/

/*
 * function:    modifyEmployeeInfo
 * description: 修改员工信息
 * @param [ in] 
 * 	 info     : 员工信息
 * @return      0:处理成功 !0:处理出错
 */
int modifyEmployeeInfo(EmployeeInfo *info)
{/*{{{*/
	char sql[256] = "";
	char *sql_format = "UPDATE employee \
					   SET age='%d', sex='%d', salary='%d', role='%d', \
					       name='%s', password='%s', department='%s' \
					   WHERE no='%d'";
	
	sprintf(sql, sql_format, info->age, info->sex, info->salary, info->role, \
			info->name, info->pwd, info->department, info->empno);
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "修改员工:", db_instance, return FuncError);
			
	return FuncNormal;
}/*}}}*/

/*
 * function:    queryEmployeeInfo
 * description: 查询员工信息
 * @param [ in] 
 * 	 empno    : 员工号
 * 	 name     : 用户名
 * @param [out] 
 *   info     : 员工信息
 * @return      查询数量
 */
int queryEmployeeInfo(uint empno, char name[EMPLOYEE_NAME_SIZE], EmployeeInfo info[QUERY_EMPLOYEE_COUNT])
{/*{{{*/
	char sql[256]  = "";
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;

	// 格式化SQL语句, 执行查询
	char *sql_format = "SELECT no, age, sex, salary, role, \
					   name, password, department \
					   FROM employee \
					   WHERE %s;";
	
	char condition[100] = "";
	(empno > 0) ? sprintf(condition, "no='%d'", empno) : sprintf(condition, "name='%s'", name);
	sprintf(sql, sql_format, condition);

	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "查询员工:", db_instance, return FuncError);
	// 最多查询20条
	if (nRow > 20) 
		nRow = 20;

	// 格式化查询结果
	int i = 0;
	int j = 0;
	for (i=1; i<=nRow; i++)
	{
		// 格式化员工信息
		info[i-1].empno  = atoi(result[i*nColumn + 0]);
		info[i-1].age    = atoi(result[i*nColumn + 1]);
		info[i-1].sex    = atoi(result[i*nColumn + 2]);
		info[i-1].salary = atoi(result[i*nColumn + 3]);
		info[i-1].role   = atoi(result[i*nColumn + 4]);
		strcpy(info[i-1].name,  result[i*nColumn + 5]);
		strcpy(info[i-1].pwd,   result[i*nColumn + 6]);
		strcpy(info[i-1].department, result[i*nColumn + 7]);
	}

	return nRow;
}/*}}}*/

/*
 * function:    createLogInfo
 * description: 创建日志
 * @param [ in] 
 * 	 info     : 日志信息
 * @return      0:处理成功 !0:处理出错
 */
int createLogInfo(LogInfo *info)
{/*{{{*/
	char sql[256] = "";
	char *sql_format = "INSERT INTO operation_log(no, description, time) \
					   VALUES('%d', '%s', '%s');";
	sprintf(sql, sql_format, info->empno, info->description, info->time);
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建日志:", db_instance, return FuncError);

	return FuncNormal;
}/*}}}*/

/*
 * function:    queryLogInfo
 * description: 查询日志
 * @param [ in] 
 * 	 time     : 日期
 * @param [out] 
 *   info     : 日志信息
 * @return      查询数量
 */
int queryLogInfo(char time[LOG_TIME_SIZE], LogInfo info[QUERY_LOG_COUNT])
{/*{{{*/
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;
	char sql[256]  = "";
	char *sql_format = "SELECT no, time, description FROM operation_log \
					   WHERE time>='%ld' AND time<='%ld';";
	struct tm stm;
	strptime(time, "%Y-%m-%d %H:%M:%S", &stm);
	sprintf(sql, sql_format, mktime(&stm)-24*60*60, mktime(&stm)+24*60*60);
	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "查询日志:", db_instance, return FuncError);
	// 最多查询20条
	if (nRow > 20) 
		nRow = 20;

	int i = 0;
	for (i=1; i<=nRow; i++)
	{
		info[i].empno = atoi(result[i*nColumn + 0]);
		info[i].time  = atoi(result[i*nColumn + 1]);
		strcpy(info[i].description, result[i*nColumn + 2]);	
	}

	return nRow;
}/*}}}*/

/*
 * function:    createLoginStateInfo
 * description: 创建登录信息
 * @param [ in] 
 * 	 info     : 员工信息
 * @return      0:成功  !0:处理出错
 */
int createLoginStateInfo(EmployeeInfo *info)
{/*{{{*/
	char sql[256] = "";
	char *sql_format = "INSERT INTO token(no, login_time, login_status) \
					   VALUES('%d', '%ld', '%s');";
	sprintf(sql, sql_format, info->empno, time(NULL), "在线");
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建登录信息:", db_instance, return FuncError);

	return FuncNormal;
}/*}}}*/

/*
 * function:    deleteLoginStateInfo
 * description: 删除登录信息
 * @param [ in] 
 * 	 empno    : 员工号
 * @return      0:成功  !0:处理出错
 */
int deleteLoginStateInfo(uint empno)
{/*{{{*/
	char sql[256] = "";
	char *sql_format = "DELETE FROM token WHERE no='%d';";
	sprintf(sql, sql_format, empno);
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "删除登录信息:", db_instance, return FuncError);

	return FuncNormal;
}/*}}}*/

/*
 * function:    deleteLoginStateInfo
 * description: 校验登录信息
 * @param [ in] 
 * 	 empno    : 员工号
 * @return      0:在线  !0:不在线
 */
int checkLoginStateInfo(uint empno)
{
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;
	char sql[256]  = "";
	char *sql_format = "SELECT * FROM token \
					   WHERE no='%d';";
	sprintf(sql, sql_format, empno);
	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "查询登录信息:", db_instance, return FuncError);
	
	return nRow>0 ? 0 : -1;
}
