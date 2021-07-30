# include "GeometricGraphs.hpp"

using namespace ipe;
using namespace std;


void makeRNG(const vector<Vector>& pts, const vector<pair<int, int>>& edges, vector<pair<int, int>>& rng) {
    for(const auto& [a,b]: edges) {
        double limit = (pts[a]-pts[b]).sqLen();

        bool emptyLune = true;
        for(int i = 0;i<(int)pts.size();i++) {
            if(i==a || i==b) continue;
            const Vector& pt = pts[i];

            double cur = std::max((pts[a]-pt).sqLen(),(pts[b]-pt).sqLen());
            if(cur>=limit) continue;
            emptyLune = false;
            break;
        }
        if(emptyLune) rng.push_back({a,b});
    }
}

bool RNG(const vector<Vector>& pts, vector<pair<int, int>>& edges) {
    vector<pair<int,int>> dedges;
    Delaunay(pts,dedges);
    makeRNG(pts,dedges, edges);
    return true;
}