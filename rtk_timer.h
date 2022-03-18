//
// Created by nins on 2022/3/18.
//

#ifndef RTKPP_RTK_TIMER_H
#define RTKPP_RTK_TIMER_H

#include <time.h>
#include <sys/time.h>

#include "stdafx.h"
#include "rtk_request.h"

class rtk_timer{
public:
    typedef std::function<void()> time_handle_func;

    rtk_timer();
    ~rtk_timer();
public:
    struct time_node{
        size_t timeout_val;
        bool delected;
        time_handle_func handler;
        rtk_request* request;
    };

    std::priority_queue<time_node*> time_queue;
    size_t rtk_current_timer;  //当前时间

public:
    int rtk_find_timer();
    //加入超时判断优先队列
    void rtk_add_timer(rtk_request* rq,size_t timeout,time_handle_func handler);
    void rtk_del_timer(rtk_request* rq,size_t timeout,time_handle_func handler);

    //处理超时时间
    void rtk_handle_expire_time();

private:
    void rtk_timer_update();

};



#endif //RTKPP_RTK_TIMER_H
