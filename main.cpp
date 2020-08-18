#include<iostream>
#include"EventLoop.h"
#include"Server.h"
#include<unistd.h>
#include<iostream>
#include<sys/types.h>
using namespace std;

int main(void)
{
	EventLoop loop;
	Server server(&loop);
	server.init();
	server.start();
	loop.loop();
	return 0;
}
