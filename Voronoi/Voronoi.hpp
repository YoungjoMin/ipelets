#ifndef __VORONOI_HEPP__
#define __VORONOI_HEPP__
#include "../../include/ipelib.h"
# include "Utils.hpp"
#include <utility>
#include <vector>

bool PointVoronoi(const std::vector<ipe::Vector>& pts,
    std::vector<ipe::Vector>& npts,
    std::pair<int,int>& edges);

#endif