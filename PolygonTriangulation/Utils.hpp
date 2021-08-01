#ifndef __UTILS_HPP__
#define __UTILS_HPP__

# include "../../include/ipelib.h"

double cross(const ipe::Vector& v1, const ipe::Vector& v2);
double cross(const ipe::Vector& v1, const ipe::Vector& v2, const ipe::Vector& v3);

//endpoint-endpoint intersection is not considered as intersection
bool intersects(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22);

//endpoint-endpoint intersection is considered as intersection
bool intersects2(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22);

//origin(0,0) in triangle(t1,t2,t3) or not
bool originInTriangle(const ipe::Vector& t1, const ipe::Vector& t2, const ipe::Vector& t3);

//p is in triangle(t1,t2,t3) or not
bool pointInTriangle(const ipe::Vector& p, const ipe::Vector& t1, const ipe::Vector& t2, const ipe::Vector& t3);
#endif