#ifndef _EPOLL_H
#define _EPOLL_H
#include<iostream>
#include<sys/epoll.h>
#include"Base/MutexLock.h"
using namespace std;
//围绕epfd进行操作
class Epoll
{
public:
	Epoll(int epfd_=-2):epfd(epfd_),Lock()
	{}
	~Epoll()
	{}
	//创建epfd以及对应的红黑树,也是一个eventloop只有一个;
        void epfd_create(int &epollfd);
	//向epfd对应的红黑树中添加监听fd 传入参数;
        void epfd_add(int fd,struct epoll_event* event);
        void epfd_mod(int fd,struct epoll_event* event);
	//......................删除......;传入参数
        void epfd_del(int fd);
	//对红黑树上的fd进行监听
        void do_epoll_wait(int &n,struct epoll_event* event);
private:
	int epfd;
	MutexLock Lock;
};

#endif
