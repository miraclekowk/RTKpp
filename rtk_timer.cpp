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

        //std::shared_ptr<time_node> tm = time_queue.top();
        //若已经删除，则此节点pop
        if(time_queue.top()->delected){
            //没删除且超时了，调用handle指定的函数来执行处理 目前主要是关闭连接释放request内存
            if(time_queue.top()->handler){
                time_queue.top()->handler();
            }
            time_queue.pop();
            continue;
        }
        //队列最小时间和当前时间的差值，如果正数则说明整个队列都还没超时
        time = (int)(time_queue.top()->timeout_val - rtk_current_timer);
        time = (time > 0)? time : 0;
        break;
    }
    return time;
}

void rtk_timer::rtk_add_timer(rtk_request *rq, size_t timeout, time_handle_func handler) {
    rtk_timer_update();

    //time_node* tm = new time_node(rtk_current_timer + timeout, false,rq,handler);

    //auto tm_ptr = std::make_shared<time_node>(tm);
    std::shared_ptr<time_node> tm_ptr(new time_node(rtk_current_timer + timeout, false,rq,handler));

    //这里发生拷贝，shared_ptr引用加一
    request_to_timer[rq] = tm_ptr;

    //进入queue，shared_ptr引用加一，离开本函数后该计数器应当为2
    time_queue.push(tm_ptr);
}

void rtk_timer::rtk_del_timer(rtk_request *rq) {
    //std::unique_lock<std::mutex> lock(q_mutex);
    rtk_timer_update();
    ///降速太严重..？ 待测
//    if(request_to_timer.find(rq) == request_to_timer.end())
//        return -1;

    ///!!! need to  quickly find tm_ptr,so time_queue have to cache pointer instead of node
    request_to_timer[rq]->delected = true; //惰性删除，先只改变标记位，rtk_handle_expire_time()中统一删
    ///效率可能很有影响,后续优化
    request_to_timer.erase(rq);  //reduce time_node shared_ptr reference 删后只剩queue中的一份shared_ptr了

}

void rtk_timer::rtk_handle_expire_time() {
    while(!time_queue.empty()){
        rtk_timer_update();
        std::weak_ptr<time_node> wk_ptr = time_queue.top(); //取得最早的时间
        //如果此节点已经删除
        if(time_queue.top()->delected){
            time_queue.pop(); //删掉queue中的time_node 按理说自动释放内存了
            continue;
        }

        //最早入队的超时时间都还没到，结束超时检查
        if(time_queue.top()->timeout_val > rtk_current_timer){
            return;
        }
        //没删除且超时了，调用handle指定的函数来执行处理
        if(time_queue.top()->handler){
            time_queue.top()->handler();  //调用request::close()删除掉rq
        }
        time_queue.pop();//处理完time_node也要删除

    }
}