# include "GeometricGraphs.hpp"

# include <utility>
# include <vector>
# include <numeric>

using namespace std;
using namespace ipe;

struct Edge {
    int a, b;
    double w;
    bool operator <(const Edge& o){
        return w<o.w;
    }
};

//union find
static std::vector<int> parent;
static std::vector<Edge> E;

void initUnionFind(int n) {
    parent.resize(n+1);
    iota(parent.begin(),parent.end(), 0);
}

int find(int a) {
    if(a==parent[a]) return a;
    else return parent[a]=find(parent[a]);
}

void merge(int a, int b) {
    int aa= find(a);
    int bb= find(b);
    if(aa==bb) return;
    parent[aa]=bb;
}

void MST(const vector<Vector>& pts, const vector<pair<int, int>>& edges, vector<pair<int, int>>& mst) {
    initUnionFind(pts.size());
    for(const auto& [a,b] : edges)
        E.push_back({a,b, (pts[a]-pts[b]).sqLen()});
    sort(E.begin(),E.end());
    for(const Edge& e : E) {
        int aa = find(e.a);
        int bb = find(e.b);
        if(aa==bb) continue;
        merge(aa,bb);
        mst.push_back({e.a, e.b});
    }
    E.clear(); parent.clear();
}

bool EMST(const vector<Vector>& pts, vector<pair<int, int>>& edges) {
    vector<pair<int,int>> dedges;
    Delaunay(pts,dedges);
    MST(pts,dedges, edges);
    return true;
}