#ifndef __GEOMETRICGRAPHS_HEPP__
#define __GEOMETRICGRAPHS_HEPP__
#include "../../include/ipelib.h"
# include "Utils.hpp"
#include <utility>
#include <vector>

bool Delaunay(const std::vector<ipe::Vector>& pts, 
    std::vector<std::pair<int, int>>& edges);

bool GG(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

bool RNG(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

bool EMST(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

#endif