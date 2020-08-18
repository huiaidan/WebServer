#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H
#include<string>
#include<memory>
class TcpConnection;
using namespace std;
class HttpResponse
{
public:
	HttpResponse()
	{}
	~HttpResponse()
	{}
	//void send_file(int newfd,string file);//是文件的话，将文件内容返回给浏览器
        void send_file(shared_ptr<TcpConnection> conn,int ofd,struct stat st,string file,string way);//是文件的话，将文件内容返回给浏览器
        void send_dentry(int newfd,string file,string way);//是目录的话，将目录返回给浏览器
        void send_response_head(int cfd, int no, const char* desp, const char* type, long len);//发送hppt协议头部给浏览器
	void send_response_error(shared_ptr<TcpConnection> conn);
        const char *get_file_type(const char *name);
        void encode_str(char* to, int tosize, const char* from);
	
private:
};


#endif
