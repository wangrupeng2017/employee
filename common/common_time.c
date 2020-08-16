
#include "common_time.h"


// 时间戳转字符串YYYY-MM-DD HH:MM:SS格式
int timestampToTimestr(time_t t, char *out)
{
	strftime(out, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return FuncNormal;
}

// 时间戳转字符串YYYY-MM-DD格式
int timestampToDatestr(time_t t, char *out)
{
	strftime(out, 20, "%Y-%m-%d", localtime(&t));
	return FuncNormal;
}

// 日期字符串转时间戳
time_t datestrToTimestamp(char *date)
{
	struct tm stm;
	char   time_str[30] = "";

	sprintf(time_str, "%s 00:00:00", date);
	strptime(time_str, "%Y-%m-%d %H:%M:%S", &stm);
	return mktime(&stm);
}

// 获取本日00:00:00的时间戳
time_t nowDayTimestamp(void)
{
	char datestr[20] = "";
	char timestr[20] = "";
	timestampToDatestr(time(NULL), datestr);
	sprintf(timestr, "%s 00:00:00", datestr);

	struct tm stm;
	strptime(timestr, "%Y-%m-%d %H:%M:%S", &stm);
	return mktime(&stm);	
}

// 获取本月1日 00:00:00的时间戳
time_t nowMonthTimestamp(void)
{
	char datestr[20] = "";
	char timestr[20] = "";
	time_t nowtime   = time(NULL);
	strftime(datestr, 20, "%Y-%m", localtime(&nowtime));
	sprintf(timestr, "%s-01 00:00:00", datestr);

	struct tm stm;
	strptime(timestr, "%Y-%m-%d %H:%M:%S", &stm);
	return mktime(&stm);	
}

// 获取某天的时间戳
time_t dateTimestamp(uint year, uint month, uint day)
{
	char timestr[20] = "";
	sprintf(timestr, "%d-%d-%d 00:00:00", year, month, day);

	struct tm stm;
	strptime(timestr, "%Y-%m-%d %H:%M:%S", &stm);
	return mktime(&stm);	
}


