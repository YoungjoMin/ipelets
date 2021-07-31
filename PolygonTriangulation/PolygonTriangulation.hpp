#ifndef __POLYGON_TRIANGULATION_HPP__
#define __POLYGON_TRIANGULATION_HPP__

# include "../../include/ipelib.h"
# include <vector>
# include <utility>

using Polygon = std::pair<int,int>; //{a,b} => pts[a-1] (pts[a], ... pts[b]) pts[b+1], pts[a-1]=pts[b], pts[b+1] = pts[a]
using Edge = std::pair<int,int>;
using Triangulation = void (*) (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);

//Triangulation MinAreaMaximize, MaxAreaMinimize;
void MinAreaMaximize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);
void MaxAreaMinimize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);

#endif