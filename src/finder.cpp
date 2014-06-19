#include "finder.h"
#include "dataserialization.h"

#include <tuple>
#include <boost/filesystem.hpp>

using std::vector;
using std::string;

vector<string> Finder::FindFiles(string const& index_path, string const& query) {
    tuple<vector<size_t>, DirData, BwtData> loaded = LoadData(index_path);
    suff_arr_ = std::get<0>(loaded);
    dir_data_ = std::get<1>(loaded);
    bwt_data_ = std::get<2>(loaded);
    return BwtSearch(query);
}

vector<string> Finder::BwtSearch(string const& query) {
    size_t letter_code = (unsigned char)query[query.size() - 1];
    size_t lbound = bwt_data_.start_pos[letter_code];
    size_t rbound = GetInitRBound(lbound, letter_code);
    for(string::const_reverse_iterator ri = query.rbegin() + 1; ri != query.rend(); ++ri) {
        letter_code = (unsigned char) *ri;
        lbound = (lbound == 0) ? 0 : bwt_data_.letters_entries[lbound - 1][letter_code];
        lbound += bwt_data_.start_pos[letter_code];
        rbound = bwt_data_.start_pos[letter_code] + bwt_data_.letters_entries[rbound - 1][letter_code];
    }
    return GetFilePaths(lbound, rbound);
}

size_t Finder::GetInitRBound(size_t lbound, size_t fst_letter) {
    size_t rbound = lbound + 1;
    if(lbound != suff_arr_.size() - 1) {
        rbound = suff_arr_.size();
        for(size_t i = fst_letter; i != bwt_data_.letters_entries[0].size(); ++i) {
            size_t next_start_pos = bwt_data_.start_pos[i];
            if(next_start_pos > lbound && next_start_pos < rbound) {
                rbound = next_start_pos;
            }
        }
    }
    return rbound;
}

vector<string> Finder::GetFilePaths(size_t lbound, size_t rbound) {
    vector<string> file_paths;
    for(size_t i = lbound; i < rbound; ++i) {
        size_t file_ptr = dir_data_.paths_mapper[suff_arr_[i]];
        string file_path = dir_data_.file_paths[file_ptr];
        if(boost::filesystem::exists(boost::filesystem::path(file_path))) {
            file_paths.push_back(file_path);
        }
    }
    return file_paths;
}
