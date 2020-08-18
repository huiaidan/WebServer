#ifndef _SERVER_H
#define _SERVER_H
#include<vector>
#include<map>
#include"Base/Data.h"
#include"EventLoopThreadPool.h"
#include"Base/ThreadPool.h"
using namespace std;
class Acceptor;
class TaskQueue;
class EventLoop;
class TcpConnection;
class TimerFd;
class Socket;
class Server
{
public:
	Server(EventLoop* loop_);
	~Server();
	
	void start();
	vector<EventLoop*> Get_vector();
	void init();
        shared_ptr<TcpConnection> newfd_conn(int fd);
        void close_newfd_conn(int fd);
	int get_lfd()
	{
		return this->listenfd;
	}
	int get_tfd()
	{
		return this->timerfd;
	}
private:
	EventLoop* loop;
	TaskQueue* task_queue;
	EventLoopThreadPool eventpool;
	ThreadPool pool;
	vector<EventLoop*> loops;
	Socket* sock;
	Acceptor* acpt;
	TimerFd* tfd;
	int listenfd;
	int timerfd;
	int wakeupfd;
	map<int,shared_ptr<TcpConnection>> Map;
};

#endif
