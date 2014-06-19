#include "threadpool.h"

ThreadPool::ThreadPool(size_t num)  {
    finish_flag_ = false;
    for(size_t i = 0; i != num; ++i) {
        threads_.push_back(thread([&]() {
            while(true) {
                unique_lock<mutex> lock(general_mtx_);
                while(!finish_flag_ && tasks_.empty()) {
                    ready_tasks_.wait(lock);
                }
                if(finish_flag_ && tasks_.empty()) {
                    return;
                }
                function<void(void)> task = tasks_.front();
                tasks_.pop();
                lock.unlock();
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    general_mtx_.lock();
    finish_flag_ = true;
    general_mtx_.unlock();
    ready_tasks_.notify_all();
    for(size_t i = 0; i != threads_.size(); ++i) {
        threads_[i].join();
    }
}
