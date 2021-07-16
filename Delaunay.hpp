#pragma once
#include "include/ipelib.h"
#include <utility>
#include <vector>


bool Delaunay(std::vector<ipe::Vector>& pts, 
    std::vector<std::pair<int, int>>& edges);