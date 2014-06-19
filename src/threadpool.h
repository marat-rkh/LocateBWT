#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <queue>
#include <vector>
#include <future>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

using std::queue;
using std::vector;
using std::packaged_task;
using std::future;
using std::function;
using std::thread;
using std::mutex;
using std::shared_ptr;
using std::unique_lock;
using std::condition_variable;

class ThreadPool {
public:
    ThreadPool(size_t num);
    ThreadPool(ThreadPool const&) = delete;
    ~ThreadPool();

    template<typename T, typename ... Args>
    future<T> Submit(function<T(Args...)> func, Args ... args) {
        function<T()> no_params_func = [=](){ return func(args...); };
        shared_ptr<packaged_task<T()>> packaged_task_ptr(new packaged_task<T()>(no_params_func));
        function<void()> no_return_val_func = [=](){ (*packaged_task_ptr)(); };
        {
            unique_lock<mutex> lock(general_mtx_);
            tasks_.push(no_return_val_func);
        }
        ready_tasks_.notify_one();
        return packaged_task_ptr->get_future();
     }

private:
    vector<thread> threads_;
    bool finish_flag_;
    condition_variable ready_tasks_;
    queue<function<void(void)> > tasks_;
    mutex general_mtx_;
};

#endif // THREADPOOL_H
