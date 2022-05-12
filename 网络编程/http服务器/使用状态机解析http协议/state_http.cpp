#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>

const int BUFFER_SIZE = 4096;


void headers(int client_sock,struct stat& statbuff){		//发送响应头部
	char buf[1024] = {0};
	char tem[64];
	//struct stat st;
	int fileid = 0;
	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Server: MY Server\r\n");
	strcat(buf,"Content-Type: text/html\r\n");
	strcat(buf,"Connect: Close\r\n");
	
	//文件长度保存完在st.st_size
	snprintf(tem,64,"Content-Length: %d\r\n\r\n",statbuff.st_size);
	strcat(buf,tem);
	fprintf(stdout,"headers :%s\n",buf);
	send(client_sock,buf,strlen(buf),0);
} 






//主状态机的两种状态
enum CHECK_STATE {CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER};

//从状态机的两种状态，读取到完整行，行出错，行不完整需要继续读取
enum LINE_STATUS {LINE_OK = 0, LINE_BAD, LINE_OPEN};

//处理http的结果 NO_REQUEST-不完整的请求,GET_REQUEST-完整请求,BAD_REQUEST-错误请求,  无权限, 内部错误 , 客户端连接关闭
enum HTTP_CODE {NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION};

static const char* szret[] = {"I get a correct result\n", "Something wrong\n"};


//解析每一行
LINE_STATUS parse_line(char* buffer, int& checked_index, int& read_index){
    char temp;
    for(;checked_index<read_index;checked_index++){
        temp = buffer[checked_index];
        if(temp == '\r'){
            if((checked_index+1) == read_index)
                return LINE_OPEN;
            else if(buffer[checked_index+1] == '\n'){
                //printf("添加0了\n");
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }

            return LINE_BAD;
        }
        else if(temp == '\n'){
            if((checked_index>1) && buffer[checked_index-1] == '\r'){
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}


HTTP_CODE parse_requestline(char* temp, CHECK_STATE& checkstate){
    
    printf("请求行为:%s\n",temp);
    // Http请求必定带有空白字符 或者 \t字符 
    char* url = strpbrk(temp," \t");
    if(!url)
        return BAD_REQUEST;
    
    *url++  = '\0';

    char* method = temp;
    //只支持GET方法；
    if(strcasecmp(method,"GET") == 0)
        printf("收到GET请求\n");
    else 
        return BAD_REQUEST;

    url += strspn(url," \t");
    char* version = strpbrk(url," \t");
    if(!version)
        return BAD_REQUEST;
    *version++ = '\0';
    printf("版本号为:%s\n",version);
    //version +=strspn(version," \t");
    //printf("ban ben: %s\n",version);
    //只支持HTTP/1.1
    if(strcasecmp(version,"HTTP/1.1") != 0){
        printf("版本号不符合!\n");
        return BAD_REQUEST;
    }     
    //检查url完整性
    //printf("url为:%s\n",url);
    //if(strncasecmp(url,"http://",7) == 0){
        //url+=7;
        //url = strchr(url,'/');
    //}
    
    if(!url || url[0] != '/')
        return BAD_REQUEST;
    printf("The request URL is: %s\n", url);

    checkstate  = CHECK_STATE_HEADER;

    printf("收到完整的请求行\n");
    return NO_REQUEST;

}

HTTP_CODE parse_headers(char *temp,CHECK_STATE& checkstate){
    if(temp[0] == '\0'){
        printf("全部解析完毕\n");
        return GET_REQUEST;

    }  
    else if(strncasecmp(temp,"Host:",5) == 0){
        temp +=5;
        temp += strspn(temp," \t");
        printf("The request host is: %s\n",temp);
    }
    else
        printf("I can no handle this header\n");

    return NO_REQUEST;
}




HTTP_CODE parse_content(char* buffer,int& checked_index,CHECK_STATE& checkstate,int& read_index,int& start_line){
    LINE_STATUS linestatus = LINE_OK;
    HTTP_CODE retcode  = NO_REQUEST;

    while((linestatus = parse_line(buffer,checked_index,read_index)) == LINE_OK){
        
        char* temp = buffer + start_line;  //---------------------
        start_line = checked_index;

        switch(checkstate){
             case CHECK_STATE_REQUESTLINE:
                retcode = parse_requestline(temp,checkstate);
                if(retcode == BAD_REQUEST)
                    return BAD_REQUEST;
                break;
            case CHECK_STATE_HEADER:
                retcode = parse_headers(temp,checkstate);
                if(retcode == BAD_REQUEST)
                    return BAD_REQUEST;
                else if(retcode == GET_REQUEST)
                    return GET_REQUEST;
                break;
            default:
                return INTERNAL_ERROR;

        }   

    }
    if(linestatus == LINE_OPEN)
        return NO_REQUEST;

    return BAD_REQUEST;
}




int main(int argc,char* argv[]){
    if(argc<=2){
        std::cout<<"Wrong number of parameters"<<std::endl;
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in address;
    memset(&address,0x00,sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    //address.sin_addr.s_addr = htonl(ip);
    inet_pton(AF_INET,ip,&address.sin_addr);

    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);
    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof(client_address);
	int fd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);

    if(fd < 0)
		printf("errno is: %d\n", errno); 
    else{
		char buffer[BUFFER_SIZE];
		memset(buffer, '\0', BUFFER_SIZE);
		int date_read = 0;  //读到的数据
		int read_index = 0;     //读到哪里了
		int checked_index = 0;  //检查到哪里了
		int start_line = 0;     
        
        //初始化 主状态机的状态
        CHECK_STATE checkstatus = CHECK_STATE_REQUESTLINE;
        while(1){
            date_read = recv(fd,buffer+read_index,BUFFER_SIZE-1,0);
            if(date_read<0 && errno==EAGAIN || errno==EWOULDBLOCK){
                printf("数据读完了\n");
                break;
            }
            else if(date_read==0){
                printf("客户端断开连接!\n");
                break;
            }
            read_index += date_read;

            HTTP_CODE result = parse_content(buffer,checked_index,checkstatus,read_index,start_line);
            if(result == NO_REQUEST) //不完整的请求
                continue;
            else if(result == GET_REQUEST){
                printf("发送文件\n");
                struct stat statbuf;
                memset(&statbuf,0x00,sizeof statbuf);
                int fff = open("./source/index.html",O_RDONLY);
                fstat(fff,&statbuf);
                headers(fd,statbuf); //发送头部
                int len = sendfile(fd,fff,NULL,statbuf.st_size);
                if(len == statbuf.st_size)
                    printf("零拷贝发送成功\n");
                
                printf("文件大小为：%d\n",statbuf.st_size);
                //send(fd,reply,strlen(reply),0);
                break;
            }
            else{
                send(fd,szret[1],strlen(szret[1]),1);
                break;
            }

        }

        close(fd);

    }
    close(listenfd);
}