#ifndef DATASERIALIZATION_H
#define DATASERIALIZATION_H

#include <fstream>
#include <tuple>

#include "dirdata.h"
#include "bwtdata.h"

using std::string;
using std::vector;
using std::tuple;

void SaveData(string const& path, vector<size_t> const& suff_arr, DirData const& dir_data, BwtData const& bwt_data);
tuple<vector<size_t>, DirData, BwtData> LoadData(string const& path);

#endif // DATASERIALIZATION_H
