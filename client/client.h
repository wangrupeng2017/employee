#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "../common/socket_models.h"

#define YES "yes"
#define STR_MALE "男"
#define STR_FEMALE "女"

/*
 * Description 		: 连接socket服务器
 * function 		:   connectServer 
 * @param [ in] 
 * 		char [20] ip 服务端ip 地址
 * 		int port     服务端端口号
 * @param [out] 
 * @return      
 * 		int 		 已连接的文件描述符
 * @Author      xuyuanbing
 * @Other       
 */
int connectServer(char *ip, int port);

/*
 * description : 登录业务
 * function    : 
 * @param [ in]: 
 * 		int fd, 
 * 		LoginModel *login_model
 * @param [out]: 
 * 		LoginResult *result_model
 * @return     : 
 *    0:登陆成功 !0: 登陆失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int loginBusiness(int fd, LoginModel *login_model, LoginResult *result_model);

// 打印登陆菜单
void showLoginMenu(void);

// 接收菜单输入(注意:输入格式并清除输入缓冲区)
int getHomeMenuChoose(void);

/*
 * description : 交互获取登录信息 用户名/密码 格式化登录数据
 * function    : getLoginModel
 * @param [ in]: 
 * @param [out]: 
 * 		LoginModel *model 用户登陆信息
 * @return     : 
 * 		int 0: 获取用户信息成功
 * 			1: 获取用户信息失败
 * @Author     : xuyuanbing
 * @Other      : 
 */
int getLoginModel(LoginModel *model);

/*
 * description : 发送登录请求, 接收结果
 * function    : sendLoginRequest
 * @param [ in]: 
 * 		LoginModel *model 用户登陆信息
 * @param [out]: 
 *  	LoginResult *out 服务器端返回的信息
 * @return     : 
 * 		int 0: 发送登陆请求成功
 * 			1: 发送登陆请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
static int sendLoginRequest(int fd, LoginModel *model, LoginResult *out);

/*
 * description : 通用的请求服务端的方法 不是放在这个文件的 TODO
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor
 * 		void * request_data 请求信息
 * 		size_t request_data_size
 * 		size_t response_data_size
 * @param [out]: 
 * 		void * response_data 请求的返回结果
 * @return     : 
 * 		0:请求成功 !0:请求出错
 * @Author     : xuyuanbing
 * @Other      : 
 */

int request(int file_descriptor, void * request_head, 
		size_t request_head_size, void * request_data,
		size_t request_data_size, void * response_head,
		size_t response_head_size, void * response_data, size_t response_data_size);

void getDataFgets(char * data, size_t size);

/*
 * description : 普通员工退出业务
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor 
 * 		LoginModel *login_model
 * @param [out]: 
 * @return     : 返回值: 0:成功 !0:出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int employeeQuitBusiness(int file_descriptor, LoginResult *login_model);


/*
 * description : 退出请求信息, 发送退出请求
 * function    : 
 * @param [ in]: 
 * 		int file_descriptor 
 * 		QuitModel *quit_model 
 * @return     : 返回值: 0:成功 !0:出错
 * @Author     : xuyuanbing
 * @Other      : 
 */
int sendQuitRequest(int file_descriptor, QuitModel *quit_model);


#endif // __CLIENT_H__
