//
// Created by nins on 2022/3/7.
//

#ifndef RTKPP_UTILS_H
#define RTKPP_UTILS_H

//引入公有头
#include "stdafx.h"



#define DELIM "="
#define BUFLEN 6657   //读取配置文件缓冲区的大小

#define RTK_CONF_OK 0
#define RTK_CONF_ERROR -1

using namespace std;

struct RTK_config{
    string root;
    int port;
    int thread_num;
};


int read_conf(string filename, RTK_config cfg);
void handle_for_sigpipe();
int listen_socket_bind();
int make_socket_no_blocking(int fd);

void accept_connection(int listen_fd,int epoll_fd,string fd);



#endif //RTKPP_UTILS_H
