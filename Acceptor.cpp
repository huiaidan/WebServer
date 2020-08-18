#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<memory>
#include"Acceptor.h"
#include"error.h"
#include"Base/Data.h"
#include"EventLoop.h"
#include"Channel.h"
#include"Server.h"
#include"TcpConnection.h"
using namespace std;
Acceptor::Acceptor(Server* server_,EventLoop* loop_):
		i(0),
		server(server_),
		loop(loop_),
		idleFd_(open("/dev/null", O_RDONLY | O_CLOEXEC))
	{}
void Acceptor::accept_start(vector<EventLoop*> loops_)
{
	this->loops=loops_;
}
int Acceptor::do_accept(int listenfd)
{
	struct sockaddr_in cli_addr;
	socklen_t cli_len=sizeof(struct sockaddr_in);
	int fd=accept(listenfd,(struct sockaddr *)&cli_addr,&cli_len);
	if(fd==-1)
	{
		if (errno == EMFILE)
			    {
				    close(idleFd_);
				    idleFd_ = accept(listenfd, NULL, NULL);
				    close(idleFd_);
				    idleFd_ = open("/dev/null", O_RDONLY | O_CLOEXEC);
			    }
		else
			perr_exit("do_accept error");
	}
	return fd;
}

void Acceptor::NewConnectionCallback()
{
	int newfd=do_accept(server->get_lfd());
	shared_ptr<TcpConnection> conn=server->newfd_conn(newfd);
	if(EVENTLOOPTHREADNUM<=0)
		loop->get_channel()->revent_init(conn);
	else{
		Channel* channel=loops[this->i]->get_channel();
		loops[this->i]->runInLoop(boost::bind(&Channel::revent_init,channel,conn));
		i++;
	if(i==EVENTLOOPTHREADNUM)
		this->i=0;}
}
