#ifndef __UTILS_HPP__
#define __UTILS_HPP__

# include "../../include/ipelib.h"
# include <algorithm>
constexpr double eps = 1e-6;

inline bool epsGE(double a, double b) {//a>=b
    return (a-b) > -std::max({1.0, std::abs(a), std::abs(b)})*eps;
}
inline bool epsGT(double a, double b) {//a>b
    return (a-b) > std::max({1.0, std::abs(a), std::abs(b)})*eps;
}
inline bool epsEQ(double a, double b) {//a==b
    return std::abs(a-b) <= std::max({1.0, std::abs(a), std::abs(b)})*eps;
}
inline bool epsNE(double a, double b) {//a!=b
    return std::abs(a-b) > std::max({1.0, std::abs(a), std::abs(b)})*eps;
}
inline bool epsLT(double a, double b) {//a<b
    return (a-b) < -std::max({1.0, std::abs(a), std::abs(b)})*eps;
}
inline bool epsLE(double a, double b) {//a<=b
    return (a-b) < std::max({1.0, std::abs(a), std::abs(b)})*eps;
}
bool inCircle(const ipe::Vector& a, const ipe::Vector& b, const ipe::Vector& c, const ipe::Vector& d);
double cross(const ipe::Vector& v1, const ipe::Vector& v2);
bool ccw(const ipe::Vector& v1, const ipe::Vector& v2, const ipe::Vector& v3);
bool sccw(const ipe::Vector& v1, const ipe::Vector& v2, const ipe::Vector& v3);

//endpoint-endpoint intersection is not considered as intersection
bool intersects(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22);

//endpoint-endpoint intersection is considered as intersection
bool intersects2(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22);

//endpoint-endpoint intersection is considered as intersection
bool intersects2(const ipe::Vector& l11, const ipe::Vector& l12, const ipe::Vector& l21, const ipe::Vector& l22, ipe::Vector& out);

bool ConvexPolygonIntersection(const std::vector<ipe::Vector>& P, const std::vector<ipe::Vector>& Q, std::vector<ipe::Vector>& out);
#endif