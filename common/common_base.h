/************************************************
 * Name: common_base.h
 * Date: 2020-08-08
 * Description: 公共基础头文件
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-08
 ************************************************/
#ifndef __COMMON_BASE_H__
#define __COMMON_BASE_H__


typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;


#define IP_SIZE 20
#define DAY_SECONDS (24*60*60)

#define ONE 1

#define TRY_ERROR(expr, desc, ...)   if (expr) { printf("[%s]:[%s]:[%d]:%s\n", __FILE__, __func__, __LINE__, desc); __VA_ARGS__; return -1; }
#define TRY_PERROR(expr, desc, ...)  if (expr) { perror(desc); __VA_ARGS__; return -1; }
#define TRY_SQLITE_ERROR(expr, describe, db, ...)  if (expr) { fprintf(stderr, "%s:%s\n", describe, sqlite3_errmsg(db)); __VA_ARGS__; exit(-1); }

typedef enum FuncStatus {
	FuncNormal    = 0,  //函数正常结束
	FuncException = -1, //函数功能异常
	FuncError     = -2, //函数发生错误
} FuncStatus;


#endif //__COMMON_BASE_H__
