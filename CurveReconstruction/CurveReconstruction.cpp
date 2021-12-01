# include "../../include/ipelib.h"
# include "CurveReconstruction.hpp"
# include "Utils.hpp"

# include <vector>
# include <utility>
# include <algorithm>

using namespace ipe;
using namespace std;


class CurveReconstructionIpelet : public Ipelet {
public:
    virtual int ipelibVersion() const {return IPELIB_VERSION;}
    virtual bool run(int num, IpeletData * data, IpeletHelper * helper);
};

IPELET_DECLARE Ipelet *newIpelet()
{
    return new CurveReconstructionIpelet;
}

bool getSelectedPoints(IpeletData* data, IpeletHelper* helper, std::vector<Vector>& pts) {
    Page* page = data->iPage;
    int n = page->count();
    for(int i= 0;i<n;i++) {
        if(page->select(i) == TSelect::ENotSelected) continue;
        Object * obj = page->object(i);
        if(obj->type() != Object::EReference) continue;
        Reference * ref = obj->asReference();
        Vector cur = obj->matrix() * ref->position();
        pts.push_back(cur);
    }
    if(pts.size()<2) {
        helper->messageBox("have to choose at least two points", NULL, IpeletHelper::EOkButton);
        return false;
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

bool CurveReconstructionIpelet::run(int num, IpeletData * data, IpeletHelper * helper) {
    const static std::vector<Func> fn = {Crust};
    if(num<0 || num>=(int)fn.size()) return false;

    std::vector<Vector> pts;
    std::vector<std::pair<int, int>> edges;
    if(!getSelectedPoints(data,helper, pts)) return false;
    fn[num](pts,edges);
    addGivenEdges(data, pts,edges);
    return true;
}