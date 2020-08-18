#include"Epoll.h"
#include"error.h"
#include"Base/Data.h"
using namespace std;
//创建监听树以及epollfd
void Epoll::epfd_create(int &epollfd)
{
	epollfd=epoll_create(MAXSIZE);
	if(epollfd==-1)
		perr_exit("epfd_create error");
}

//将fd加入监听树进行监听
void Epoll::epfd_add(int fd,struct epoll_event* event)
{
        if(this->epfd==-2)
	{
		epfd_create(this->epfd);
	}
	MutexLockGuard lock(Lock);
	int ret=epoll_ctl(this->epfd,EPOLL_CTL_ADD,fd,event);
	if(ret==-1)
		perr_exit("epfd_add error");
}

//修改监听树上的fd的监听事件类型
void Epoll::epfd_mod(int fd,struct epoll_event* event)
{
	MutexLockGuard lock(Lock);
	int ret=epoll_ctl(this->epfd,EPOLL_CTL_MOD,fd,event);
	if(ret==-1)
		perr_exit("epfd_add error");
}

//将fd从监听树上删除
void Epoll::epfd_del(int fd)
{
	MutexLockGuard lock(Lock);
	int ret=epoll_ctl(this->epfd,EPOLL_CTL_DEL,fd,NULL);
	if(ret==-1)
		perr_exit("epfd_del error");
}

//监听监听树上的fd
void Epoll::do_epoll_wait(int &n,struct epoll_event* event)
{
	n=epoll_wait(epfd,event,MAXSIZE,-1);
	if(n==-1&&errno!=EINTR)
		perr_exit("do_epoll_wait error");
}
