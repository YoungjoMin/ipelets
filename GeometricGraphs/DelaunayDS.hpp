#ifndef __DELAUNAY_DS_HPP__
#define __DELAUNAY_DS_HPP__

#include "../../include/ipelib.h"

using namespace ipe;

struct Triangle;
struct Node;
struct PointLocation;

struct Triangle { //for legalize, save adj triangle info
    Node * node;
    Triangle * adj[3];
    Triangle() {}
    void updateAdj(Triangle * prv, Triangle * cur);
};
struct Node {
    int p1, p2, p3; //ccw order
    //edge order = 12, 23, 31
    Node *c1,*c2,*c3;//child
    Triangle * t;// is it's not leaf, then t = NULL

    //to find suitable child
    //Vector pt;
    int pt;// point that divide this node
    enum Divide {
        NotDivided=0,
        ByPtOnEdge12,
        ByPtOnEdge23,
        ByPtOnEdge31,
        BySinglePt
    } divideBy;

    bool visited;//for gathering all Triangles at the last

    Node(int p1, int p2, int p3);
    bool isLeaf() const;
    Node * findChild(int idx) const;//assert this node have child 
    int edgeNum(int idx) const;//assert this point idx is inside the triangle, if point on edge, return corresponding edge num, o.w. return 0
    int vertexNum(int idx) const;
    int getOppositeVertex(int edge) const;
    int getOppositeEdge(int idx) const;
    int getAdjNodeOppositeVertex(int edge) const;
    Node * getAdjNode(int edge) const;
    void pushValidEdges(std::vector<std::pair<int, int>>& edges, int limit) const;
};

struct PointLocation {
    Node * root;
    PointLocation(const Vector& bl, const Vector& tr); //Point location structure that all points are bounded by bl, tr
    void insert(int idx);//insert idx's point
    void gatherAllEdges(std::vector<std::pair<int, int>>& edges, int limit) const;

private:
    void legalize(Node * node, int idx);
    void flip(Node * node, Node * onode, int a, int b);
    void insertPointInterior(Node* node, int idx);
    void insertPointEdge(Node* node, int idx, int edge);
};


#endif