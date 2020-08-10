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

// 业务处理入口
int businessEntrance(int fd);


// 登录请求处理函数
int loginHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 退出请求处理函数
int quitHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 员工查询请求处理函数
int employeeQueryHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 员工修改请求处理函数
int employeeModifyHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 员工添加请求处理函数
int employeeAddHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 员工删除请求处理函数
int employeeDeleteHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 日志查询请求处理函数
int logsQueryHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 员工签到处理函数
int signInHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);
// 查询签到信息处理函数
int signInInfoHandler(int fd, RequestInfo *req_head, void *req_data, ResponseInfo *res_head, void **res_data);


#endif //__BUSINESS_H__

