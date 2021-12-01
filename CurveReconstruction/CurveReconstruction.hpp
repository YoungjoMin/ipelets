#ifndef __POLYGON_TRIANGULATION_HPP__
#define __POLYGON_TRIANGULATION_HPP__

# include "../../include/ipelib.h"
# include <vector>
# include <utility>

using Edge = std::pair<int,int>;
using Func = bool (*) (std::vector<ipe::Vector>&, std::vector<Edge>&);

bool Crust(std::vector<ipe::Vector>& pts,
    std::vector<Edge>& edges);

bool Delaunay(const std::vector<ipe::Vector>& npts,
    std::vector<Edge>& edges);

bool VoronoiVertices(const std::vector<ipe::Vector>& pts,
    std::vector<ipe::Vector>& npts);

#endif