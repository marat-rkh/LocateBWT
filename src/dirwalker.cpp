#include "dirwalker.h"

#include <boost/filesystem.hpp>
#include <queue>
#include <memory>
#include <future>
#include <mutex>
#include <functional>

#include "dirdata.h"
#include "threadpool.h"
#include "global_params.h"

using namespace boost::filesystem;
using std::queue;
using std::string;
using std::shared_ptr;
using std::future;
using std::mutex;
using namespace std::placeholders;

shared_ptr<DirData> DirWalker::CollectData(string const& root_dir) {
    path root_path(root_dir);
    if(exists(root_path)) {
        shared_ptr<DirData> dir_data = CollectDataRec(root_path);
        while (!future_res_.empty()) {
            dir_data->Concat(*(future_res_.front().get()));
            unique_lock<mutex> lock(mtx_);
            future_res_.pop();
        }
        return dir_data;
    }
    return shared_ptr<DirData>();
}

shared_ptr<DirData> DirWalker::CollectDataRec(path dir_path) {
    shared_ptr<DirData> dir_data(new DirData());
    directory_iterator di(dir_path);
    directory_iterator dend;

    function<shared_ptr<DirData>(path)> fn_col_data = std::bind(&DirWalker::CollectDataRec, this, _1);
    for(; di != dend; ++di) {
        file_status cur_filestatus = di->status();
        path p = di->path();
        dir_data->Add(p);
        if(is_directory(cur_filestatus)) {
            unique_lock<mutex> lock(mtx_);
            future_res_.push(tpool_.Submit(fn_col_data, p));
        }
    }
    return dir_data;
}
