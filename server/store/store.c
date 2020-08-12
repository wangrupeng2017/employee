/************************************************
 * Name: store.c 
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


#include "store.h"

// 数据库对象实例
static sqlite3* db_instance = NULL;
/*
 * function:    initSQL
 * description: 实例化sqlite3对象
 * @param [out] out sqlite3对象实例
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
	ret = sqlite3_exec(db_instance, CREATE_SIGNIN_TABLE, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建表格:", db_instance, return FuncError);

	// 添加默认管理员
	sqlite3_exec(db_instance, DELETE_ADMIN_DATA, NULL, NULL, NULL);
	ret = sqlite3_exec(db_instance, INSERT_ADMIN_DATA, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "添加管理员:", db_instance, return FuncError);

	// 添加普通员工帐号
	ret = sqlite3_exec(db_instance, INSERT_TEST_DATA, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "添加普通员工:", db_instance, return FuncError);

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
 * function:    查询操作的基础封装
 * @param [ in] sql SQL语句
 * @param [ in] call 数据结构格式化回调方法
 * @param [ in] datasize 格式化后的元素数据大小
 * @param [out] outdatas 传出的数据
 * @return      操作结果：0:成功，!0:失败
 */
int execQuerySql(char *sql, dataFormatCall call, int datasize, void *outdatas)
{
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;

	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	if (ret!=SQLITE_OK) return ret;

	int i = 0;
	for (i=1; i<=nRow; i++)
	{
		call(result+(nColumn*i), outdatas+(datasize*(i-1)));
	}

	sqlite3_free_table(result);
	return nRow;
}

/*
 * function:    checkEmployeeInfo
 * description: 校验员工信息
 * @param [ in] name 用户名
 * @param [ in] pwd 密码
 * @param [out] info 用户信息
 * @return      0:处理成功 !0:处理出错
 */
int checkEmployeeInfo(ename_t name, epwd_t pwd, EmployeeInfo *info)
{
	// 格式化SQL语句
	char sql[256]  = "";
	char *sql_format = "SELECT no, age, sex, salary, role, name, password, department \
					    FROM employee  WHERE name='%s'  AND password='%s' limit 1;";
	sprintf(sql, sql_format, name, pwd); 

	// 执行查询
	int ret = execQuerySql(sql, formatEmployeeCall, sizeof(EmployeeInfo), info);
	TRY_SQLITE_ERROR(ret<0, "查询员工:", db_instance, return FuncError);
	
	return FuncNormal;
}

/*
 * function:    createEmployeeInfo
 * description: 创建员工信息
 * @param [ in] info 用户信息
 * @param [out] empno 员工号
 * @return      0:处理成功 !0:处理出错
 */
int createEmployeeInfo(EmployeeInfo *info, uint *empno)
{
	// 查询最大员工号
	*empno = 0;
	int ret = execQuerySql("SELECT no FROM employee ORDER BY no DESC LIMIT 1;", formatEmpnoCall, sizeof(uint), empno);
	TRY_SQLITE_ERROR(ret<0, "查询最大员工号:", db_instance, return FuncError);
	*empno += 1;
	
	// 格式化创建SQL语句
	char sql[256] = "";
	char *sql_format = "INSERT INTO employee(no, age, sex, salary, role, name, password, department) \
					   VALUES('%ld', '%d', '%d', '%d', '%d', '%s', '%s', '%s');";
	sprintf(sql, sql_format, *empno, info->age, info->sex, info->salary, info->role, \
			info->name, info->pwd, info->department);

	// 执行创建语句
	ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建员工:", db_instance, return FuncError);

	return FuncNormal;
}

/*
 * function:    deleteEmployeeInfo
 * description: 删除员工信息
 * @param [ in] empno 员工号
 * @return      0:处理成功 !0:处理出错
 */
int deleteEmployeeInfo(uint empno)
{
	// 格式化删除员工SQL语句
	char sql[256] = "";
	char *sql_format = "DELETE FROM employee  WHERE no='%u';";
	sprintf(sql, sql_format, empno);

	// 执行删除员工SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "删除员工:", db_instance, return FuncError);

	return FuncNormal;
}

/*
 * function:    modifyEmployeeInfo
 * description: 修改员工信息
 * @param [ in] info 员工信息
 * @return      0:处理成功 !0:处理出错
 */
int modifyEmployeeInfo(EmployeeInfo *info)
{
	// 格式化修改员工SQL语句
	char sql[256] = "";
	char *sql_format = "UPDATE employee \
					   SET age='%d', sex='%d', salary='%d', role='%d', \
					       name='%s', password='%s', department='%s' \
					   WHERE no='%d'";
	sprintf(sql, sql_format, info->age, info->sex, info->salary, info->role, \
			info->name, info->pwd, info->department, info->empno);

	// 执行修改员工SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "修改员工:", db_instance, return FuncError);
			
	return FuncNormal;
}

