#include"Server.h"
#include"TcpConnection.h"
#include"Acceptor.h"
#include"Base/Socket.h"
#include"Base/TaskQueue.h"
#include"EventLoop.h"
#include"Base/TimerFd.h"
#include"Channel.h"
#include<unistd.h>

using namespace std;
Server::Server(EventLoop* loop_):
	loop(loop_),
	task_queue(new TaskQueue()),
	eventpool(EVENTLOOPTHREADNUM,task_queue),
	pool(6,task_queue),
	sock(new Socket()),
        acpt(new Acceptor(this,loop)),
	tfd(new TimerFd()),
	listenfd(-2),
	timerfd(-2),
	wakeupfd(-2)
	{}

Server::~Server()
{
	cout<<"进入Server的析构函数了"<<endl;
}
void Server::start()
{
	eventpool.start(loops);
	if(EVENTLOOPTHREADNUM>0)
		this->acpt->accept_start(this->loops);//将eventloop指针传给Acceptor
	pool.start();
}
	
vector<EventLoop*>Server:: Get_vector()
{
	return this->loops;
}

void Server:: init()
{
	this->sock->do_listen(this->listenfd);
	this->tfd->create_timerfd(this->timerfd);
	shared_ptr<TcpConnection> conn(new TcpConnection(this->listenfd,this,acpt,task_queue,this->tfd->get_timerqueue()));
	Map.insert(make_pair(listenfd,conn));
	loop->get_channel()->lfd_event_init(conn);
	shared_ptr<TcpConnection> conn_(new TcpConnection(this->timerfd,this,acpt,task_queue,this->tfd->get_timerqueue()));
	Map.insert(make_pair(timerfd,conn_));
	loop->get_channel()->tfd_event_init(conn_);
}

shared_ptr<TcpConnection> Server::newfd_conn(int fd)
{
	shared_ptr<TcpConnection> conn(new TcpConnection(fd,this,acpt,task_queue,this->tfd->get_timerqueue()));
	Map.insert(make_pair(fd,conn));
	return conn;
}

void Server::close_newfd_conn(int fd)
{
	Map.erase(fd);
}
