/*******************************************************************************
 * admin.h 管理员的一些操作
 *
 * @fcopyright  Copyright 2020 SIC Microelectronics CO. Ltd.
 * @fauthor     [Xu Yuanbing] [569636976@qq.com]
 * @fversion    1.0
 *
 * @fhistory    [2020-08-09] Create file.
 ******************************************************************************/
#ifndef __ADMIN_H__
#define __ADMIN_H__

#include "client.h"

#define STR_MALE "男"
#define STR_FEMALE "女"

/*
 * description : 管理员业务
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int doAdminBusiness(int file_descriptor, LoginResultModel * login_model);

/*
 * description : 打印管理员菜单
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
 void showAdminMenu(void);

// 解析菜单输入
int gotoAdminChoose(int file_descriptor, int userChoose, LoginResultModel * login_model);

/*
 * description : 接收菜单输入
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 * @return     : int 选择的菜单序号
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getAdminMenuChoose(void);


/*
 * description : 添加用户业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		loginresultmodel * login_model
 * @param [out]: 
 * @return     : 返回值: 0:新建用户成功 !0:新建用户失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int adminAddBusiness(int file_descriptor, LoginResultModel * login_model);

/*
 * description : 交互获取用户信息 员工号(自动生成)/用户名/密码(初始:123)/性别/年龄/部门/工资
 * function    : getAdminAddModel
 * @param [ in]: 
 *     EmployeeCreateModel * create_model 请求信息
 * @param [out]: 
 * @return     : 
 *   返回值: 0:信息获取成功 !0:获取错误或用户取消操作
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getAdminAddModel(EmployeeCreateModel * create_model);


/*
 * description : 格式化添加用户数据, 发送添加用户请求, 接收处理结果
 * function    : 
 * @param [ in]: 
 * 		EmployeeCreateModel*create_model 请求信息添加新员工
 * 		int file_descriptor
 * @param [out]: 
 * 		EmployeeCreateResult *result 请求添加用户的结果
 * @return     : 
 * 		0:请求成功 !0:请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendAdminAddRequest(int file_descriptor, EmployeeCreateModel *create_model, EmployeeCreateResult *result);

/*
 * description : 删除用户业务
 * function    : 
 * @param [ in]: int file_descriptor 
 * 		LoginResultModel * login_model
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int adminDeleteBusiness(int file_descriptor, LoginResultModel * login_model);


/*
 * description : 交互获取用户选择的用户员工号
 * function    : 
 * @param [ in]: 
 * 		sdtio
 * @param [out]: 
 *   	int *employee_number 员工号
 * @return     : 
 *   0:信息获取成功 !0:获取错误或用户取消操作
 * @Author     : xuyuanbing
 * @Other      : 
 */
void getEmployeeNumber(uint *employee_number);

/*
 * description : 发送删除用户请求, 接收查询结果
 * function    : 
 * @param [ in]: 
 *  	int file_descriptor
 *  	EmployeeDeleteModel *delete_model 删除信息
 * @param [out]: 
 * @return     : 
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendEmployeeDeleteRequest (int file_descriptor, EmployeeDeleteModel *delete_model);

// 6.修改用户业务:::
int adminModifyBusiness(int file_descriptor, LoginResultModel *login_model);

/*
 * description : 交互获取用户信息(用户名)
 * function    : 
 * @param [ in]: 
 * 		char * name 用户名
 * 		size_t size
 * @param [out]: 
 * @return     : 
 * 		0:请求成功 !0:请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeName(char * name, size_t size);


int sendAdminEmployeeQueryRequest(int file_descriptor, EmployeeQueryModel *req_data);

/*
 * description : 交互获取用户信息 用户名/密码/性别/年龄/部门/工资
 * function    : 
 * @param [ in]: 
 * @param [out]: 
 *  	EmployeeModifyModel *modif_model 新的员工信息
 * @return     : 
 * 		0:信息获取成功 !0:获取错误或用户取消操作	
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getEmployeeNewinfo(EmployeeModifyModel *modify_model);


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

int sendAdminModifyRequest(int file_descriptor, EmployeeModifyModel *modify_model);

#endif /* __ADMIN_H__ */
