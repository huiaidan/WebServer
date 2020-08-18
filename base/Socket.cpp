#include<iostream>
#include<sys/types.h>
#include<ctype.h>
#include"error.h"
#include"Socket.h"

using namespace std;

void Socket::socket_init()
{
	this->serv_addr.sin_family=AF_INET;
	this->serv_addr.sin_port=htons(SERV_PORT);
	this->serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
}

void Socket::listenfd_create()
{
	int opt=1;
	this->listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(this->listenfd==-1)
		perr_exit("socket_create error");
	setsockopt(this->listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
}

void Socket::socket_bind()
{
	int ret=bind(this->listenfd,(struct sockaddr *)&this->serv_addr,sizeof(this->serv_addr));
	if(ret==-1)
		perr_exit("socket_bind error");

}

void Socket::socket_listen()
{
	int ret=listen(this->listenfd,MAXSIZE);
	if(ret==-1)
		perr_exit("socket_listen error");
}

void Socket::do_listen(int& lfd)
{
	socket_init();
	listenfd_create();
	socket_bind();
	socket_listen();
	lfd=this->listenfd;
}
