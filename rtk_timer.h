//
// Created by nins on 2022/3/18.
//

#ifndef RTKPP_RTK_TIMER_H
#define RTKPP_RTK_TIMER_H

#include <time.h>
#include <sys/time.h>
#include <functional>
#include <mutex>

#include "stdafx.h"
#include "rtk_request.h"

//默认500ms过期时间
#define TIMEOUT_DEFAULT 500

class rtk_timer{
public:
    typedef std::function<void()> time_handle_func;

    struct time_node{
        size_t timeout_val;
        bool delected;
        rtk_request* request;
        time_handle_func handler;


        bool operator<(const time_node &a) const{
            return timeout_val > a.timeout_val;  //小顶堆
        };
    };

    std::unordered_map<rtk_request*,std::shared_ptr<time_node>> request_to_timer; //存储request到time_node的对应关系

    rtk_timer();
    ~rtk_timer();
public:


    std::priority_queue<std::shared_ptr<time_node>> time_queue;
    size_t rtk_current_timer;  //当前时间

    //std::mutex q_mutex; //protect queue


public:
    int rtk_find_timer();
    //加入超时判断优先队列
    void rtk_add_timer(rtk_request* rq,size_t timeout,time_handle_func handler);
    void rtk_del_timer(rtk_request* rq);

    //处理超时时间
    void rtk_handle_expire_time();

private:
    void rtk_timer_update();

};



#endif //RTKPP_RTK_TIMER_H
