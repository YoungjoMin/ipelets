# include "../../include/ipelib.h"
# include "Utils.hpp"

using namespace ipe;
using namespace std;

double cross(const Vector& v1, const Vector& v2) {
    return v1.x*v2.y - v1.y*v2.x;
}
double ccw(const Vector& v1, const Vector& v2, const Vector& v3) {
    return cross(v2-v1,v3-v1);
}

//endpoint-endpoint intersection is not considered as intersection
bool intersects(const Vector& l11, const Vector& l12, const Vector& l21, const Vector& l22) {
	Vector d1 = l12 - l11;
	Vector d2 = l22 - l21;
	Vector x  = l21 - l11;
	double n1 = cross(x, d2);
	double n2 = cross(x, d1);
	double d = cross(d1, d2);
	
	
	if (d == 0) {
		if (n1 != 0) return false;
		n1 = dot(l21-l11,d1);
        n2 = dot(l22-l11,d1);
		d = dot(d1,d1);
        if (n1>n2) std::swap(n1,n2);
        return 0<n2 && n1<d;
	}
	else {
		if (d < 0) d = -d, n1 = -n1, n2 = -n2;
        return ((0 < n1 && n1 < d) && (0 <= n2 && n2 <= d))
            || ((0 <= n1 && n1 <= d) && (0 < n2 && n2 < d));
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
	
	
	if (d == 0) {
		if (n1 != 0) return false;
		n1 = dot(l21-l11,d1);
        n2 = dot(l22-l11,d1);
		d = dot(d1,d1);
        if (n1>n2) std::swap(n1,n2);
        return 0<=n2 && n1<=d;
	}
	else {
		if (d < 0) d = -d, n1 = -n1, n2 = -n2;
        return ((0 <= n1 && n1 <= d) && (0 <= n2 && n2 <= d));
	}
}

bool originInTriangle(const Vector& t1, const Vector& t2, const Vector& t3) {
    Vector p1,p2=t3;
    int w =0;

    for(const Vector& c : {t1,t2,t3}) {
        p1 = p2;
        p2 = c;

        if(p1.y*p2.y<0) {
            double tmp = cross(p2,p1);
            if(p1.y<0) w += tmp<0 ?  2 : 0;
            else       w += tmp>0 ? -2 : 0;
        }
        else if(p1.y==0 && p1.x>0) {
            w += p2.y>0 ? 1 : -1;
        }
        else if(p2.y==0 && p2.x>0) {
            w += p1.y<0 ? 1 : -1;
        }

    }
    return w!=0;
}

bool pointInTriangle(const Vector& p, const Vector& t1, const Vector& t2, const Vector& t3) {
    return originInTriangle(t1-p,t2-p,t3-p);
}