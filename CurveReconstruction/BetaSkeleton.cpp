#include "DelaunayDS.hpp"
#include "CurveReconstruction.hpp"
#include "Utils.hpp"

using namespace std;
using namespace ipe;

void getBetaEdges(const vector<Vector>& pts, const vector<Edge>& dedges, double beta, vector<Edge>& out) {
  out.clear();
  double bb = std::sqrt(beta*beta-1.0)/2;
  for(const auto & [a, b] : dedges) {
    Vector mid = (pts[a]+pts[b])*0.5;
    Vector per = (pts[b]-pts[a]).orthogonal()*bb;
    
    Vector c1 = mid+per, c2 = mid-per;
    double sqRad = (pts[b]-pts[a]).sqLen()*beta*beta*0.25;

    bool emptyRegion = true;
    for(int i= 0;i<(int)pts.size();i++) {
      if(i==a || i==b) continue;
      if(epsGT((pts[i]-c1).sqLen(),sqRad) &&
         epsGT((pts[i]-c2).sqLen(),sqRad)) continue;
      emptyRegion =false;
      break;      
    }

    if(emptyRegion) out.push_back({a,b});
  }
}

bool BetaSkeleton(const vector<Vector>& pts, vector<Edge>& edges, double beta) {
    vector<pair<int,int>> dedges;
    Delaunay(pts, dedges);
    getBetaEdges(pts, dedges, beta, edges);
    return true;
}