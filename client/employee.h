/*******************************************************************************
 * employee.h 普通用户的一些基本操作
 *
 * @fcopyright  Copyright 2020 SIC Microelectronics CO. Ltd.
 * @fauthor     [Xu Yuanbing] [569636976@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2020-08-09] Create file.
 ******************************************************************************/

#ifndef __EMPLOYEE_H__
#define __EMPLOYEE_H__

#include "client.h"


// 普通用户的业务
/*
 * description : 普通用户的业务
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int doEmployeeBusiness(int file_descriptor, LoginResult * login_model);

/*
 * description : 打印普通用户菜单
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
void showEmployeeMenu(void);

/*
 * description : 解析菜单输入
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 0: 退出到登陆页面，!0:返回上一级 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int gotoEmployeeChoose(int file_descriptor, int userChoose, LoginResult * login_model);

/*
 * description : 普通员工信息查询业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		LoginModel * login_model
 * @param [out]: 
 * @return     : 0:正常退出  !0:错误退出
 * @Author     : xuyuanbing
 * @Other      : 
 */
int employeeQueryBusiness(int file_descriptor, LoginResult * login_model);

/*
 * description : 发送信息查询请求, 接收查询结果
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		EmployeeQueryModel* query_model
 * @param [out]: 
 *	    EmployeeQueryResult *query_Result
 * @return     : 0:正常退出  !0:错误退出
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendEmployeeQueryRequest(int file_descriptor, EmployeeQueryModel* query_model,
		EmployeeQueryResult *query_Result);

/*
 * description : 普通员工信息修改业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		LoginModel * login_model
 * @param [out]: 
 * @return     : 0:修改成功 !0:修改出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int employeeModifyBusiness(int file_descriptor, LoginResult * login_model);

/*
 * description : 交互获取用户信息 用户名/密码/性别/年龄
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * 		EmployeeModifyModel*model
 * @return     : 0:信息获取成功 !0:获取错误或用户取消操作
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeModifyModel(EmployeeModifyModel*model);

/*
 * description : 格式化信息修改数据, 发送信息修改请求, 接收处理结果
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		EmployeeModifyModel*model
 * @param [out]: 
 * @return     : 0:修改成功 !0:修改出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendEmployeeModifyRequest(int file_descriptor, EmployeeModifyModel*model);

/*
 * description : 员工打卡
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int signinEmployee(int file_descriptor, LoginResult * login_model);

/*
 * description : 查询打开信息
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getSigninInfo(int file_descriptor, LoginResult * login_model);

#endif /* __EMPLOYEE_H__ */
