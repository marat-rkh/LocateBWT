#ifndef DIRSTRAVERSE_H
#define DIRSTRAVERSE_H

#include <mutex>
#include <memory>
#include <queue>

#include "dirdata.h"
#include "threadpool.h"
#include "global_params.h"
#include "dirdata.h"

using std::shared_ptr;
using std::mutex;
using std::queue;

class DirWalker {
public:
    DirWalker() : tpool_(THREADS_NUM) {}
    shared_ptr<DirData> CollectData(std::string const& root_dir);
private:
    shared_ptr<DirData> CollectDataRec(path dir_path);

    ThreadPool tpool_;
    queue<future<shared_ptr<DirData> > > future_res_;
    mutex mtx_;
};

#endif // DIRSTRAVERSE_H
