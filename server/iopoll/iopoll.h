/************************************************
 * Name: iopoll.h
 * Date: 2020-08-10
 * Description: 多路复用poll封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-10
 ************************************************/

#ifndef __IOPOLL_H__
#define __IOPOLL_H__

#define _GNU_SOURCE 1
#include <poll.h>


// poll回调函数类型
typedef void (*iopoll_call_t)(int fd, int revents);

// 添加监听文件描述符
int iopollAdd(int fd, int events);
// 删除监听文件描述符
int iopollDel(int fd);
// 等待监听
int iopollWait(int timeout, iopoll_call_t call);
// 释放poll内存
void iopollDistroy();

#endif //__IOPOLL_H__
