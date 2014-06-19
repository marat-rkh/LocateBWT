#include "dirdata.h"

#include <algorithm>
#include <functional>

void DirData::Add(path const& file_path) {
    file_paths.push_back(file_path.generic_string());
    string filename = file_path.filename().generic_string();
    filenames += DELIM;
    filenames += filename;
    paths_mapper.push_back(0);
    for(size_t i = filenames.size() - filename.size(); i != filenames.size(); ++i) {
        paths_mapper.push_back(file_paths.size() - 1);
    }
}

void DirData::Concat(DirData const& other) {
    filenames += other.filenames;
    paths_mapper.insert(paths_mapper.end(), other.paths_mapper.begin(), other.paths_mapper.end());
    std::function<void(size_t&)> shift = [=](size_t &elem){ elem += file_paths.size(); };
    std::for_each(paths_mapper.end() - other.paths_mapper.size(), paths_mapper.end(), shift);
    file_paths.insert(file_paths.end(), other.file_paths.begin(), other.file_paths.end());
}
