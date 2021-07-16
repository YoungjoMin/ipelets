# include "include/ipelib.h"
# include "Delaunay.hpp"
# include "DelaunayDS.hpp"

using namespace ipe;

bool RunDelaunay(IpeletData * data, IpeletHelper * helper);

class GeometricGraphsIpelet : public Ipelet {
public:
    virtual int ipelibVersion() const {return IPELIB_VERSION;}
    virtual bool run(int num, IpeletData * data, IpeletHelper * helper);
};

IPELET_DECLARE Ipelet *newIpelet()
{
    return new GeometricGraphsIpelet;
}

bool GeometricGraphsIpelet::run(int num, IpeletData * data, IpeletHelper * helper) {
    switch(num) {
        case 1: return RunDelaunay(data,helper);
        default: break;
    }
    return false;
}

bool printDebugInfoAndReturn(IpeletHelper* helper, const char * str) {
    helper->message(str);
    return false;
}

bool getSelectedPoints(IpeletData* data, IpeletHelper* helper, std::vector<Vector>& pts) {
    Page* page = data->iPage;
    int n = page->count();
    for(int i= 0;i<n;i++) {
        if(page->select(i) == TSelect::ENotSelected) continue;
        Object * obj = page->object(i);
        if(obj->type() != Object::EReference) 
            return printDebugInfoAndReturn(helper, "selected Objects have to be reference");
        Reference * ref = obj->asReference();
        Vector cur = obj->matrix() * ref->position();
        pts.push_back(cur);
    }
    return true;
}

Path * getSegmentAsPath(IpeletData * data, const Vector& p, const Vector& q) {
    return new Path(data->iAttributes,Shape(Segment(p,q)));
}

bool addGivenEdges(IpeletData * data, std::vector<Vector>& pts, std::vector<std::pair<int, int>>& edges) {
    Group * group = new Group();
    for(auto [i,j] : edges) {
        group->push_back(
            getSegmentAsPath(data, pts[i],pts[j])
        );
    }
    Page * page = data->iPage;
    page->append(ESecondarySelected, data->iLayer, group);
    return true;
}

bool RunDelaunay(IpeletData * data, IpeletHelper * helper) {
    std::vector<Vector> pts;
    std::vector<std::pair<int, int>> edges;
    if(!getSelectedPoints(data,helper, pts)) return false;
    if(!Delaunay(pts,edges)) return false;
    if(!addGivenEdges(data, pts,edges)) return false;
    return true;
}