/*
 * function:    queryEmployeeInfo
 * description: 查询员工信息
 * @param [ in] empno 员工号
 * @param [ in] name 用户名
 * @param [out] info 员工信息
 * @return      查询数量
 */
int queryEmployeeInfo(uint empno, ename_t name, EmployeeInfo *info, uint max_count)
{
	// 格式化查询员工SQL语句
	char sql[256]  = "";
	char condition[100] = "";
	char *sql_format = "SELECT no, age, sex, salary, role, name, password, department \
					    FROM employee WHERE %s  LIMIT %d;";
	(empno > 0) ? sprintf(condition, "no='%d'", empno) : sprintf(condition, "name like '%%%s%%'", name);
	sprintf(sql, sql_format, condition, max_count);

	// 执行查询员工信息SQL
	int ret = execQuerySql(sql, formatEmployeeCall, sizeof(EmployeeInfo), info);
	TRY_SQLITE_ERROR(ret<0, "查询员工:", db_instance, return FuncError);

	return ret;
}

/*
 * function:    createLogInfo
 * description: 创建日志
 * @param [ in] info 日志信息
 * @return      0:处理成功 !0:处理出错
 */
int createLogInfo(OperationLogInfo *info)
{
	// 格式化创建日志SQL
	char sql[256] = "";
	char *sql_format = "INSERT INTO operation_log(no, description, time) \
					   VALUES('%d', '%s', '%ld');";
	sprintf(sql, sql_format, info->empno, info->description, info->time);

	// 执行创建日志SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建日志:", db_instance, return FuncError);

	return FuncNormal;
}

/*
 * function:    queryLogInfo
 * description: 查询日志
 * @param [ in] time 查询日期字符串 YYYY-MM-DD
 * @param [out] info 日志信息
 * @return      查询数量
 */
int queryLogInfo(stime_t time, LogQueryResult info[QUERY_LOG_COUNT])
{
	// 格式化查询日志SQL
	char sql[256]  = "";
	char *sql_format = "SELECT no, time, description FROM operation_log \
					   WHERE time>='%ld' AND time<='%ld' ORDER BY time DESC LIMIT 20;";
	time_t tstamp = datestrToTimestamp(time);
	sprintf(sql, sql_format, tstamp-DAY_SECONDS, tstamp+DAY_SECONDS);

	// 执行查询日志SQL
	int ret = execQuerySql(sql, formatLogCall, sizeof(LogQueryResult), info);
	TRY_SQLITE_ERROR(ret<0, "查询日志:", db_instance, return FuncError);

	return ret;
}

/*
 * function:    saveLogs
 * description: 记录日志
 * @param [ in] empno 员工号
 * @param [ in] message 日志内容
 */
void saveLogs(uint empno, char message[50])
{
	OperationLogInfo info = {0};
	info.empno = empno;
	info.time  = time(NULL);
	strcpy(info.description, message);

	createLogInfo(&info);
}

/*
 * function:    createLoginStateInfo
 * description: 创建登录信息
 * @param [ in] sockfd 客户端文件描述符
 * @param [ in] info 员工信息
 * @return      0:成功  !0:处理出错
 */
int createLoginStateInfo(int sockfd, EmployeeInfo *info)
{
	char sql[256] = "";
	sprintf(sql, "DELETE FROM token WHERE sockfd='%d';", sockfd);
	sqlite3_exec(db_instance, sql, NULL, NULL, NULL); 

	bzero(sql, sizeof(sql));
	char *sql_format = "INSERT INTO token(sockfd, no, login_time, login_status) \
					   VALUES('%d', '%d', '%ld', '%s');";
	sprintf(sql, sql_format, sockfd, info->empno, time(NULL), "在线");

	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "创建登录信息:", db_instance, return FuncError);

	return FuncNormal;
}

/*
 * function:    deleteLoginStateInfo
 * description: 删除登录信息
 * @param [ in] empno 员工号
 * @return      0:成功  !0:处理出错
 */
int deleteLoginStateInfo(uint empno)
{
	char sql[256] = "";
	char *sql_format = "DELETE FROM token WHERE no='%d';";
	sprintf(sql, sql_format, empno);

	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "删除登录信息:", db_instance, return FuncError);

	return FuncNormal;
}

/*
 * function:    deleteLoginStateInfo
 * description: 校验登录信息
 * @param [ in] sockfd 客户端文件描述符
 * @return      >0:员工号, <0:不在线/未登录
 */
