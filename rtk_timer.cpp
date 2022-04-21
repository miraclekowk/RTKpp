//
// Created by nins on 2022/3/18.
//

#include "rtk_timer.h"

void rtk_timer::rtk_timer_update() {
    //获取当前时间
    struct timeval tv;
    int rc = gettimeofday(&tv,NULL); ///参数解释：
    rtk_current_timer = (tv.tv_sec* 1000)+(tv.tv_usec / 1000);
}

rtk_timer::rtk_timer() {
    rtk_timer_update();
}

int rtk_timer::rtk_find_timer() {
    std::unique_lock<std::mutex> lock(q_mutex);
    int time;
    //循环删除可能已经删除掉的时间节点
    while(!time_queue.empty()){
        rtk_timer_update();

        auto tm = time_queue.top();
        //若已经删除，则此节点pop
        if(tm->delected){
            time_queue.pop();
            continue;
        }
        //最小时间节点和当前时间的差值，如果正数则说明都还没超时
        time = (int)(tm->timeout_val - rtk_current_timer);
        time = (time > 0)? time : 0;
        break;
    }
    return time;
}

void rtk_timer::rtk_add_timer(rtk_request *rq, size_t timeout, time_handle_func handler) {
    //std::unique_lock<std::mutex> lock(q_mutex);
    rtk_timer_update();

    time_node tm;

    tm.timeout_val = rtk_current_timer + timeout;
    tm.delected = false;
    tm.request = rq;
    tm.handler = handler;

    auto tm_ptr = std::make_shared<time_node>(tm);

    ///request->timer = tm;
    request_to_timer[rq] = tm_ptr;

    time_queue.push(tm_ptr);

}

void rtk_timer::rtk_del_timer(rtk_request *rq) {
    //std::unique_lock<std::mutex> lock(q_mutex);
    rtk_timer_update();
    ///降速太严重..？ 待测
//    if(request_to_timer.find(rq) == request_to_timer.end())
//        return -1;


    ///!!! need to  quickly find tm_ptr,so time_queue have to cache pointer instead of node
    std::shared_ptr<time_node> tm_ptr = request_to_timer[rq];
    tm_ptr->delected = true;
    request_to_timer.erase(rq);  //reduce time_node shared_ptr reference
}

void rtk_timer::rtk_handle_expire_time() {
    while(!time_queue.empty()){
        rtk_timer_update();
        std::shared_ptr<time_node> tm_ptr = time_queue.top(); //取得最早的时间
        //如果此节点已经删除
        if(tm_ptr->delected){
            time_queue.pop(); //pointer is pop,memory will also be release
            //free(&tm)
        }

        //最早的时间都还没到，结束超时检查
        if(tm_ptr->timeout_val > rtk_current_timer){
            return;
        }
        //没删除且超时了，调用handle指定的函数来执行处理
        if(tm_ptr->handler){
            tm_ptr->handler();
        }

        time_queue.pop();
        //free(&tm);
    }
}