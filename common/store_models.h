/************************************************
 * Name: store_models.c
 * Date: 2020-08-11
 * Description: 数据库数据模型
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-11
 ************************************************/

#ifndef __STORE_MODELS_H__
#define __STORE_MODELS_H__

#include "common_base.h"

#define QUERY_LOG_COUNT 20
#define QUERY_EMPLOYEE_COUNT 20

typedef char (ename_t)[50];
typedef char (epwd_t)[50];
typedef char (edepartment_t)[50];
typedef char (lstatus_t)[40];
typedef char (opdesc_t)[40];
typedef char (stime_t)[50];

// 员工信息结构体
typedef struct EmployeeInfo {
	uint          empno;       //员工号
	ename_t       name;        //用户名
	epwd_t        pwd;         //密码
	edepartment_t department;  //部门
	uchar         age;         //年龄
	uchar         sex;         //性别
	uint          salary;      //工资
	uchar         role;        //角色
} EmployeeInfo;

// 在线用户信息结构体
typedef struct TokenInfo {
	uint      sockfd;          //文件描述符
	uint      empno;           //员工号 
	long      login_time;      //登录时间
	lstatus_t login_status;    //登录状态
} TokenInfo;

// 日志信息结构体
typedef struct OperationLogInfo {
	uint     id;               //日志序号
	uint     empno;            //员工号
	opdesc_t description;      //操作描述
	long     time;             //操作时间
} OperationLogInfo;

// 签到信息结构体
typedef struct SigninInfo {
	uint id;                   //序号
	uint empno;                //员工号
	long time;                 //签到时间
} SigninInfo;


#endif //__STORE_MODELS_H__
