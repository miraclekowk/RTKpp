//
// Created by nins on 2022/3/15.
//

#include "rtk_threadpool.h"


rtk_threadpool::rtk_threadpool():done(false),isEmpty(true),isFull(false) {

}

//设置初始线程数
void rtk_threadpool::setSize(int num) {
    this->initnum = num;
}

//添加任务
void rtk_threadpool::addTask(const task &f) {
    if(!done){
        //独占_mutex锁，对象lk生命周期结束后自动解锁
        std::unique_lock<std::mutex> lk(_mutex);

        //if task_queue is full
        while(this->isFull){
            cond.wait(lk);
        }
        //增加任务
        task_queue.push(f);

        if(task_queue.size() == this->initnum){
            this->isFull = true;
        }

        isEmpty = false;
        //唤醒其他阻塞的线程
        cond.notify_one();
    }

}

//线程池结束全部工作
void rtk_threadpool::finish() {
    for(int i = 0;i < this->threads.size();i++){
        threads[i].join();
    }
}

void rtk_threadpool::runTask() {
    while(!done){
        //加锁互斥锁
        std::unique_lock<std::mutex> lk(_mutex);

        //任务空
        ///尝试拿锁，拿到了说明轮到本线程执行了
        while(this->isEmpty){
            cond.wait(lk);
        }

        //任务队列出列
        task ta;
        ///move() 左值引用强行变成右值引用
        ta = std::move(task_queue.front());
        task_queue.pop();

        if(task_queue.empty()){
            this->isEmpty = true;
        }
        isFull = false;

        //执行任务
        //ta.func(ta.arg); //原先使用函数指针，不太对，这里学习c++写法 typedef function<返回值>
        ta();   //利用bind(funcName,args)来带参数执行
        cond.notify_one();
    }
}

//
void rtk_threadpool::start(int num) {
    rtk_threadpool::setSize(num);

    //线程数组初始化
    for(int i = 0;i < num;i++){
        threads.push_back(std::thread(&rtk_threadpool::runTask, this));
    }
}

rtk_threadpool::~rtk_threadpool() {

}

void test_tp(int a){
    std::cout<<"now is thread:"<<std::this_thread::get_id()<<"， task finish"<<a<<std::endl;
}

TEST(TestCase,test5_threadpool_func){
    rtk_threadpool tp;
    tp.start(4);
    int a = 0;

    while(1){
        a++;
        //调整cpu调度
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto task = std::bind(test_tp, a);
        tp.addTask(task);
    }

    tp.finish();
}