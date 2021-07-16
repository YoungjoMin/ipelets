#pragma once

struct Triangle;
struct Node;
struct PointLocation;

struct Triangle { //for legalize, save adj triangle info
    Node * node;
    Triangle * adj[3];
    Triangle() {}
};
struct Node {
    int p1, p2, p3; //ccw order
    //edge order = 12, 23, 31
    Node *c1,*c2,*c3;//child
    Triangle * t;// is it's not leaf, then t = NULL

    //to find suitable child
    int pt;// point that divide this node
    enum Divide {
        NotDivided,
        BySinglePt,
        ByPtOnEdge12,
        ByPtOnEdge23,
        ByPtOnEdge31
    } divideBy;

    bool visited;//for gathering all Triangles at the last

    Node(int p1, int p2, int p3);
    bool isLeaf() const;
    Node * findChild(int idx);//assert this node have child 

};

struct PointLocation {
    Node * root;
    PointLocation(const Vector& bl, const Vector& tr); //Point location structure that all points are bounded by bl, tr
    void insert(int idx);//insert idx's point
    void Legalize(Node * node, int idx);
    void gatherAllLeafs(std::vector<Node *>& buffer);
};
