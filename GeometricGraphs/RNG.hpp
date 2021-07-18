#ifndef __RNG_HPP__
#define __RNG_HPP__
#include "../../include/ipelib.h"
#include <utility>
#include <vector>

bool RNG(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

#endif