
#include "utils.h"
#include "rtk_epoll.h"

#define CONF_FILE "../tkeed.conf"

RTK_config conf;
extern struct epoll_event* events_list;


#ifndef DEBUG_MODE


int main() {
    std::cout << "Hello, World!" << std::endl;
    //读配置
    read_conf(CONF_FILE,conf);
    //屏蔽掉SIGPIPE信号
    handle_for_sigpipe();

    //创建初始化listen_fd 并bind-listen 开始监听
    int listen_fd = socket_bind_and_listen(conf.port);
    //设置listen_fd非阻塞
    int rc = make_socket_no_blocking(listen_fd);

    //创建epoll对象 -- 这边有 单例，构造直接初始化等优化空间
    rtk_epoll* rtk_ep = new rtk_epoll();
    int epoll_fd = rtk_ep->rtk_epoll_create(0);

    //初始化request --为了把listen_fd加入epoll事件监听中
    rtk_request request(conf.root);
    rtk_ep->rtk_epoll_add(epoll_fd,listen_fd,&request,(EPOLLIN | EPOLLET));

    //初始化线程池
    rtk_threadpool* tp = new rtk_threadpool();
    tp->start(conf.thread_num);  //读取配置文件中指定的线程池数量

    //初始化timer计时器
    rtk_timer* timer = new rtk_timer();

    while(1){
        int time = timer->rtk_find_timer();
        //std::cout<<"now minimum time is "<<time<<std::endl;

        //拉取当前活跃事件
        int events_num = rtk_ep->rtk_epoll_wait(epoll_fd,events_list,MAXEVENTS,-1);

        //处理已超时请求
        timer->rtk_handle_expire_time();

        //分发任务给worker线程
        rtk_ep->distribute_events(epoll_fd,listen_fd,events_list,events_num,conf.root,tp,timer);

    }

    std::cout<<"RTKpp server is closing..."<<std::endl;

    free(rtk_ep);
    free(tp);
    free(timer);

    return 0;
}
#else

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif