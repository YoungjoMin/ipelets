#ifndef __EMST_HPP__
#define __EMST_HPP__
#include "../../include/ipelib.h"
#include <utility>
#include <vector>

bool EMST(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

#endif