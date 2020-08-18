
#include<sys/socket.h>
#include<arpa/inet.h>
#include"Data.h"
using namespace std;

class Socket
{
public:
	Socket(int lifd=-1):listenfd(lifd)
	{}
	~Socket()
	{}
	void socket_init();//初始化socket结构体serv_addr
	void listenfd_create();//创建listenfd;一个eventloop只有一个
	void socket_bind();//绑定listenfd到socket结构体
	void socket_listen();//设置listenfd的最大监听数量
	void do_listen(int& lfd);
private:
	int listenfd;
	struct sockaddr_in serv_addr;
};

