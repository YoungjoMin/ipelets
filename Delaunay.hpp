#ifndef __DELAUNAY_HPP__
#define __DELAUNAY_HPP__
#include "include/ipelib.h"
#include <utility>
#include <vector>


bool Delaunay(const std::vector<ipe::Vector>& pts, 
    std::vector<std::pair<int, int>>& edges);

#endif