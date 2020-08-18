#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H
#include<vector>
#include<boost/bind.hpp>
using namespace std;
class EventLoop;
class Channel;
class Server;
class TcpConnection;
class Acceptor
{
public:
	Acceptor(Server* server_,EventLoop* loop_);
	~Acceptor()
	{}
	int  do_accept(int listenfd);//产生newfd,之后再调用channel的函数初始化事件，接着再调用epoll的函数将事件加入到epfd红黑树
        void NewConnectionCallback();
	void accept_start(vector<EventLoop*> loops_);
private:
	int i;
	Server* server;
	vector<EventLoop*> loops;
	EventLoop* loop;
	int idleFd_;
};

#endif
