# include "include/ipelib.h"
# include "Delaunay.hpp"
# include "DelaunayDS.hpp"
# include <vector>
# include <random>

using namespace ipe;

//all points want to add at Delaunay triangulation
//contained here
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

void loadPts(const std::vector<Vector>& o) {
    pts.clear();
    pts.insert(pts.begin(),o.begin(),o.end());
}

Node::Node() {
    c1=c2=c3= NULL; t=NULL;
    divideBy = NotDivided;
}

bool Node::isLeaf() const {
    return divideBy==NotDivided;
}

PointLocation::PointLocation(const Vector& bl, const Vector& tr) {
    Vector nbl = bl - (tr-bl)*2;
    Vector ntr = tr + (tr-bl)*2;

    double &a = ntr.x,&b=ntr.y, &c=nbl.x,&d=nbl.y;
    
    //Giant triangle
    int n = pts.size();
    pts.push_back( Vector((a+c)/2, b + (a-c)/2));
    pts.push_back( Vector(c - (b-d), d));
    pts.push_back( Vector(a + (b-d), d));

    Triangle* t = new Triangle(n, n+1, n+2);
    Node * node = new Node();
    t->node = node;
    t->adj12=t->adj23=t->adj31 = NULL;
    node->t = t;
    root = node;
}