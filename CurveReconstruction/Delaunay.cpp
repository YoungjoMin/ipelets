# include "../../include/ipelib.h"
# include "CurveReconstruction.hpp"
# include "DelaunayDS.hpp"
# include "Utils.hpp"
# include <vector>
# include <ctime>
# include <random>
# include <queue>
# include <numeric>

using namespace ipe;

//all points want to add at Delaunay triangulation
//contained here
static std::vector<Vector> pts;
static std::mt19937 gen((unsigned int)time(NULL));


bool inCircle(int a, int b, int c, int pt) {
    return inCircle(pts[a],pts[b],pts[c],pts[pt]);
}
bool ccw(int a, int b, int c) {
    return ccw(pts[a],pts[b],pts[c]);
}
bool sccw(int a, int b, int c) {
    return sccw(pts[a],pts[b],pts[c]);
}

/////////////////////////////////////////////////
//impl

void Triangle::updateAdj(Triangle * prv, Triangle* cur) {
    if(adj[0]==prv) adj[0]=cur;
    if(adj[1]==prv) adj[1]=cur;
    if(adj[2]==prv) adj[2]=cur;
    return;
}

Node::Node(int p1, int p2, int p3): p1(p1), p2(p2), p3(p3) {
    c1=c2=c3= NULL; t=NULL;
    divideBy = NotDivided;
}

bool Node::isLeaf() const {
    return divideBy==NotDivided;
}

Vector Node::getCircumCenter() const {
    //TODO
    Vector d1 = (pts[p1]-pts[p2]).orthogonal(), c1 = (pts[p1]+pts[p2])*0.5;
    Vector d2 = (pts[p2]-pts[p3]).orthogonal();//, c2 = (pts[p2]+pts[p3])*0.5;
    Vector c = (pts[p3]-pts[p1])*0.5;

    //c1 + xd1 = c2 + yd2;
    //(d1 -d2)  (x y)^T = c2-c1
    double x_num = d2.x* c.y - d2.y*c.x;
    double x_den = d1.y*d2.x - d1.x*d2.y;

    return c1 + d1*(x_num/x_den);
}

PointLocation::PointLocation(const Vector& bl, const Vector& tr) {
    Vector nbl = bl - (tr-bl)*16;
    Vector ntr = tr + (tr-bl)*16;

    double &a = ntr.x,&b=ntr.y, &c=nbl.x,&d=nbl.y;

    //Giant triangle
    int n = pts.size();
    pts.push_back( Vector((a+c)/2, b + (a-c)/2));
    pts.push_back( Vector(c - (b-d), d));
    pts.push_back( Vector(a + (b-d), d));

    NILNODE = new Node(-1,-1,-1);
    NILT = new Triangle();
    NILT->node = NILNODE, NILNODE->t = NILT;
    NILT->adj[0]=NILT->adj[1]=NILT->adj[2]= NILT;

    Triangle* t = new Triangle();
    Node * node = new Node(n, n+1, n+2);
    valid_node.push_back(node);
    t->node = node;
    t->adj[0]=t->adj[1]=t->adj[2] = NILT;
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
        if(b1&&!b2) return c1;
        else if(b2&&!b3) return c2;
        return c3;
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
    Node * dnode = new Node(bidx, aidx, didx);
    valid_node.push_back(unode);
    valid_node.push_back(dnode);

    ut->node = unode;
    ut->adj[0]=dt, ut->adj[1]=t4, ut->adj[2]=t1;

    dt->node = dnode;
    dt->adj[0]=ut, dt->adj[1]=t2, dt->adj[2]=t3;
    
    t1->updateAdj(t, ut);
    t2->updateAdj(t, dt);
    t3->updateAdj(ot, dt);
    t4->updateAdj(ot, ut);    

    unode->t = ut;
    dnode->t = dt;

    //////////////
    node->pt = bidx; onode->pt = aidx;
    node->divideBy = (Node::Divide)(node->getOppositeEdge(aidx));
    onode->divideBy = (Node::Divide)(onode->getOppositeEdge(bidx));
    node->t = onode->t = NULL;
    node->c1= dnode, node->c2 = unode;
    onode->c1 = unode, onode->c2 = dnode;    

    delete t;
    delete ot;
}

