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
    int time;
    //循环删除可能已经删除掉的时间节点
    while(!time_queue.empty()){
        rtk_timer_update();

        time_node* tm = time_queue.top();
        //若已经删除，则此节点pop
        if(tm->delected){
            time_queue.pop();
            free(tm);
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
    rtk_timer_update();

    time_node tm;
    ///request->timer = tm;   需要优化 产生循环Include
    tm.timeout_val = rtk_current_timer + timeout;
    tm.delected = false;
    tm.handler = handler;
    tm.request = rq;

    time_queue.push(&tm);
}

void rtk_timer::rtk_del_timer(rtk_request *rq, size_t timeout, time_handle_func handler) {
    rtk_timer_update();
    ///...

}

void rtk_timer::rtk_handle_expire_time() {



}