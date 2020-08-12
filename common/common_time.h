/************************************************
 * Name: common_time.h
 * Date: 2020-08-12
 * Description: 公共基础时间方法头文件
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-12
 ************************************************/
#ifndef __COMMON_TIME_H__
#define __COMMON_TIME_H__

#include <stdio.h>
#include <time.h>
#include "common_base.h"


// 时间戳转字符串YYYY-MM-DD HH:MM:SS格式
int timestampToTimestr(time_t t, char *out);
// 时间戳转字符串YYYY-MM-DD格式
int timestampToDatestr(time_t t, char *out);
// 日期字符串转时间戳
time_t datestrToTimestamp(char *date);
// 获取本日00:00:00的时间戳
time_t nowDayTimestamp(void);
// 获取本月1日 00:00:00的时间戳
time_t nowMonthTimestamp(void);
// 获取某天的时间戳
time_t dateTimestamp(uint year, uint month, uint day);


#endif //__COMMON_TIME_H__
