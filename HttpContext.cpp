#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string>
#include<iostream>
#include"HttpContext.h"
#include"error.h"
#include"Base/Data.h"
#include"TcpConnection.h"
using namespace std;
void HttpContext::file_state(shared_ptr<TcpConnection> conn,string file,string way)//判断文件是否存在
{
	struct stat sbuf;
	int ret=stat(file.c_str(),&sbuf);
	if(ret!=0){//说明不存在文件；
		responser->send_response_error(conn);//发送错误提示给浏览器
                conn->shut();	       
	        conn->conn_del();
		return;
	}
	file_or_dentry(conn,sbuf,file,way);
}

void HttpContext::file_or_dentry(shared_ptr<TcpConnection> conn,struct stat &sbuf,string file,string way)//判断是文件还是目录
{
	if(S_ISREG(sbuf.st_mode)){//是普通文件
		int ofd=open(file.c_str(),O_RDONLY);
		responser->send_file(conn,ofd,sbuf,file,way);
	}
	else if(S_ISDIR(sbuf.st_mode)){//是目录
		responser->send_dentry(conn->get_fd(),file,way);
	}
	else ;

}
//分析请求方法
void HttpContext::do_analyse(shared_ptr<TcpConnection> conn,string way,string file,string verssion)
{
	if(way=="GET"||way=="HEAD"){
		file_state(conn,file, way);
	}
	else{
	       responser->send_response_error(conn);//发送错误提示给浏览器
               conn->shut();	       
	       conn->conn_del();
	}
}
