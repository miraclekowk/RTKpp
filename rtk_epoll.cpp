//
// Created by nins on 2022/3/15.
//

#include "rtk_epoll.h"

struct epoll_event* events_list;

int rtk_epoll::rtk_epoll_create(int flag) {
    //flag是0时可以关闭fork之后子进程无用的fd
    int epoll_fd = epoll_create1(flag);
    if(epoll_fd == -1)
        return -1;

    events_list = new epoll_event; //为感兴趣事件申请空间
    return epoll_fd;
};

int rtk_epoll::rtk_epoll_add(int epoll_fd, int fd, rtk_request rq, int events) {
    struct epoll_event ev;
    ev.data.ptr = &rq;
    ev.events = events;
    int res = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev);
    if(res = -1)
        return -1;
    return 0;
}

int rtk_epoll::rtk_epoll_mod(int epoll_fd, int fd, rtk_request rq, int events) {
    struct  epoll_event ev;
    ev.data.ptr = &rq;
    ev.events = events;
    int res = epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&ev);
    if(res = -1)
        return -1;
    return 0;
}

int rtk_epoll::rtk_epoll_wait(int epoll_fd, struct epoll_event *events_list, int max_events, int timeout) {
    int active_events =  epoll_wait(epoll_fd,events_list,max_events,timeout);
    return active_events;
}

void rtk_epoll::do_request(rtk_request rq,rtk_response rsp){
    //用户态缓冲区 request->buf的指针
    char* plast;
    size_t remain_size;
    int n_read; //实际读取数量
    int parse_return; //解析函数返回的结果
    std::string filename;  //请求资源的绝对路径
    struct stat sbuf;  //用来存放文件stat信息的结构体

    while(1){
        // plast指向缓冲区buf当前可写入的第一个字节位置，取余是为了实现循环缓冲
        plast = &rq.buff[rq.last % MAX_BUF];

        // remain_size表示缓冲区当前剩余可写入字节数
        remain_size = std::min(MAX_BUF - (rq.last - rq.pos) - 1, MAX_BUF - rq.last % MAX_BUF);

        n_read = read(rq.fd,plast,remain_size);
        //读到文件结束符或没有可读数据  断开tcp连接
        if(n_read == 0){
            goto err;
        }else if(n_read < 0 && (errno != RTK_AGAIN)){
            goto err;
        }else if(n_read < 0 && (errno == RTK_AGAIN)){
            break;//若read返回一个EAGAIN，则说明当前包读完，但是没见到EOF,重新去epoll中注册(即保持了连接)等待新连接包到达
        }

        rq.last += n_read; //读到n个字符，移动buf内读到的指针


        parse_return = rq.parse_request_line();
        if(parse_return == RTK_AGAIN){
            continue; //读到EAGAIN,说明buf中有的都解析完了，但line还没结尾，while继续去fd里面拉取
        }else if(parse_return != 0){
            goto err;  //return 0时，是正常解析到结尾，不为0则异常
        }

        parse_return = rq.parse_request_body();
        if(parse_return == RTK_AGAIN){
            continue;  ///这里continue其实会有点问题，但因为buf足够大不会有分段解析的问题
        }else if(parse_return != 0){
            goto err;
        }

        filename = rq.parse_uri();
        ///这里不理解为啥要继续尝试? 因此改成直接出去断连
        if(rsp.error_file_path(&sbuf,filename,rsp.fd)){
            //continue;  //找不到文件，可能是fiename文件解析问题，继续尝试
            goto err;
        }
        //形成正常响应头
        rsp.rtk_http_handle_head(rq);

        rsp.serve_static(rq,filename,sbuf.st_size);

        if(!rsp.keep_alive)
            goto close;
    }
    //一次请求响应结束后不立即断开连接，对应一个文件需要多个http包的场景，等待后续请求包
    rtk_epoll_mod(rq.epoll_fd,rq.fd,rq,(EPOLLIN | EPOLLET | EPOLLONESHOT));
    //rtk_add_timer();
    return;

    err:
    close:
    rq.RTK_close();
};


void rtk_epoll::distribute_events(int epoll_fd, int listen_fd, struct epoll_event *events_list, int events_num, std::string path,rtk_response rsp,rtk_threadpool tp) {
    for(int i = 0;i < events_num;i++){
        rtk_request* rq = static_cast<rtk_request *>(events_list[i].data.ptr);
        int fd = events_list[i].data.fd;

        if(fd == listen_fd){
            accpet_new_connection();
        }else{
            //error is like 0x008,use &,if the result is 1,indicate events is belong to this type of error
            //EPOLLHUP -- 文件被挂断
            //EPOLLERR --文件上发生了一个错误。这个事件是一直监控的即使没有明确指示
            //EPOLLIN -- 可读
            if((events_list[i].events & EPOLLERR)  || (events_list[i].events & EPOLLHUP) || (!events_list[i].events & EPOLLIN)){
                close(fd);
                continue;  //处理异常事件，处理方法就是关闭fd且跳过
            }

//            void(rtk_response:: *func)(rtk_request);
//            func = &rtk_response::do_request;
            rtk_threadpool::task tsk = std::bind(do_request, rq, rsp);  ///大概bind把epoll类的this绑进去了？ 如果改动response那边会交叉引用
            tp.addTask(tsk);
        }
    }
}