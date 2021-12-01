#ifndef __POLYGON_TRIANGULATION_HPP__
#define __POLYGON_TRIANGULATION_HPP__

# include "../../include/ipelib.h"
# include <vector>
# include <utility>

using Func = bool (*) (const std::vector<ipe::Vector>&, std::vector<std::pair<int, int>>&);

bool crust(const std::vector<ipe::Vector>& pts,
    std::vector<std::pair<int, int>>& edges);

#endif