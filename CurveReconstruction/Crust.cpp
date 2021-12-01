#include "DelaunayDS.hpp"
#include "CurveReconstruction.hpp"

using namespace std;
using namespace ipe;


void getCrustEdges(const vector<Vector>& pts, const vector<Edge>& dedges, int n, vector<Edge>& out) {
  out.clear();
  for(const auto & [a, b] : dedges) {
    if(a>=n || b>=n) continue;
    out.push_back({a,b});
  }
}

bool Crust(vector<Vector>& pts, vector<Edge>& edges) {
    int n = pts.size();
    
    vector<Vector> vpts;
    VoronoiVertices(pts, vpts);
    pts.insert(pts.end(), vpts.begin(),vpts.end());

    vector<pair<int,int>> dedges;
    Delaunay(pts, dedges);
    getCrustEdges(pts, dedges, n, edges);

    return true;
}