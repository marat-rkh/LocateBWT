#ifndef SUFFIXARRAYBUILDER_H
#define SUFFIXARRAYBUILDER_H

#include <string>
#include <vector>

#include "global_params.h"
#include "threadpool.h"

using std::vector;
using std::string;

class SuffixArrayBuilder {
public:
    SuffixArrayBuilder() : tpool(THREADS_NUM) {}

    vector<size_t> build(string const& input);
private:
    void Init(vector<size_t>& positions, string const& input);
    void CountingSort(vector<size_t> const& positions, int alphabet_size);
    vector<size_t> Count(vector<size_t> const& positions, int alphabet_size);
    size_t RecountEqClasses(string const& input);
    void SetFstPartsPositions(size_t substr_size, vector<size_t>& positions);
    size_t RecountEqClasses(size_t pair_elem_size);

    vector<size_t> sorted_substrs_;
    vector<size_t> eq_classes_;
    ThreadPool tpool;

    const char DELIM = '\1';
};

#endif // SUFFIXARRAYBUILDER_H
