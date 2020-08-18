#include<sys/types.h>
#include<sys/socket.h>
#include"HttpRequest.h"
#include"TcpConnection.h"
#include"Base/MutexLock.h"

using namespace std;
void HttpRequest::get_request_head(shared_ptr<TcpConnection> conn)//获取客户端发送过来的http请求协议的首行,并读走后面不用的几行；
{
	int n=0;
	MutexLockGuard lock(conn->get_lock());
	this->len=get_line(conn->get_fd(),this->req_buf,sizeof(this->req_buf));
	if(len==0)
	{
		conn->shut();
		conn->conn_del();
		return;
	}
	char buf[1024];
	while (1) {
	        n=read(conn->get_fd(),buf,sizeof(buf));
		if(n==-1)
		{
			if(errno!=EAGAIN&&errno!=EINTR)
			perr_exit("read error");
			break;
		}
	}	
}

void HttpRequest::request_classify(shared_ptr<TcpConnection> conn,char *buf) //获取请求方法，获取目标文件名，获取协议号 
{
	int t=0;
	for(int j=0;j<len;j++){
		if(buf[j]!=' ')
			break;
		t++;
	}
	int flag=0;
	file=file+"./zd";
	for(int i=t;i<this->len-1-t;i++){
		if(buf[i]==' '){
			if(buf[i-1]!=' ')
				flag++;
			continue;
		}
		switch(flag){
			case 0:
				this->way.push_back(buf[i]);
				break;
			case 1:
				this->file.push_back(buf[i]);
				break;
			case 2:
				this->verssion.push_back(buf[i]);
				break;
			default:
				break;
		}
	}
	{
	MutexLockGuard lock(conn->get_lock());
	context->do_analyse(conn,this->way,this->file,this->verssion);
	}
	this->file.clear();
	this->way.clear();
	this->verssion.clear();
}

int HttpRequest::get_line(int newfd, char *buf, int size)
{
	int i = 0;
        char c = '\0';
	int n;
       	while ((i < size-1) && (c != '\n')) {  
		n = recv(newfd, &c, 1, 0);
		if (n > 0) {     
			if (c == '\r') {       
			      	n = recv(newfd, &c, 1, MSG_PEEK);
		      		if ((n > 0) && (c == '\n')) {              
					recv(newfd, &c, 1, 0);
				} else {                       
					c = '\n';
				}
			}
			buf[i] = c;
			i++;
		} else {      
			c = '\n';
		}
    	}
	buf[i] = '\0';
    	if (-1 == n)
    	i = n;
       	return i;
}

void HttpRequest::do_read(shared_ptr<TcpConnection> conn)
{
	get_request_head(conn);
	if(conn->get_shutdown())
		return;
	request_classify(conn,this->req_buf);
}
