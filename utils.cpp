//
// Created by nins on 2022/3/7.
//



#include "utils.h"

using namespace std;

bool str_valid_isdigit(string str){
    int n = str.length();
    if(n > 6) return false;
    for(int i = 0;i < str.length();i++){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}

int read_conf(string filename, RTK_config cfg){

    fstream infile;
    infile.open(filename,ios::in); //只读打开
    if(!infile)
        return RTK_CONF_ERROR;

    string line;
    while(getline( infile,line)){

        size_t found = line.find(DELIM);
        //如果找不到分隔符
        if(found == std::string::npos){
            return RTK_CONF_ERROR;
        }
        string key = line.substr(0,found);
        string value = line.substr(found+1);

        //c++的string是类，所以可以直接用==来比较  char*是指针，==在比较两指针是否相等
        if(key == "root"){
            cfg.root = value;
        }

        if(key == "port"){
            if(!str_valid_isdigit(value)){
                return RTK_CONF_ERROR;
            }
            cfg.port = stoi(value);
        }

        if(key == "thread_num"){
            if(!str_valid_isdigit(value)){
                return RTK_CONF_ERROR;
            }
            cfg.port = stoi(value);
        }
    }

    infile.close();
    return RTK_CONF_OK;
}

//屏蔽SIGPIPE信号，防止客户端断连后服务端异常关闭
void handle_for_sigpipe(){
    //struct sigaction* sa = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE,&sa,NULL))
        return;
}

int make_socket_no_blocking(int fd){
    //F_GETFL  获取文件状态标志
    int flag = fcntl(fd,F_GETFL,0);
    if (flag == -1)
        return -1;
    //固定写法
    flag |= O_NONBLOCK;
    if(fcntl(fd,F_SETFL,flag) == -1)
        return -1;
};

//socket()创建listen_fd并开始监听
int socket_bind_and_listen(int port){

    port = ((port <= 1024) || (port >= 65535) ) ? 6777:port;

    //AF_INET: protocol families  IPv4 Internet protocols
    //SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams. An out-of-band data transmission mechanism may be supported.
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd == -1)
        return -1;

    //消除bind时address already in used
    int optval = 1;
    if(setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(int)) == -1)
        return -1;

    struct sockaddr_in server_addr;
    //memset(&server_addr,0, sizeof(struct sockaddr_in));
    server_addr = {0};    //初始化
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if(bind(listen_fd,(struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
        return -1;

    //开始监听
    if(listen(listen_fd,LISTENQ) == -1)
        return -1;

    if(listen_fd == -1){
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}
