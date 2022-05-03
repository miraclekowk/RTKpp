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
    {
        std::unique_lock<std::mutex> lk(_mutex);
        done = true;
    }

    for(int i = 0;i < this->threads.size();i++){
        threads[i].join();
    }
    threads.clear();
}

void rtk_threadpool::runTask() {
    while(!done){
        //任务队列出列
        task ta;
        {
            //加锁互斥锁
            std::unique_lock<std::mutex> lk(_mutex);

            ///尝试拿锁，拿到了说明轮到本线程执行了
            //任务空
            while (this->isEmpty) {
                cond.wait(lk);
            }

            //任务出列
            ///move() 左值引用强行变成右值引用,避免深拷贝
            ta = std::move(task_queue.front());
            task_queue.pop();
            if (task_queue.empty()) {
                this->isEmpty = true;
            }
            isFull = false;
        }
        //执行任务
        //ta.func(ta.arg); //原先使用函数指针,这里学习c++写法 typedef function<返回值>
        ta();   //利用bind(funcName,args)来带参数执行

        cond.notify_one();
    }
}

void rtk_threadpool::start(int num) {
    rtk_threadpool::setSize(num);

    //线程数组初始化
    for(int i = 0;i < num;i++){
        threads.push_back(std::thread(&rtk_threadpool::runTask, this));
    }
}

rtk_threadpool::~rtk_threadpool() {
    finish();
}
class test1_tp{
public:
    static void test_tp(int a,int b){
        std::cout<<"now is thread:"<<std::this_thread::get_id()<<"， task finish"<<a+b<<std::endl;
    }
};

#ifdef DEBUG_MODE

TEST(TestCase,test5_threadpool_func){
    rtk_threadpool tp;
    tp.start(4);
    int a = 0;
    int b = 6657;

    while(a<100){
        a++;
        //调整cpu调度间隔
        std::this_thread::sleep_for(std::chrono::milliseconds (200));
        //这里 ts是自动根据bind推断的  推断结果是bind<void(int,int)>
        auto ts = std::bind(test1_tp::test_tp, a,b);
        tp.addTask(ts);
    }

    tp.finish();
}

#endif