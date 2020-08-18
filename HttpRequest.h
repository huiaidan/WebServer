#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H
#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include"HttpContext.h"
class TcpConnection;
using namespace std;
class HttpRequest
{
public:
	HttpRequest(int len_=0):len(len_),context(new HttpContext())
	{}
	~HttpRequest()
	{}
	void get_request_head(shared_ptr<TcpConnection> conn);//获取客户端发送过来的http请求协议的首行,并读走后面不用的几行；
	void request_classify(shared_ptr<TcpConnection> conn,char *buf); //获取请求方法，获取目标文件名，获取协议号;
        int get_line(int newfd, char *buf, int size);//获取一行
        void do_read(shared_ptr<TcpConnection> conn);	
private:
	int len;
	char  req_buf[1024];
	string way;
	string file;
	string verssion;
	HttpContext* context;
};


#endif
