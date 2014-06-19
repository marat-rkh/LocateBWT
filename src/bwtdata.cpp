#include "bwtdata.h"
#include "global_params.h"

using std::string;
using std::vector;

void BwtData::Build(string const& text, vector<size_t> const& suff_array) {
    start_pos = BuildStartPos(text, suff_array);
    letters_entries = BuildLetterEntries(text, suff_array);
}

vector<size_t> BwtData::BuildStartPos(string const& text, vector<size_t> const& suff_array) {
    vector<size_t> start_positions(ALPHABET_SIZE, 0);
    size_t cur_pos = 0;
    for(size_t i = 0; i != ALPHABET_SIZE; ++i) {
        size_t letter_code = (size_t)text[suff_array[cur_pos]];
        while(i > letter_code && cur_pos != text.size() - 1) {
            ++cur_pos;
            letter_code = (size_t)text[suff_array[cur_pos]];
        }
        start_positions[i] = cur_pos;
    }
    return start_positions;
}

vector<vector<size_t> > BwtData::BuildLetterEntries(string const& text, vector<size_t> const& suff_array) {
    vector<vector<size_t> > entries(1, vector<size_t>(ALPHABET_SIZE, 0));
    size_t prev_letter = (suff_array[0] == 0) ? (size_t)text.back() : (size_t)text[suff_array[0] - 1];
    entries[0][prev_letter] = 1;
    for(size_t i = 1; i != suff_array.size(); ++i) {
        entries.push_back(entries[i - 1]);
        prev_letter = (suff_array[i] == 0) ? (size_t)text.back() : (size_t)text[suff_array[i] - 1];
        entries[i][prev_letter] += 1;
    }
    return entries;
}
