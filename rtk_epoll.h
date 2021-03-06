//
// Created by nins on 2022/3/15.
//

#ifndef RTKPP_RTK_EPOLL_H
#define RTKPP_RTK_EPOLL_H


#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>  //struct sockaddr_in

#include "stdafx.h"
#include "rtk_request.h"
#include "rtk_response.h"
#include "rtk_threadpool.h"
#include "utils.h"
#include "rtk_timer.h"

#define MAXEVENTS 1024

//extern struct epoll_event* events_list;

class rtk_epoll{
public:
    epoll_event events_list[MAXEVENTS];

    std::mutex add_mutex;
    int epoll_fd;
public:

    void accept_connection(int linsten_fd,int epoll_fd,std::string path,rtk_timer* timer);
    void do_request(rtk_request* rq,rtk_timer* timer);


    //处理epoll队列
    int rtk_epoll_create(int flag);
    int rtk_epoll_add(int epoll_fd,int fd,rtk_request* rq,int events);
    int rtk_epoll_mod(int epoll_fd,int fd,rtk_request* rq,int events);
    int rtk_epoll_wait(int epoll_fd,int max_events,int timeout);

    void distribute_events(int epoll_fd,int listen_fd,int events_num,std::string path
           ,rtk_threadpool* tp,rtk_timer* timer);


};


#endif //RTKPP_RTK_EPOLL_H
