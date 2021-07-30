# include "../../include/ipelib.h"
# include "GeometricGraphs.hpp"
# include <algorithm>
# include <iostream>

using namespace ipe;


bool RunDelaunay(const std::vector<Vector>& pts) {
    std::vector<std::pair<int, int>> edges;
    if(!Delaunay(pts,edges)) return false;
    return true;
}

bool RunEMST(const std::vector<Vector>& pts) {
    std::vector<std::pair<int, int>> edges;
    if(!EMST(pts,edges)) return false;
    return true;
}

int main() {
    //std::vector<Vector> pts = {
    //    Vector(1,1), Vector(2,2), Vector(4,4), Vector(6,7), Vector(0,-1), Vector(1,5),
    //};
    std::vector<Vector> pts;
    for(int i : {1,5,9})
        for(int j : {1,11,21,31})
            pts.push_back(Vector(i,j));
    RunEMST(pts);
    return 0;
}
