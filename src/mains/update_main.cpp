#include <iostream>
#include <boost/program_options.hpp>

#include "../suffixarraybuilder.h"
#include "../dirdata.h"
#include "../dirwalker.h"
#include "../bwtdata.h"
#include "../dataserialization.h"

using std::cout;
using std::string;
using namespace boost::program_options;

int main(int ac, char** av) {
    options_description desc("Opts");
    string root_flag = "database-root";
    string out_flag = "output";
    desc.add_options()
            (root_flag.c_str(), value<string>(), "")
            (out_flag.c_str(), value<string>(), "");

    variables_map vm;
    store(parse_command_line(ac, av, desc), vm);
    notify(vm);

    if (vm.count(root_flag) && vm.count(out_flag)) {
        string root_path = vm[root_flag].as<string>();
        string out_path = vm[out_flag].as<string>();
        DirWalker dir_walker;
        std::shared_ptr<DirData> dir_data_ptr = dir_walker.CollectData(root_path);
        if(dir_data_ptr == nullptr) {
            cout << "No such directory: " + root_path << '\n';
            return 1;
        }
        SuffixArrayBuilder builder;
        vector<size_t> suff_arr = builder.build(dir_data_ptr->filenames);
        BwtData bwt_data;
        bwt_data.Build(dir_data_ptr->filenames,  suff_arr);
        SaveData(out_path, suff_arr, *dir_data_ptr, bwt_data);
        return 0;
    }
    cout << "Usage: updatedb --­­database-­root <indexing dir path> --­­output <index store path>" << '\n';
    return 1;
}
