//
// Created by nins on 2022/3/15.
//

#ifndef RTKPP_RTK_THREADPOOL_H
#define RTKPP_RTK_THREADPOOL_H

#include "stdafx.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "rtk_response.h"
#include "rtk_request.h"


//struct task{
//    void* arg;
//    void (*func)(void*);
//};


class rtk_threadpool{
public:
    ///c++11
    typedef std::function<void()> task;

    rtk_threadpool();
    ~rtk_threadpool();

public:
    size_t initnum;
    //线程数组
    std::vector<std::thread> threads;


    //任务队列
    std::queue<task> task_queue;

    //互斥锁条件变量
    std::mutex _mutex;
    std::condition_variable cond;

    ///结束工作标志位-线程池工作结束时为真
    bool done;

    //任务队列是否为空/满
    bool isEmpty;
    bool isFull;

public:
    void addTask(const task &f);
    void start(int num);
    void setSize(int num);
    void runTask();
    void finish();
};




#endif //RTKPP_RTK_THREADPOOL_H
