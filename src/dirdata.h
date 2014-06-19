#ifndef DIRDATA_H
#define DIRDATA_H

#include <string>
#include <vector>
#include <boost/filesystem.hpp>

using std::string;
using std::vector;
using boost::filesystem::path;

class DirData {
public:
    string filenames;
    vector<size_t> paths_mapper;
    vector<string> file_paths;

    void Add(path const& file_path);
    void Concat(DirData const& other);
private:
    static const char DELIM = '\2';
};


#endif // DIRDATA_H
