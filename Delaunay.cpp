# include "include/ipelib.h"
# include "Delaunay.hpp"
# include "DelaunayDS.hpp"
# include <vector>
# include <random>
# include <queue>

using namespace ipe;

//all points want to add at Delaunay triangulation
//contained here
static std::vector<Vector> pts;
Triangle NOTUSED;
Triangle * NIL = &NOTUSED;

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

bool inCircle(int a, int b, int c, int pt) {
    return inCircle(pts[a],pts[b],pts[c],pts[pt]);
}

double cross(const Vector& v1, const Vector& v2) {
    return v1.x*v2.y-v1.y*v2.x;
}

bool ccw(const Vector& p1, const Vector& p2, const Vector& p3) {
    Vector t1 = p2-p1, t2 = p3-p1;
    return cross(t1,t2)>=0;
}

bool ccw(int a, int b, int c) {
    return ccw(pts[a],pts[b],pts[c]);
}

bool sccw(const Vector& p1, const Vector& p2, const Vector& p3) {
    Vector t1 = p2-p1, t2 = p3-p1;
    return cross(t1,t2)>0;
}

bool sccw(int a, int b, int c) {
    return sccw(pts[a],pts[b],pts[c]);
}

//intersection of two line segments a->b,  c->d
Vector intersection(const Vector& a, const Vector& b, const Vector& c, const Vector& d) {
    Vector A = b-a, B = b-d, C = c-d;
    double t1 = cross(B,C)/cross(A,C);
    return b - A*t1;
}

Vector intersection(int a, int b, int c, int d) {
    return intersection(pts[a],pts[b],pts[c],pts[d]);
}

void loadPts(const std::vector<Vector>& o) {
    pts.clear();
    pts.insert(pts.begin(),o.begin(),o.end());
}


//============================== implement DS

void Triangle::updateAdj(Triangle * prv, Triangle* cur) {
    if(adj[0]==prv) adj[0]=cur;
    else if(adj[1]==prv) adj[1]=cur;
    else if(adj[2]==prv) adj[2]=cur;
    return;
}

Node::Node(int p1, int p2, int p3): p1(p1), p2(p2), p3(p3) {
    c1=c2=c3= NULL; t=NULL;
    divideBy = NotDivided;
    visited = false;
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

    Triangle* t = new Triangle();
    Node * node = new Node(n, n+1, n+2);
    t->node = node;
    t->adj[0]=t->adj[1]=t->adj[2] = NIL;
    node->t = t;
    root = node;
}

//assert divieBy != NotDivided
Node * Node::findChild(int idx) const{
    if(divideBy == BySinglePt) {
        bool b1,b2,b3;
        b1 = ccw(pt,p1,idx);
        b2 = ccw(pt,p2,idx);
        b3 = ccw(pt,p3,idx);
        if(b1&&b2) return c1;
        else if(b2&&b3) return c2;
        return c3;
        //else if(b3&&b1) return c3;
    }
    else {
        int p = 0;
        p = divideBy == ByPtOnEdge12? p3 : p;
        p = divideBy == ByPtOnEdge23? p1 : p;
        p = divideBy == ByPtOnEdge31? p2 : p;

        if(ccw(pt,p,idx)) return c1;
        else return c2;
    }
}

int Node::edgeNum(int idx) const {
    if(!sccw(idx,p1,p2)) return 1;
    if(!sccw(idx,p2,p3)) return 2;
    if(!sccw(idx,p3,p1)) return 3;
    return 0;
}

int Node::vertexNum(int idx) const {
    if(p1==idx) return 1;
    if(p2==idx) return 2;
    if(p3==idx) return 3;
    return 0;
}

int Node::getOppositeVertex(int edge) const {
    int tmp[] = {0, p3, p1, p2};
    return tmp[edge];
}
int Node::getOppositeEdge(int idx) const {
    int tmp = vertexNum(idx);
    return tmp==3 ? 1 : tmp+1;
}

Node * Node::getAdjNode(int edge) const {
    Triangle * t = this->t;
    Triangle * adjt = t->adj[edge-1];
    return adjt->node;
}

int Node::getAdjNodeOppositeVertex(int edge) const {
    int tmp[] = {0, p1, p2, p3};
    Node * adj = this->getAdjNode(edge);
    int e1 = tmp[edge];
    if(e1==adj->p1) return adj->p2;
    if(e1==adj->p2) return adj->p3;
    if(e1==adj->p3) return adj->p1;
    return -1;
}

void PointLocation::insert(int idx) {
    Node * cur = root;
    while(!cur->isLeaf()) {
        cur = cur->findChild(idx);
    }
    int en = cur->edgeNum(idx);
    if(en!=0)  insertPointEdge(cur,idx,en);
    else  insertPointInterior(cur,idx);
    return;
}


void PointLocation::legalize(Node * node, int idx) {
    int edge = node->getOppositeEdge(idx);
    int test = node->getAdjNodeOppositeVertex(edge);
    if(test == -1) return; //if adj Node is NIL
    if(!inCircle(node->p1,node->p2,node->p3, test)) return;
    Node * onode = node->getAdjNode(edge);
    flip(node, onode, idx, test);
    legalize(node->c1, idx);
    legalize(node->c2, idx);
}

