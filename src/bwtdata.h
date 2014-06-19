#ifndef BWTDATACREATION_H
#define BWTDATACREATION_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class BwtData {
public:
    vector<size_t> start_pos;
    vector<vector<size_t>> letters_entries;
    void Build(string const& text, vector<size_t> const& suff_array);
private:
    vector<size_t> BuildStartPos(string const& text, vector<size_t> const& suff_array);
    vector<vector<size_t> > BuildLetterEntries(string const& text, vector<size_t> const& suff_array);
};

#endif // BWTDATACREATION_H
