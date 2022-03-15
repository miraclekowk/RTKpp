//
// Created by nins on 2022/3/15.
//

#ifndef RTKPP_RTK_EPOLL_H
#define RTKPP_RTK_EPOLL_H


#include <sys/epoll.h>

#include "stdafx.h"
#include "rtk_request.h"
#include "rtk_threadpool.h"


class rtk_epoll{
public:
    //处理epoll队列
    int rtk_epoll_create(int flag);
    int rtk_epoll_add(int epoll_fd,int fd,rtk_request rq,int events);
    int rtk_epoll_mod(int epoll_fd,int fd,rtk_request rq,int events);
    int rtk_epoll_wait(int epoll_fd,int fd,rtk_request rq,int events);

    void distribute_events(int epoll_fd,int listen_fd,struct epoll_event* events,int events_num,std::string path,rtk_threadpool tp);


};


#endif //RTKPP_RTK_EPOLL_H
