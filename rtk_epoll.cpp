//
// Created by nins on 2022/3/15.
//

#include "rtk_epoll.h"

//struct epoll_event* events_list;

int rtk_epoll::rtk_epoll_create(int flag) {
    //flag是0时可以关闭fork之后子进程无用的fd
    int epoll_fd = epoll_create1(flag);
    if(epoll_fd == -1)
        return -1;

    //events_list = (struct epoll_event*) malloc(sizeof(struct epoll_event) * MAXEVENTS); //为感兴趣事件申请空间
    return epoll_fd;
};

int rtk_epoll::rtk_epoll_add(int epoll_fd, int fd, rtk_request* rq, int events) {
    struct epoll_event ev;
    ev.data.ptr = (void*)rq;
    ev.data.fd = fd;
    ev.events = events;
    int res = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev);
    if(res == -1)
        return -1;
    return 0;
}

int rtk_epoll::rtk_epoll_mod(int epoll_fd, int fd, rtk_request* rq, int events) {
    struct  epoll_event ev;
    ev.data.ptr = rq;
    ev.events = events;
    int res = epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&ev);
    if(res = -1)
        return -1;
    return 0;
}

int rtk_epoll::rtk_epoll_wait(int epoll_fd, int max_events, int timeout) {
    int active_events =  epoll_wait(epoll_fd,events_list,max_events,timeout);
    return active_events;
}

void rtk_epoll::accept_connection(int listen_fd,int epoll_fd, std::string path,rtk_timer* timer){
    struct sockaddr_in client_addr;
    memset(&client_addr,0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = 0;

    //accpect_fd即真正用来传输数据的fd
    int accpet_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(accpet_fd == -1)
        perror("accept");

    int rc = make_socket_no_blocking(accpet_fd);
    rtk_request* rq = new rtk_request(path);

    //增加epoll活跃监听
    rtk_epoll_add(epoll_fd,accpet_fd,rq,(EPOLLIN | EPOLLET | EPOLLONESHOT));

    //刷新时间，往时间queue里加入节点  如果超时则届时调用request.close()
    timer->rtk_add_timer(rq,TIMEOUT_DEFAULT,std::bind(&rtk_request::RTK_close,rq));
};


void rtk_epoll::do_request(rtk_request* rq,rtk_timer* timer){
    //用户态缓冲区 request->buf的指针
    char* plast;
    size_t remain_size;
    int n_read; //实际读取数量
    int parse_return; //解析函数返回的结果
    std::string filename;  //请求资源的绝对路径
    struct stat sbuf;  //用来存放文件stat信息的结构体

    timer->rtk_del_timer(rq);

    while(1){
        // plast指向缓冲区buf当前可写入的第一个字节位置，取余是为了实现循环缓冲
        plast = &rq->buff[rq->last % MAX_BUF];

        // remain_size表示缓冲区当前剩余可写入字节数
        remain_size = std::min(MAX_BUF - (rq->last - rq->pos) - 1, MAX_BUF - rq->last % MAX_BUF);

        n_read = read(rq->fd,plast,remain_size);
        //读到文件结束符或没有可读数据  断开tcp连接
        if(n_read == 0){
            goto err;
        }else if(n_read < 0 && (errno != RTK_AGAIN)){
            goto err;
        }else if(n_read < 0 && (errno == RTK_AGAIN)){
            break;//若read返回一个EAGAIN，则说明当前包读完，但是没见到EOF,重新去epoll中注册(即保持了连接)等待新连接包到达
        }

        rq->last += n_read; //读到n个字符，移动buf内读到的指针


        parse_return = rq->parse_request_line();
        if(parse_return == RTK_AGAIN){
            continue; //读到EAGAIN,说明buf中有的都解析完了，但line还没结尾，while继续去fd里面拉取
        }else if(parse_return != 0){
            goto err;  //return 0时，是正常解析到结尾，不为0则异常
        }

        parse_return = rq->parse_request_body();
        if(parse_return == RTK_AGAIN){
            continue;  ///这里continue其实会有点问题，但因为buf足够大不会有分段解析的问题
        }else if(parse_return != 0){
            goto err;
        }

        filename = rq->parse_uri();

        rtk_response* rsp;  //开始根据request生成一个response
        ///这里不理解为啥要继续尝试? 因此改成直接出去断连
        if(rsp->error_file_path(&sbuf,filename,rq->fd)){
            //continue;  //找不到文件，可能是fiename文件解析问题，继续尝试
            goto err;
        }
        //形成正常响应头
        rsp->rtk_http_handle_head(*rq);

        rsp->serve_static(*rq,filename,sbuf.st_size);

        if(!rsp->keep_alive)
            goto close;
    }
    //一次请求响应结束后不立即断开连接，对应一个文件需要多个http包的场景，等待后续请求包
    rtk_epoll_mod(rq->epoll_fd,rq->fd,rq,(EPOLLIN | EPOLLET | EPOLLONESHOT));
    timer->rtk_add_timer(rq,TIMEOUT_DEFAULT,std::bind(&rtk_request::RTK_close,rq));
    return;

    err:
    close:
    rq->RTK_close();
};

void rtk_epoll::distribute_events(int epoll_fd, int listen_fd, int events_num,
        std::string path,rtk_threadpool* tp,rtk_timer* timer) {
    for(int i = 0;i < events_num;i++){

        rtk_request* rq = (rtk_request *)(events_list[i].data.ptr);  ///第一次建立连接是0 is ok
        int fd = events_list[i].data.fd;

        if(fd == listen_fd){
            accept_connection(listen_fd,epoll_fd,path,timer);
        }else{
            //error is like 0x008,use &,if the result is 1,indicate events is belong to this type of error
            //EPOLLHUP -- 文件被挂断
            //EPOLLERR --文件上发生了一个错误。这个事件是一直监控的即使没有明确指示
            //EPOLLIN -- 可读
            if((events_list[i].events & EPOLLERR)  || (events_list[i].events & EPOLLHUP) || (!events_list[i].events & EPOLLIN)){
                close(fd);
                continue;  //处理异常事件，处理方法就是关闭fd且跳过
            }

            ///store the result of a call to std::bind -- bind do_request and it's parameter
            auto tsk = std::bind(&rtk_epoll::do_request, this, rq,timer);  ///第一个参数是方法地址，第二个参数是拥有这个方法的实例
            tp->addTask(tsk);
        }
    }
}

