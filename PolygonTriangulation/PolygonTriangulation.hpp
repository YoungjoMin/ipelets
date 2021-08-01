#ifndef __POLYGON_TRIANGULATION_HPP__
#define __POLYGON_TRIANGULATION_HPP__

# include "../../include/ipelib.h"
# include <vector>
# include <utility>

using Polygon = std::pair<int,int>; //{a,b} => pts[a-1] (pts[a], ... pts[b]) pts[b+1], pts[a-1]=pts[b], pts[b+1] = pts[a]
using Edge = std::pair<int,int>;
using Triangulation = void (*) (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);

//Triangulations
void MinAreaMaximize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);
void MaxAreaMinimize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);

void SumLengthMaximize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);
void SumLengthMinimize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);

void MinAngleMaximize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);
void MaxAngleMinimize (const std::vector<ipe::Vector>&, const std::vector<Polygon>&, std::vector<Edge>&);

#endif