/************************************************
 * Name: business.h
 * Date: 2020-08-08
 * Description: 业务处理方法头文件
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/

#ifndef __BUSINESS_H__
#define __BUSINESS_H__

#include "socket_models.h"


// 登录请求处理函数
int loginHandler(int sockfd, RequestInfo *info);
// 退出请求处理函数
int quitHandler(int sockfd, RequestInfo *info);
// 员工查询请求处理函数
int employeeQueryHandler(int sockfd, RequestInfo *info);
// 员工修改请求处理函数
int employeeModifyHandler(int sockfd, RequestInfo *info);
// 员工添加请求处理函数
int employeeAddHandler(int sockfd, RequestInfo *info);
// 员工删除请求处理函数
int employeeDeleteHandler(int sockfd, RequestInfo *info);
// 日志查询请求处理函数
int logsQueryHandler(int sockfd, RequestInfo *info);
// 员工签到处理函数
int signInHandler(int sockfd, RequestInfo *info);
// 查询签到信息处理函数
int signInInfoHandler(int sockfd, RequestInfo *info);


#endif //__BUSINESS_H__

