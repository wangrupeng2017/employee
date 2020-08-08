#ifndef __CLIENT_H__
#define __CLIENT_H__

// 连接socket服务器 
int connectServer(char[20] ip, int port);

// 打印首页菜单
void showHomeMenu(void);

// 接收菜单输入(注意:输入格式并清除输入缓冲区)
int getHomeMenuChoose(void);

// 解析菜单输入
void gotoChoose(int userChoose);


// 登录业务:
int login_business(void);

//  交互获取登录信息 用户名/密码 格式化登录数据:::
    static int get_login_model(struct LoginModel *model);

 // 发送登录请求, 接收结果:::
   static int send_login_request(struct LoginModel *model, struct LoginResultModel *out);



#endif // __CLIENT_H__
