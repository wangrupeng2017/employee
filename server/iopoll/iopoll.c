/************************************************
 * Name: iopoll.c
 * Date: 2020-08-10
 * Description: 多路复用poll封装
 * Author: wrp
 * Version: v1.0
 * Modification: 2020-08-10
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_base.h"
#include "iopoll.h"


// poll文件描述符
static struct pollfd *g_pollfds = NULL;
static int            g_nfds    = 0;



/*
 * function:    添加监听文件描述符
 * @param [ in] fd 添加的文件描述符
 * @param [ in] events 监听的事件
 * @return      正常:fds的长度 错误:<0
 */
int iopollAdd(int fd, int events)
{/*{{{*/
	struct pollfd pfd = {
		.fd     = fd,
		.events = events
	};
	
	int i = 0;
	int insert_index = g_nfds;
	for (i=0; i<g_nfds; i++)
	{
		// 修改pollfd信息
		if (g_pollfds[i].fd == pfd.fd)
		{
			g_pollfds[i].events  = pfd.events;
			g_pollfds[i].revents = pfd.revents;
			return g_nfds;
		}

		// 插入新pollfd节点
		if (g_pollfds[i].fd > pfd.fd)
		{
			insert_index = i;
			break;
		}
	}

	// 新增pfd节点
	int pfd_size = sizeof(struct pollfd);
	struct pollfd *out_fds = (struct pollfd*)malloc(pfd_size * (g_nfds+1));
	for (i=0; i<=g_nfds; i++)
	{
		if (i<insert_index)       memcpy(&out_fds[i], &g_pollfds[i],   pfd_size);
		else if (i==insert_index) memcpy(&out_fds[i], &pfd,          pfd_size);
		else if (i>insert_index)  memcpy(&out_fds[i], &g_pollfds[i-1], pfd_size);
	}

	if (g_pollfds) free(g_pollfds);
	g_pollfds = out_fds;
	g_nfds   += 1;
	return g_nfds;
}/*}}}*/


/*
 * function:    删除监听文件描述符
 * @param [ in] fd 要删除的文件描述符
 * @return      正常:fds的长度 错误:<0
 */
int iopollDel(int fd)
{/*{{{*/
	int i = 0;
	int delete_index = -1;
	for (i=0; i<g_nfds; i++)
	{
		// 要删除的节点
		if (g_pollfds[i].fd == fd)
		{
			delete_index = i;
			break;
		}

		// 未找到
		if (g_pollfds[i].fd > fd)
			break;
	}

	// 没有要删除的节点
	if (delete_index == -1)
		return g_nfds;

	
	// 删除pfd节点
	int pfd_size = sizeof(struct pollfd);
	struct pollfd *out_fds = (struct pollfd*)malloc(pfd_size * (g_nfds-1));
	for (i=0; i<g_nfds; i++)
	{
		if (i<delete_index)       memcpy(&out_fds[i],   &g_pollfds[i], pfd_size);
		else if (i==delete_index) continue;
		else if (i>delete_index)  memcpy(&out_fds[i-1], &g_pollfds[i], pfd_size);
	}

	if (g_pollfds) free(g_pollfds);
	g_pollfds = out_fds;
	g_nfds   -= 1;
	return g_nfds;
}/*}}}*/


/*
 * function:    等待监听IO事件发生
 * @param [ in] timeout 超时时间
 * @param [ in] call IO事件回调函数
 * @return      0:正常  <0:出错
 */
int iopollWait(int timeout, iopoll_call_t call)
{/*{{{*/
	int ret = poll(g_pollfds, g_nfds, timeout);
	TRY_PERROR(ret==-1, "poll():", return FuncError);


	int pfd_size               = sizeof(struct pollfd);
	int worknfds               = g_nfds;
	struct pollfd *workpollfds = (struct pollfd*)malloc(pfd_size * worknfds);
	memcpy(workpollfds, g_pollfds, pfd_size * worknfds);

	int fd      = 0;
	int revents = 0;
	int i       = 0;
	for (i=0; i<worknfds; i++)
	{
		fd      = workpollfds[i].fd;
		revents = workpollfds[i].revents;

		if (revents == 0)
			continue;

		call(fd, revents);
	}

	free(workpollfds);
	return FuncNormal;
}/*}}}*/

/*
 * function:    释放poll内存
 */
void iopollDistroy()
{/*{{{*/
	if (g_pollfds) free(g_pollfds);
}/*}}}*/