void PointLocation::flip(Node * node, Node * onode, int aidx, int bidx) {
    Triangle * t = node->t, *ot = onode->t;

    int anum = node->vertexNum(aidx), bnum = onode->vertexNum(bidx);
    int tmp[] = {node->p3, node->p1, node->p2, node->p3, node->p1};
    int cidx = tmp[anum-1], didx = tmp[anum+1];

    Triangle * t1 = t->adj[anum==1 ? 2 : anum-2];
    Triangle * t2 = t->adj[anum-1];
    Triangle * t3 = ot->adj[bnum==1 ? 2 : bnum-2];
    Triangle * t4 = ot->adj[bnum-1];

    Triangle * ut = new Triangle();
    Triangle * dt = new Triangle();
    Node * unode = new Node(aidx, bidx, cidx);
    Node * dnode = new Node(didx, bidx, aidx);

    ut->node = unode;
    ut->adj[0]=dt, ut->adj[1]=t4, ut->adj[2]=t1;

    dt->node = dnode;
    dt->adj[0]=t3, dt->adj[1]=ut, dt->adj[2]=t2;

    t1->updateAdj(t, ut);
    t2->updateAdj(t, dt);
    t3->updateAdj(ot, dt);
    t4->updateAdj(ot, ut);

    unode->t = ut;
    dnode->t = dt;
    
    //////////////
    int n = pts.size();
    node->pt = onode->pt = n;
    pts.push_back(intersection(aidx,bidx,cidx,didx));
    node->divideBy = (Node::Divide)(node->getOppositeEdge(aidx));
    onode->divideBy = (Node::Divide)(onode->getOppositeEdge(bidx));
    node->t = onode->t = NULL;
    node->c1= dnode, node->c2 = unode;
    onode->c1 = unode, onode->c2 = dnode;

    delete t;
    delete ot;
}

void PointLocation::gatherAllLeafs(std::vector<Node *>& buffer) const {
    if(root->isLeaf()) {
        buffer.push_back(root);
        return;
    }
    std::queue<Node *> q;
    q.push(root);
    while(!q.empty()) {
        Node* cur = q.front();
        q.pop();
        if(cur->isLeaf()) {
            buffer.push_back(cur);
            continue;
        }
        for(Node * chd : {cur->c1, cur->c2, cur->c3}) {
            if(chd == NULL) continue;
            if(chd->visited) continue;
            chd->visited=true;
            q.push(chd);
        }
    }
    return;
}

void PointLocation::insertPointInterior(Node * node, int idx) {
    Triangle * t = node->t;
    Triangle * t1,*t2,*t3;

    t1=t->adj[0], t2=t->adj[1],t3=t->adj[2];

    Triangle * nt1 = new Triangle();
    Triangle * nt2 = new Triangle();
    Triangle * nt3 = new Triangle();
    Node * node1 = new Node(idx, node->p1, node->p2);
    Node * node2 = new Node(idx, node->p2, node->p3);
    Node * node3 = new Node(idx, node->p3, node->p1);

    nt1->node = node1;
    nt1->adj[0] = nt3, nt1->adj[1] = t1, nt1->adj[2] = nt2;

    nt2->node = node2;
    nt2->adj[0] = nt1, nt2->adj[1] = t2, nt2->adj[2] = nt3;

    nt3->node = node3;
    nt3->adj[0] = nt2, nt3->adj[1] = t3, nt3->adj[2] = nt1;

    t1->updateAdj(t, nt1);
    t2->updateAdj(t, nt2);
    t3->updateAdj(t, nt3);

    node1->t = nt1;
    node2->t = nt2;
    node3->t = nt3;
    /////
    node->divideBy = Node::BySinglePt;
    node->pt = idx;
    node->t = NULL;
    node->c1 = node1;
    node->c2 = node2;
    node->c3 = node3;

    delete t;
}
void PointLocation::insertPointEdge(Node* node, int idx, int edge) {
    Node * onode = node->getAdjNode(edge);

    int tmp[] = {node->p3, node->p1, node->p2, node->p3, node->p1};
    int aidx = tmp[edge-1], bidx = tmp[edge+1], cidx = tmp[edge];
    int didx = node->getAdjNodeOppositeVertex(edge);
    int bnum = node->vertexNum(bidx), dnum = onode->vertexNum(didx);
    
    Triangle * t = node->t, *ot = onode->t;
    
    Triangle * t1 = t->adj[bnum==1 ? 2 : bnum-2];
    Triangle * t2 = t->adj[bnum-1];
    Triangle * t3 = ot->adj[dnum==1 ? 2 : dnum-2];
    Triangle * t4 = ot->adj[dnum-1];

    Triangle * nt1 = new Triangle();
    Triangle * nt2 = new Triangle();
    Triangle * nt3 = new Triangle();
    Triangle * nt4 = new Triangle();

    Node * node1 = new Node(idx, aidx, bidx);
    Node * node2 = new Node(idx, bidx, cidx);
    Node * node3 = new Node(idx, cidx, didx);
    Node * node4 = new Node(idx, didx, aidx);

    nt1->node = node1;
    nt1->adj[0] = nt4, nt1->adj[1] = t1, nt1->adj[2] = nt2;
    
    nt2->node = node2;
    nt2->adj[0] = nt1, nt2->adj[1] = t2, nt2->adj[2] = nt3;

    nt3->node = node3;
    nt3->adj[0] = nt2, nt3->adj[1] = t3, nt3->adj[2] = nt4;

    nt4->node = node4;
    nt4->adj[0] = nt3, nt4->adj[1] = t4, nt4->adj[2] = nt1;

    t1->updateAdj(t,nt1);
    t2->updateAdj(t,nt2);
    t3->updateAdj(ot,nt3);
    t4->updateAdj(ot,nt4);

    node1->t = nt1;
    node2->t = nt2;
    node3->t = nt3;
    node4->t = nt4;
    //////////////
    node->pt = onode->pt = idx;
    node->divideBy = (Node::Divide)(node->getOppositeEdge(bidx));
    onode->divideBy = (Node::Divide)(onode->getOppositeEdge(didx));
    node->t = onode->t = NULL;
    node->c1 = node2, node->c2 = node1;
    onode->c1 = node4, onode->c2 = node3;

    delete t;
    delete ot;
}