#ifndef FINDER_H
#define FINDER_H

#include <vector>
#include <string>

#include "dirdata.h"
#include "dataserialization.h"

using std::vector;
using std::string;

class Finder {
public:
    vector<string> FindFiles(string const& index_path, string const& query);
private:
    vector<string> BwtSearch(string const& query);
    size_t GetInitRBound(size_t lbound, size_t fst_letter);
    vector<string> GetFilePaths(size_t lbound, size_t rbound);

    vector<size_t> suff_arr_;
    DirData dir_data_;
    BwtData bwt_data_;
};

#endif // FINDER_H