void Node::pushValidEdges(std::vector<std::pair<int, int>>& edges, int limit) const {
    if(p1<p2 && p1<limit && p2<limit) edges.push_back({p1,p2});
    if(p2<p3 && p2<limit && p3<limit) edges.push_back({p2,p3});
    if(p3<p1 && p3<limit && p1<limit) edges.push_back({p3,p1});
}

void PointLocation::gatherAllEdges(std::vector<std::pair<int, int>>& edges, int limit) const {
    for(const Node * node : valid_node) {
        if(!node->isLeaf()) continue;
        node->pushValidEdges(edges,limit);
    }
    return;
}

void PointLocation::gatherAllVoronoiVertices(std::vector<ipe::Vector>& vpts, int limit) const {
    for(const Node * node : valid_node) {
      if(!node->isLeaf()) continue;
      vpts.push_back(node->getCircumCenter());
    }
}

PointLocation::~PointLocation() 
{
    for(const Node * node : valid_node) {
        if(node->isLeaf())
            delete node->t;
        delete node;
    }
    delete NILNODE;
    delete NILT;
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
    valid_node.push_back(node1);
    valid_node.push_back(node2);
    valid_node.push_back(node3);

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

    legalize(node1, idx);
    legalize(node2, idx);
    legalize(node3, idx);    

    delete t;
}
void PointLocation::insertPointEdge(Node* node, int idx, int edge) {   
    Node * onode = node->getAdjNode(edge);

    int tmp[] = {node->p3, node->p1, node->p2, node->p3, node->p1};
    int aidx = tmp[edge+1], bidx = tmp[edge-1], cidx = tmp[edge];
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
    valid_node.push_back(node1);
    valid_node.push_back(node2);
    valid_node.push_back(node3);
    valid_node.push_back(node4);

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

    ///////////////
    legalize(node1, idx);
    legalize(node2, idx);
    legalize(node3, idx);
    legalize(node4, idx);
    

    delete t;
    delete ot;
}

////////////////////////////
//Delaunay
void getBoundingBox(const std::vector<Vector>& pts, Vector& bl, Vector& tr) {
    if(pts.size()==0) {bl=Vector(-1,-1),tr=Vector(1,1); return;}
    double b,l,t,r;
    b=t=pts[0].y, l=r=pts[0].x;
    for(const Vector& v : pts) {
        b = std::min(b, v.y);
        l = std::min(l, v.x);

        t = std::max(t, v.y);
        r = std::max(r, v.x);
    }
    bl = Vector(l, b);
    tr = Vector(r, t);
    return;
}

bool Delaunay(const std::vector<ipe::Vector>& npts, std::vector<std::pair<int, int>>& edges)
{
    pts.clear();
    pts.insert(pts.begin(), npts.begin(),npts.end());
    Vector bl, tr;
    getBoundingBox(npts, bl, tr);
    PointLocation pointlocation (bl,tr);

    std::vector<int> order(npts.size());
    std::iota(order.begin(),order.end(),0);
    std::shuffle(order.begin(), order.end(), gen);

    for(int cur : order) {
        pointlocation.insert(cur);
    }
    pointlocation.gatherAllEdges(edges, npts.size());
    pts.clear();
    return true;
}

bool VoronoiVertices(const std::vector<ipe::Vector>& npts, std::vector<ipe::Vector>& vpts)
{
    pts.clear();  
    pts.insert(pts.begin(), npts.begin(),npts.end());
    Vector bl, tr;
    getBoundingBox(npts, bl, tr);
    PointLocation pointlocation (bl,tr);

    std::vector<int> order(npts.size());
    std::iota(order.begin(),order.end(),0);
    std::shuffle(order.begin(), order.end(), gen);

    for(int cur : order) {
        pointlocation.insert(cur);
    }
    pointlocation.gatherAllVoronoiVertices(vpts, npts.size());
    pts.clear();
    return true;
}

