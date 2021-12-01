# include "../../include/ipelib.h"
# include "Utils.hpp"

using namespace std;
using Vector = ipe::Vector;

double cross(const Vector& v1, const Vector& v2) {
    return v1.x*v2.y - v1.y*v2.x;
}
bool ccw(const Vector& v1, const Vector& v2, const Vector& v3) {
    return epsGE(cross(v2-v1,v3-v1),0.0);
}
bool sccw(const Vector& v1, const Vector& v2, const Vector& v3) {
    return epsGT(cross(v2-v1,v3-v1),0.0);
}
bool inCircle(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
    double t[3][3] = {
        {a.x-d.x, a.y-d.y, a.sqLen()-d.sqLen()},
        {b.x-d.x, b.y-d.y, b.sqLen()-d.sqLen()},
        {c.x-d.x, c.y-d.y, c.sqLen()-d.sqLen()},
    };
    double det = 0.0;
    det += t[0][0]*(t[1][1]*t[2][2]-t[1][2]*t[2][1]);
    det += t[0][1]*(t[1][2]*t[2][0]-t[1][0]*t[2][2]);
    det += t[0][2]*(t[1][0]*t[2][1]-t[1][1]*t[2][0]);
    return epsGT(det,0.0);
}
//endpoint-endpoint intersection is not considered as intersection
bool intersects(const Vector& l11, const Vector& l12, const Vector& l21, const Vector& l22) {
	Vector d1 = l12 - l11;
	Vector d2 = l22 - l21;
	Vector x  = l21 - l11;
	double n1 = cross(x, d2);
	double n2 = cross(x, d1);
	double d = cross(d1, d2);
	
	
	if (epsEQ(d,0.0)) {
		if (epsNE(n1,0.0)) return false;
		n1 = dot(l21-l11,d1);
        n2 = dot(l22-l11,d1);
		d = dot(d1,d1);
        if (n1>n2) std::swap(n1,n2);
        return epsGT(n2,0.0) && epsLT(n1,d);
	}
	else {
		if (epsLT(d,0.0)) d = -d, n1 = -n1, n2 = -n2;
        return ((epsGT(n1,0.0)&&epsLT(n1,d)) && (epsGE(n2,0.0)&&epsLE(n2,d)))
            || ((epsGE(n1,0.0)&&epsLE(n1,d)) && (epsGT(n2,0.0)&&epsLT(n2,d)));
	}
}

//endpoint-endpoint intersection is considered as intersection
bool intersects2(const Vector& l11, const Vector& l12, const Vector& l21, const Vector& l22) {
	Vector d1 = l12 - l11;
	Vector d2 = l22 - l21;
	Vector x  = l21 - l11;

	double n1 = cross(x, d2);
	double n2 = cross(x, d1);
	double d = cross(d1, d2);
	
	
	if (epsEQ(d,0.0)) {
		if (epsNE(n1,0.0)) return false;
		n1 = dot(l21-l11,d1);
        n2 = dot(l22-l11,d1);
		d = dot(d1,d1);
        if (n1>n2) std::swap(n1,n2);
        return epsGE(n2,0.0) && epsLE(n1,d);
	}
	else {
		if (epsLT(d,0.0)) d = -d, n1 = -n1, n2 = -n2;
        return (epsGE(n1,0.0)&&epsLE(n1,d)) && (epsGE(n2,0.0)&&epsLE(n2,d));
	}
}

//endpoint-endpoint intersection is considered as intersection
bool intersects2(const Vector& l11, const Vector& l12, const Vector& l21, const Vector& l22, Vector& pos) {
	Vector d1 = l12 - l11;
	Vector d2 = l22 - l21;
	Vector x  = l21 - l11;

	double n1 = cross(x, d2);
	double n2 = cross(x, d1);
	double d = cross(d1, d2);
	
	
	if (epsEQ(d,0.0)) {
		if (epsNE(n1,0.0)) return false;
		n1 = dot(l21-l11,d1);
        n2 = dot(l22-l11,d1);
		d = dot(d1,d1);
        pos = (-eps< n1 && n1 < d+eps) ? l21 : l22;
        if (n1>n2) std::swap(n1,n2);
        return epsGE(n2,0.0) && epsLE(n1,d);
	}
	else {
        pos = l11 + (n1/d)*d1;
		if (epsLT(d,0.0)) d = -d, n1 = -n1, n2 = -n2;
        return (epsGE(n1,0.0)&&epsLE(n1,d)) && (epsGE(n2,0.0)&&epsLE(n2,d));
	}
}