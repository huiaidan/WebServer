#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H
#include<string>
#include<iostream>
#include<stdio.h>
#include<memory>
#include"HttpResponse.h"
class TcpConnection;
using namespace std;
class HttpContext
{
public:
	HttpContext()
	{}
	~HttpContext()
	{}
	void file_state(shared_ptr<TcpConnection> conn,string file,string way);//判断文件是否存在
	void file_or_dentry(shared_ptr<TcpConnection> conn,struct stat &sbuf,string file,string way);//判断是文件还是目录
	void do_analyse(shared_ptr<TcpConnection> conn,string way,string file,string verssion);
private:
	HttpResponse* responser;
};

#endif
