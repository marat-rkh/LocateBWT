#include "dataserialization.h"
#include "dirdata.h"
#include "bwtdata.h"

#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

void SaveData(string const& path, vector<size_t> const& suff_arr, DirData const& dir_data, BwtData const& bwt_data) {
    std::ofstream ofs(path);
    boost::archive::binary_oarchive oa(ofs);
    oa << suff_arr;
    oa << dir_data.paths_mapper;
    oa << dir_data.file_paths;
    oa << bwt_data.start_pos;
    oa << bwt_data.letters_entries;
}

tuple<vector<size_t>, DirData, BwtData> LoadData(string const& path) {
    std::ifstream ifs(path);
    boost::archive::binary_iarchive ia(ifs);
    vector<size_t> suff_arr;
    ia >> suff_arr;
    DirData dir_data;
    ia >> dir_data.paths_mapper;
    ia >> dir_data.file_paths;
    BwtData bwt_data;
    ia >> bwt_data.start_pos;
    ia >> bwt_data.letters_entries;
    return std::make_tuple(suff_arr, dir_data, bwt_data);
}
