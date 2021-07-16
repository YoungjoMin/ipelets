#pragma once

struct Triangle;
struct Node;
struct PointLocation;

struct Triangle {
    int p1, p2, p3; //ccw order
    Node * node;
    Triangle * adj12,* adj23,* adj31;
    Triangle(int p1, int p2, int p3): p1(p1), p2(p2), p3(p3) {}
};
struct Node {
    Node *c1,*c2,*c3;//child
    Triangle * t;// is it's not leaf, then t = NULL

    //to find suitable child
    Vector pt;
    enum Divide {
        NotDivided,
        BySinglePt,
        ByPtOnEdge12,
        ByPtOnEdge23,
        ByPtOnEdge31
    } divideBy;

    Node();
    bool isLeaf() const;
};

struct PointLocation {
    Node * root;
    PointLocation(const Vector& bl, const Vector& tr); //Point location structure that all points are bounded by bl, tr
};
