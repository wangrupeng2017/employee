#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "../common/socket_models.h"

// 连接socket服务器 
int connectServer(char* ip, int port);

// 打印首页菜单
void showHomeMenu(void);

// 接收菜单输入(注意:输入格式并清除输入缓冲区)
int getHomeMenuChoose(void);

// 解析菜单输入
void gotoChoose(int userChoose);


// 登录业务:
int loginBusiness(int);

//  交互获取登录信息 用户名/密码 格式化登录数据:::
     int getLoginModel( LoginModel *model);

 // 发送登录请求, 接收结果:::
   static int sendLoginRequest(int,  LoginModel *model,  LoginResultModel *out);



#endif // __CLIENT_H__
