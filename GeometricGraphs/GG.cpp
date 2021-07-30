# include "GeometricGraphs.hpp"

using namespace ipe;
using namespace std;

void makeGG(const vector<Vector>& pts, const vector<pair<int, int>>& edges, vector<pair<int, int>>& gg) {
    for(const auto& [a,b]: edges) {
        Vector mid = (pts[a]+pts[b])*0.5;
        double limit = (pts[a]-mid).sqLen();

        
        bool emptyDisc = true;
        for(int i= 0;i<(int)pts.size();i++) {
            if(i==a || i==b) continue;
            const Vector& pt = pts[i];

            double cur = (mid-pt).sqLen();
            if(cur>=limit) continue;
            emptyDisc = false;
            break;
        }
        if(emptyDisc) gg.push_back({a,b});
    }
}

bool GG(const vector<Vector>& pts, vector<pair<int, int>>& edges) {
    vector<pair<int,int>> dedges;
    Delaunay(pts,dedges);
    makeGG(pts,dedges, edges);
    return true;
}