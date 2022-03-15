//
// Created by nins on 2022/3/15.
//

#include "rtk_epoll.h"

struct epoll_event* events;

int rtk_epoll::rtk_epoll_create(int flag) {
    //flag是0时可以关闭fork之后子进程无用的fd
    int epoll_fd = epoll_create1(flag);
    if(epoll_fd == -1)
        return -1;

    events = new epoll_event; //为感兴趣事件申请空间
    return epoll_fd;
};

int rtk_epoll::rtk_epoll_add(int epoll_fd, int fd, rtk_request rq, int events) {


}

int rtk_epoll::rtk_epoll_mod(int epoll_fd, int fd, rtk_request rq, int events) {


}

int rtk_epoll::rtk_epoll_wait(int epoll_fd, int fd, rtk_request rq, int events) {


}

void rtk_epoll::distribute_events(int epoll_fd, int listen_fd, struct epoll_event *events, int events_num, std::string path,rtk_threadpool tp) {


}