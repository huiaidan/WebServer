#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<cstring>
#include<string>
#include<dirent.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"HttpResponse.h"
#include"TcpConnection.h"
#include"error.h"
using namespace std;

void HttpResponse::send_file(shared_ptr<TcpConnection> conn,int ofd,struct stat st,string file,string way)//是文件的话，将文件内容返回给浏览器
{
	int ret=0,n=0;
	send_response_head(conn->get_fd(), 200, "OK", get_file_type(file.c_str()), st.st_size);
	if(way=="HEAD")
		return;
	lseek(ofd,conn->get_lseek(),SEEK_SET);
	char buf[118096];
	while((n=read(ofd,buf,sizeof(buf)))>0){
		while(1){
		ret=send(conn->get_fd(),buf,n,0);
		if(ret==-1)
		{
			if(errno==EAGAIN||errno==EINTR)
			{
				off_t len=lseek(ofd,0,SEEK_CUR);
				conn->set_lseek(ofd,len-n);
				conn->listen_write();
				return;
			}
			perr_exit("send error");
		}
		if(ret==0)
		{
			conn->shut();
			conn->conn_del();
			return;
		}
		if(ret<n)
		{
			n=n-ret;
			off_t len=lseek(ofd,0,SEEK_CUR);
			conn->set_lseek(ofd,len-n);
			conn->listen_write();
			return;
		}
		if(ret==n)
			break;
		}
	}	
        conn->set_lseek(-1,0);	
}

void HttpResponse::send_dentry(int newfd,string file,string way)//是目录的话，将目录返回给浏览器
{
	char buf[8096];	
	if(way=="HEAD")
	{
	        send_response_head(newfd, 200, "OK", get_file_type(".html"), -1);
		return;
	}
	struct dirent** ptr;
	sprintf(buf, "<html><head><title>项目测试</title></head>");
	sprintf(buf+strlen(buf), "<body><h1>目录：</h1><table>");
	int n=scandir(file.c_str(),&ptr,NULL,alphasort);
	for(int i=2;i<n;i++){
		char path[200];
		char enstr[200];
        	char *name=ptr[i]->d_name;
		sprintf(path, "%s/%s", file.c_str(), name);
		struct stat st;
		stat(path, &st);
		encode_str(enstr, sizeof(enstr), name);
		if(S_ISREG(st.st_mode)) {       
			sprintf(buf+strlen(buf), 
					"<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
					enstr, name, (long)st.st_size);
		} else if(S_ISDIR(st.st_mode)) {		// 如果是目录       
			sprintf(buf+strlen(buf), 
					"<tr><td><a href=\"%s/\">%s/</a></td><td>%ld</td></tr>",
					enstr, name, (long)st.st_size);
		}
		
	}
	sprintf(buf+strlen(buf), "</table></body></html>");
	send_response_head(newfd, 200, "OK", get_file_type(".html"), strlen(buf));
	send(newfd, buf, strlen(buf),0);
}

void HttpResponse::send_response_error(shared_ptr<TcpConnection> conn)//发送hppt协议错误信息给浏览器
{
	char buf[1024];
	send_response_head(conn->get_fd(), 404, "Not Found", get_file_type(".html"), -1);
	sprintf(buf, "<html><head><title>404 Not Found</title></head></html>");
	send(conn->get_fd(), buf, strlen(buf), 0);
}

//发送hppt协议头部给浏览器
void HttpResponse::send_response_head(int newfd, int no, const char* desp, const char* type, long len)
{
	char buf[1024] = {0};
		//状态行
	sprintf(buf, "http/1.1 %d %s\r\n", no, desp);
	     // 消息报头
	sprintf(buf+strlen(buf), "Content-Length:%ld\r\n", len);
	//sprintf(buf+strlen(buf), "Transfer-Encoding:chunked\r\n");
	sprintf(buf+strlen(buf), "Connection: keep-alive\r\n");
        sprintf(buf+strlen(buf), "Content-Type:%s\r\n", type);
	write(newfd, buf, strlen(buf));
	write(newfd, "\r\n", 2);
}


const char * HttpResponse::get_file_type(const char *name)
{
    const char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = strrchr(name, '.');   
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0||strcmp(dot,".ico")==0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}

void HttpResponse::encode_str(char* to, int tosize, const char* from)
{
	int tolen;

	for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) {    
		if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0) {      
			*to = *from;
			++to;
			++tolen;
		} else {
			sprintf(to, "%%%02x", (int) *from & 0xff);
			to += 3;
			tolen += 3;
		}
	}
	*to = '\0';
}
