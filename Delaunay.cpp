# include "include/ipelib.h"
# include "Delaunay.hpp"
# include "DelaunayDS.hpp"
# include <vector>
# include <random>

using namespace ipe;

static std::vector<Vector> pts;

bool inCircle(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
    double t[3][3] = {
        {b.x-a.x, b.y-a.y, b.sqLen()-a.sqLen()},
        {c.x-a.x, c.y-a.y, c.sqLen()-a.sqLen()},
        {d.x-a.x, d.y-a.y, d.sqLen()-a.sqLen()},
    };
    double det = 0.0;
    det += t[0][0]*(t[1][1]*t[2][2]-t[1][2]*t[2][1]);
    det += t[0][1]*(t[1][2]*t[2][0]-t[1][0]*t[2][2]);
    det += t[0][2]*(t[1][0]*t[2][1]-t[1][1]*t[2][0]);
    return det>0;
}