#ifndef __GG_HPP__
#define __GG_HPP__
#include "../../include/ipelib.h"
#include <utility>
#include <vector>

bool GG(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

#endif