#include "suffixarraybuilder.h"
#include "global_params.h"

#include <thread>
#include <algorithm>
#include <cmath>

using std::string;
using std::vector;
using std::thread;

vector<size_t> SuffixArrayBuilder::build(std::string const& input) {
    string norm_input = input + DELIM;
    vector<size_t> positions(norm_input.size());
    Init(positions, norm_input);
    CountingSort(positions, ALPHABET_SIZE);
    size_t classes_num = RecountEqClasses(norm_input);
    size_t i = 0;
    size_t pair_elem_size = 1 << i;
    while (pair_elem_size < sorted_substrs_.size()) {
        SetFstPartsPositions(pair_elem_size, positions);
        CountingSort(positions, classes_num);
        classes_num = RecountEqClasses(pair_elem_size);
        ++i;
        pair_elem_size = 1 << i;
    }
    sorted_substrs_.erase(sorted_substrs_.begin());
    return sorted_substrs_;
}

void SuffixArrayBuilder::Init(vector<size_t>& positions, string const& input) {
    for(size_t i = 0; i != input.size(); ++i) {
        positions.at(i) = i;
        eq_classes_.push_back((unsigned char)input.at(i));
    }
    sorted_substrs_.resize(input.size());
}

void SuffixArrayBuilder::CountingSort(vector<size_t> const& positions, int alphabet_size) {
//    vector<size_t> counts(alphabet_size, 0);
//    for (size_t i = 0; i != positions.size(); ++i) {
//        ++counts[eq_classes_[positions[i]]];
//    }
    vector<size_t> counts = Count(positions, alphabet_size);
    for (size_t i = 1; i != counts.size(); ++i) {
        counts[i] += counts[i - 1];
    }
    for (vector<size_t>::const_reverse_iterator it = positions.rbegin(); it != positions.rend(); ++it) {
        sorted_substrs_[--counts[eq_classes_[*it]]] = *it;
    }
}

size_t SuffixArrayBuilder::RecountEqClasses(std::string const& input) {
    eq_classes_[sorted_substrs_[0]] = 0;
    size_t classes_num = 1;
    for (size_t i = 1; i != sorted_substrs_.size(); ++i) {
        if (input[sorted_substrs_[i]] != input[sorted_substrs_[i - 1]]) {
            ++classes_num;
        }
        eq_classes_[sorted_substrs_[i]] = classes_num - 1;
    }
    return classes_num;
}

void SuffixArrayBuilder::SetFstPartsPositions(size_t pair_elem_size, vector<size_t>& positions) {
    for (size_t i = 0; i != positions.size(); ++i) {
        int raw_pos = sorted_substrs_[i] - pair_elem_size;
        positions[i] = raw_pos < 0 ? raw_pos + sorted_substrs_.size() : raw_pos;
    }
}

size_t SuffixArrayBuilder::RecountEqClasses(size_t pair_elem_size) {
    vector<size_t> fst_part_eq_classes(eq_classes_.size(), 0);
    size_t classes_num = 1;
    for (size_t i = 1; i != sorted_substrs_.size(); ++i) {
        size_t pair_fst_part_cur_pos = sorted_substrs_[i] + pair_elem_size;
        size_t pair_fst_part_prev_pos = sorted_substrs_[i - 1] + pair_elem_size;
        if(eq_classes_[sorted_substrs_[i]] != eq_classes_[sorted_substrs_[i - 1]] ||
           eq_classes_[pair_fst_part_cur_pos] != eq_classes_[pair_fst_part_prev_pos])
        {
            ++classes_num;
        }
        fst_part_eq_classes[sorted_substrs_[i]] = classes_num - 1;
    }
    eq_classes_ = fst_part_eq_classes;
    return classes_num;
}

vector<size_t> SuffixArrayBuilder::Count(vector<size_t> const& positions, int alphabet_size) {
    function<vector<size_t>(size_t, size_t)> task = [positions, alphabet_size, this](size_t start, size_t end) {
        vector<size_t> counts(alphabet_size, 0);
        for (size_t i = start; i != end; ++i) {
            ++counts[eq_classes_[positions[i]]];
        }
        return counts;
    };
    size_t block_size = std::ceil(positions.size() * 1.0 / THREADS_NUM);
    vector<future<vector<size_t> > > future_results;
    for(size_t i = 0; i < positions.size(); i += block_size) {
        size_t ubound = (i + block_size > positions.size()) ? positions.size() : i + block_size;
        future_results.push_back(tpool.Submit(task, i, ubound));
    }
    vector<vector<size_t> > res;
    for(future<vector<size_t> > & f : future_results) {
        res.push_back(f.get());
    }
    for(int i = 0; i != alphabet_size; ++i) {
        for(size_t j = 1; j != res.size(); ++j) {
            res[0][i] += res[j][i];
        }
    }
    return res[0];
}
