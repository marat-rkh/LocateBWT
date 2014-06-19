#ifndef GLOBAL_PARAMS_H
#define GLOBAL_PARAMS_H

#include <cstdlib>
#include <thread>

const size_t ALPHABET_SIZE = 256;
const size_t THREADS_NUM = std::thread::hardware_concurrency() == 0 ? 2 : std::thread::hardware_concurrency();

#endif // GLOBAL_PARAMS_H
