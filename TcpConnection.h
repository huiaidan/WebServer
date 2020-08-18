#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H
#include"Base/TimerQueue.h"
#include"HttpRequest.h"
#include"Base/TaskQueue.h"
#include"Timer.h"
#include<iostream>
#include<map>
using namespace std;
class TimerQueue;
class Server;
class HttpRequest;
class TaskQueue;
class Acceptor;
class Channel;
class TcpConnection:public enable_shared_from_this<TcpConnection>
{
public:
	typedef boost::function<void()> func;
	TcpConnection(int fd_,Server* server_,Acceptor* acpt_,TaskQueue* task_buf_,TimerQueue* tq_);
	~TcpConnection();
	void myread();
	void mywrite();
        void tfd_action();
        void lfd_action();
        void do_raction(weak_ptr<TcpConnection>);
        void do_waction(weak_ptr<TcpConnection>);
	MutexLock& get_lock();
	void conn_del();
	void conn_del(weak_ptr<TcpConnection> live);
        void listen_write();
        void set_timer(Channel* ch);
        void set_timer();
	void set_time(int64_t time_)
	{
		time=time_;
	}
	bool get_shutdown()
	{
		return ShutDown;
	}
	void shut()
	{
		ShutDown=1;
	}
	void set_lseek(int fd,off_t len)
	{
		ls=len;
		ofd=fd;
	}
        off_t get_lseek()
	{
		return ls;
	}
	shared_ptr<TcpConnection> get_this()
	{
		return shared_from_this();
	}
	HttpRequest* get_request()
	{
		return this->request;
	}
	int get_fd()
	{
		return this->newfd;
	}
	TimerQueue* get_tq()
	{
		return this->tq;
	}
	TaskQueue* get_taskqueue()
	{
		return this->task_buf;
	}
	bool get_stat()
	{
		return stat;
	}
	void set_stat(bool i)
	{
		stat=i;
	}
private:
	int newfd;
	int64_t time;
	bool ShutDown;
	off_t ls;
	int ofd;
	bool stat;//表示是否在epoll树上进行监听
	Server* server;
	TimerQueue* tq;
	HttpRequest* request;
	Acceptor* acpt;
	TaskQueue* task_buf;
        MutexLock conn_mutex;
	Channel* channel;
	Timer timer;
};

#endif
