#include <iostream>
#include <boost/program_options.hpp>

#include "../finder.h"

using std::cout;
using namespace boost::program_options;

int main(int ac, char** av) {
    options_description desc("Opts");
    string opt_flag = "database";
    vector<string> args;
    desc.add_options()
            (opt_flag.c_str(), value<vector<string> >(&args)->multitoken(), "");

    variables_map vm;
    store(parse_command_line(ac, av, desc), vm);
    notify(vm);

    if (vm.count(opt_flag) && args.size() >= 2) {
        Finder finder;
        vector<string> files = finder.FindFiles(args[0], args[1]);
        for(string s : files) {
            cout << s << '\n';
        }
        return 0;
    }
    cout << "Usage: locate --database <db file path> <query>" << '\n';
    return 1;
}
