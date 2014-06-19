#ifndef TESTS_H
#define TESTS_H

#include <boost/filesystem.hpp>
#include "suffixarraybuilder.h"
#include "dirdata.h"
#include "dirstraverse.h"
#include "bwtdata.h"
#include "dataserialization.h"

using namespace boost::filesystem;
using std::cout;
using std::endl;

void TestSufArrBuilder() {
    SuffixArrayBuilder builder;
    string str = "mississippi";
    vector<size_t> res = builder.build(str);
    for(size_t i = 0; i != res.size(); ++i) {
        cout << str.substr(res[i]) << endl;
    }
    cout << '\n';
}

void TestDirData() {
    DirData dd1;
    path p1("/home/f1");
    path p2("/home/f2");
    dd1.Add(p1);
    dd1.Add(p2);
    DirData dd2;
    path p3("/home/f3");
    path p4("/home/f4");
    dd2.Add(p3);
    dd2.Add(p4);
    dd1.Concat(dd2);
    DirData dd3;
    path p5("/home/d/f5");
    path p6("/home/d/f6");
    dd3.Add(p5);
    dd3.Add(p6);
    dd1.Concat(dd3);
    cout << dd1.filenames << '\n';
    for(size_t fpn : dd1.paths_mapper) {
        cout << fpn;
    }
    cout << '\n';
    for(string fp : dd1.file_paths) {
        cout << fp << '\n';
    }
    cout << '\n';
}

void TestDirsTraverse() {
    std::shared_ptr<DirData> data = CollectData("/home/mrx/Dropbox/Study/CppLessons/igor");
    if(data == nullptr) {
        cout << "result is null\n";
    }
    else {
        cout << data->filenames << '\n';
        for(size_t fpn : data->paths_mapper) {
            cout << fpn;
        }
        cout << '\n';
        for(string fp : data->file_paths) {
            cout << fp << '\n';
        }
    }
    cout << '\n';
}

void TestBwtDataCreation() {
    SuffixArrayBuilder builder;
    string str = "aabac";
    vector<size_t> res = builder.build(str);
    BwtData bwt_data;
    bwt_data.Build(str, res);
    for(size_t s : bwt_data.start_pos) {
        cout << s << '\n';
    }
    cout << '\n';
    for(vector<size_t> v : bwt_data.letters_entries) {
        for(size_t s : v) {
            cout << s << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

void PrintUpdateData(vector<size_t> const& suff_arr, DirData const& dir_data, BwtData const& bwt_data);

void TestUpdate() {
    string path = "/home/mrx/Downloads";
    DirData dir_data = *CollectData(path);
    SuffixArrayBuilder builder;
    vector<size_t> suff_arr = builder.build(dir_data.filenames);
    BwtData bwt_data;
    bwt_data.Build(dir_data.filenames,  suff_arr);
    //PrintUpdateData(suff_arr, dir_data, bwt_data);
    string store_path = "test";
    SaveData(store_path, suff_arr, dir_data, bwt_data);
    tuple<vector<size_t>, DirData, BwtData> loaded = LoadData(store_path);
    //PrintUpdateData(std::get<0>(loaded), std::get<1>(loaded), std::get<2>(loaded));
}

void PrintUpdateData(vector<size_t> const& suff_arr, DirData const& dir_data, BwtData const& bwt_data) {
    for(size_t e : suff_arr) {
        cout << e << ' ';
    }
    cout << "\n\n";
    for(size_t e : dir_data.paths_mapper) {
        cout << e << ' ';
    }
    cout << "\n\n";
    for(string e : dir_data.file_paths) {
        cout << e << ' ';
    }
    cout << "\n\n";
    for(size_t e : bwt_data.start_pos) {
        cout << e << ' ';
    }
    cout << "\n\n";
    for(vector<size_t> v : bwt_data.letters_entries) {
        for(size_t s : v) {
            cout << s << ' ';
        }
        cout << '\n';
    }
    cout << "\n\n";
}

#endif // TESTS_H
