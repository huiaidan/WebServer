#include"TcpConnection.h"
#include<boost/bind.hpp>
#include<boost/function.hpp>
#include"Base/Data.h"
#include"Base/TimerQueue.h"
#include"HttpRequest.h"
#include"Base/TaskQueue.h"
#include"Acceptor.h"
#include"Server.h"
#include"Channel.h"
#include<unistd.h>
typedef function<void()> test;
TcpConnection::TcpConnection(int fd_,Server* server_,Acceptor* acpt_,TaskQueue* task_buf_,TimerQueue* tq_)
		:newfd(fd_),
		time(0),
		ShutDown(0),
		ls(0),
		ofd(-1),
		stat(0),
		server(server_),
		tq(tq_),
		request(new HttpRequest()),
		acpt(acpt_),
		task_buf(task_buf_),
		conn_mutex(),
		channel(NULL),
		timer()
	{}
TcpConnection::~TcpConnection()
{
	cout<<"TcpConnection 析构了"<<endl;
	delete request;
	close(newfd);
}
//判断读fd的类型，确定相应的读操作
void TcpConnection::myread()
{
	if(this->newfd==server->get_lfd()){
		this->lfd_action();
		return;
	}
	if(this->newfd==server->get_tfd()){
		this->tfd_action();
		return;
	}
	weak_ptr<TcpConnection> live=get_this();
	test Test=boost::bind(&TcpConnection::do_raction,this,live);
        task_buf->put_into(Test);
}
void TcpConnection::mywrite()
{
	weak_ptr<TcpConnection> live=get_this();
	test Test=boost::bind(&TcpConnection::do_waction,this,live);
        task_buf->put_into(Test);
}
//定时器读事件的回调函数
void TcpConnection::tfd_action()
{
	tq->get_event();
}
//连接事件的回调函数
void TcpConnection::lfd_action()
{
	acpt->NewConnectionCallback();
}
//newfd读事件的回调函数，交给线程池中的线程执行
void TcpConnection::do_raction(weak_ptr<TcpConnection> live)
{
	shared_ptr<TcpConnection> conn;
	if((conn=live.lock())==NULL)
		return;
        this->get_request()->do_read(conn);
	if(ShutDown)
		return;
        set_timer();	
}

//newfd写事件的回调函数，交给线程池中的线程执行
void TcpConnection::do_waction(weak_ptr<TcpConnection> live)
{
	if(ofd==-1)
		return;
	MutexLockGuard lock(conn_mutex);
	shared_ptr<TcpConnection> conn;
	if((conn=live.lock())==NULL)
		return;
	int n=0,ret=0;
	lseek(ofd,ls,SEEK_SET);
	char buf[118024];
	while((n=read(ofd,buf,sizeof(buf)))>0){
		while(1){
		ret=send(conn->get_fd(),buf,n,0);
		if(ret==-1)
		{
			if(errno==EAGAIN||errno==EINTR)
			{
				off_t len=lseek(ofd,0,SEEK_CUR);
				conn->set_lseek(ofd,len-n);
				conn->listen_write();
				return;
			}
			perr_exit("send error");
		}
		if(ret==0)
		{
			conn->shut();
			conn->conn_del();
			return;
		}
		if(ret>0&&ret<n)
		{
			n=n-ret;
			off_t length=lseek(ofd,0,SEEK_CUR);
			conn->set_lseek(ofd,length-n);
			conn->listen_write();
			return;
		}
		if(ret==n)
			break;
		}
	}
        channel->revent_init(conn);	
        set_lseek(-1,0);
        set_timer();	
}

MutexLock& TcpConnection::get_lock()
{
	return conn_mutex;
}
//删除TcpConnection对象
void TcpConnection::conn_del(weak_ptr<TcpConnection> live)
{
	if((live.lock())==NULL)
		return;
	channel->get_ep()->epfd_del(newfd);
	channel->connmap_erase(newfd);
	server->close_newfd_conn(newfd);
}

//删除TcpConnection对象
void TcpConnection::conn_del()
{
	channel->get_ep()->epfd_del(newfd);
	channel->connmap_erase(newfd);
	server->close_newfd_conn(newfd);
}
//监听newfd的写事件
void TcpConnection::listen_write()
{
	channel->wevent_init(shared_from_this());
}
//设置定时器
void TcpConnection::set_timer(Channel* ch)
{
	channel=ch;
	weak_ptr<TcpConnection> live=get_this();
	timer=Timer();
	func task=boost::bind(&TcpConnection::conn_del,this,live); 
	timer.timer_init_s(get_this(),server->get_tfd(),30,task);
}
//设置定时器
void TcpConnection::set_timer()
{
	weak_ptr<TcpConnection> live=get_this();
	tq->pop(time);
	func task=boost::bind(&TcpConnection::conn_del,this,live); 
	timer.timer_init_s(get_this(),server->get_tfd(),30,task);
}
