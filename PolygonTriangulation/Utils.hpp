#ifndef __UTILS_HPP__
#define __UTILS_HPP__

# include "../../include/ipelib.h"

double cross(const ipe::Vector& v1, const ipe::Vector& v2);
double cross(const ipe::Vector& v1, const ipe::Vector& v2, const ipe::Vector& v3);

//endpoint-endpoint intersection is not considered as intersection
bool intersects(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22);

//endpoint-endpoint intersection is considered as intersection
bool intersects2(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22);

#endif