int checkLoginStateInfo(int sockfd)
{
	char **result  = NULL;
	char *pzErrmsg = NULL;
	int nRow       = 0;
	int nColumn    = 0;
	char sql[256]  = "";
	char *sql_format = "SELECT no FROM token  WHERE sockfd='%d';";
	sprintf(sql, sql_format, sockfd);

	int ret = sqlite3_get_table(db_instance, sql, &result, &nRow, &nColumn, &pzErrmsg);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "查询登录信息:", db_instance, return FuncError);
	
	return nRow>0 ? atoi(result[1*nColumn + 0]) : -1;
}


/*
 * function:    checkSigninInfo
 * description: 检查是否签到
 * @param [ in] empno 员工号
 * @param [ in] out 1:已签到，0:未签到
 * @return      0:成功  !0:处理出错
 */
int checkSigninInfo(int empno, int *out)
{
	// 获取今日00:00:00时间戳
	time_t nowDay = nowDayTimestamp();

	// 格式化SQL语句
	char sql[256]  = "";
	char *sql_format = "SELECT no FROM signin  WHERE no='%d' and time>'%d' and time<'%d';";
	sprintf(sql, sql_format, empno, nowDay, nowDay+DAY_SECONDS);

	// 执行查询签到记录SQL
	uint signin_empno = -1;
	int ret = execQuerySql(sql, formatEmpnoCall, sizeof(uint), &signin_empno);
	TRY_SQLITE_ERROR(ret<0, "检查是否签到:", db_instance, return FuncError);
		
	empno==signin_empno ? (*out=1) : (*out=0);
	return FuncNormal;
}

/*
 * function:    createSigninInfo
 * description: 新增签到信息
 * @param [ in] empno 员工号
 * @return      0:成功  !0:处理出错
 */
int createSigninInfo(int empno)
{
	// 格式化创建签到信息SQL
	char sql[256] = "";
	char *sql_format = "INSERT INTO signin(no, time) VALUES('%d', '%d');";
	sprintf(sql, sql_format, empno, time(NULL));

	// 执行创建签到信息SQL
	int ret = sqlite3_exec(db_instance, sql, NULL, NULL, NULL);
	TRY_SQLITE_ERROR(ret!=SQLITE_OK, "新增签到信息:", db_instance, return FuncError);

	return FuncNormal;
}

/*
 * function:    querySigninCount
 * description: 查询本月签到计数
 * @param [ in] empno 员工号
 * @param [out] out 签到信息
 * @return      0:成功  !0:处理出错
 */
int querySigninCount(int empno, uint *out)
{
	// 获取本月初时间戳，下月初时间戳
	time_t nowtime  = time(NULL);
	struct tm *tm_t = localtime(&nowtime);
	uint year  = tm_t->tm_year + 1900;
	uint month = tm_t->tm_mon  + 1;
	time_t nowMonthTstamp  = dateTimestamp(year, month, 1);
	if (month+1 > 12) { year+=1; month=0; }
	time_t nextMonthTstamp = dateTimestamp(year, month+1, 1);

	// 格式化查询本月签到总数的SQL
	char sql[256] = "";
	char *sql_format = "SELECT COUNT(*) FROM signin WHERE no='%d' and time>'%d' and time<'%d';";
	sprintf(sql, sql_format, empno, nowMonthTstamp, nextMonthTstamp);

	printf("sql:%s\n", sql);
	// 执行查询本月签到总数SQL
	uint signin_count = 0;
	int ret = execQuerySql(sql, formatCountCall, sizeof(uint), &signin_count);
	TRY_SQLITE_ERROR(ret<0, "查询本月签到计数:", db_instance, return FuncError);

	*out = signin_count;
	return FuncNormal;
}


// 格式化查询总数方法
void formatCountCall(char **row, void *outdata)
{
	uint *count = outdata;	
	*count = atoi(row[0]);
}

// 格式化日志信息
void formatLogCall(char **row, void *outdata)
{
	LogQueryResult *info = outdata;

	info->empno = atoi(row[0]);
	strcpy(info->describe, row[2]);	
	timestampToTimestr(atoi(row[1]), info->date);
}

// 格式化员工号信息
void formatEmpnoCall(char **row, void *outdata)
{
	uint *empno = outdata;
	*empno = atoi(row[0]);
}

// 格式化员工信息
void formatEmployeeCall(char **row, void *outdata)
{
	EmployeeInfo *info = outdata;

	info->empno  = atoi(row[0]);
	info->age    = atoi(row[1]);
	info->sex    = atoi(row[2]);
	info->salary = atoi(row[3]);
	info->role   = atoi(row[4]);
	strcpy(info->name,  row[5]);
	strcpy(info->pwd,   row[6]);
	strcpy(info->department, row[7]);
}




