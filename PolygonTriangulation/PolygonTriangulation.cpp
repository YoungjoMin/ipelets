# include "../../include/ipelib.h"
# include "PolygonTriangulation.hpp"
# include "Utils.hpp"

# include <vector>
# include <utility>
# include <algorithm>

using namespace ipe;
using namespace std;

class PolygonTriangulationIpelet : public Ipelet {
public:
    virtual int ipelibVersion() const {return IPELIB_VERSION;}
    virtual bool run(int num, IpeletData * data, IpeletHelper * helper);
};

IPELET_DECLARE Ipelet *newIpelet()
{
    return new PolygonTriangulationIpelet;
}

bool isSubpathClosedPolygon(const SubPath * subpath, IpeletHelper* helper) {
    if(!subpath->closed()) return false;
    if(subpath->type() != SubPath::ECurve) return false;

    const Curve* curve = subpath->asCurve();
    int curveCnt = curve->countSegments();

    //curveCnt+1-gon
    if(curveCnt+1<3) return false;

    for(int k =0;k<curveCnt;k++) {
        if(curve->segment(k).type() != CurveSegment::ESegment) return false;
    }
    return true;
}

void insertClosedPolygon(const SubPath* subpath, const Matrix& mat, vector<Vector>& pts, vector<Polygon>& polys) {
    const Curve* curve = subpath->asCurve();
    int n = curve->countSegments();
    //(n+1)-gon
    pts.push_back(mat * curve->segment(n-1).cp(1));
    polys.push_back({pts.size(),pts.size()+n});
    pts.push_back(mat * curve->segment(0).cp(0));
    for(int k = 0;k<n;k++) {
        pts.push_back(mat * curve->segment(k).cp(1));
    }
    pts.push_back(mat * curve->segment(0).cp(0));
}

bool getSelectedPolygons(IpeletData* data, IpeletHelper* helper, vector<Vector>& pts, vector<Polygon>& polys) {
    Page* page = data->iPage;
    int n = page->count();
    for(int i= 0;i<n;i++) {
        if(page->select(i) == TSelect::ENotSelected) continue;
        Object * obj = page->object(i);
        if(obj->type() != Object::EPath) continue;

        const Shape& shape = obj->asPath()->shape();
        int shapeCnt = shape.countSubPaths();
        if(shapeCnt != 1) continue;

        const SubPath * subpath =shape.subPath(0);
        if(!isSubpathClosedPolygon(subpath, helper)) continue;
        insertClosedPolygon(subpath, obj->matrix(), pts, polys);
    }
    if(polys.size()==0) {
        helper->messageBox("must choose polygons", NULL, IpeletHelper::EOkButton);
        return false;
    }
    return true;
}

Path * getSegmentAsPath(IpeletData * data, const Vector& p, const Vector& q) {
    return new Path(data->iAttributes,Shape(Segment(p,q)));
}

bool addGivenEdges(IpeletData * data, vector<Vector>& pts, vector<pair<int, int>>& edges) {
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

bool isClockwise(const vector<Vector>& pts, const Polygon& p) {
    int a = p.first, b =p.second;
    int minIdx = p.first;
    Vector min = pts[a];
    for(int i= a+1;i<=b;i++) {
        const Vector& cur = pts[i];
        if(min.x==cur.x ? min.y<cur.y : min.x<cur.x) continue;
        min = cur;
        minIdx = i;
    }
    return cross(pts[minIdx-1],pts[minIdx],pts[minIdx+1])<0;
}
void reverseOrder(vector<Vector>& pts, Polygon& p) {
    Vector * arr = pts.data();
    std::reverse(arr + (p.first-1), arr +  (p.second+2));
}
bool selfIntersecting(const vector<Vector>& pts, const Polygon& p) {
    for(int i= p.first;i<=p.second;i++) {
        if(intersects(pts[i],pts[i+1],pts[i+1],pts[i+2])) return true;
    }
    for(int i = p.first; i<=p.second; i++) {
        for(int j= i+2; j<=p.second;j++) {
            if( i==p.first && j==p.second ) continue;
            if(intersects2(pts[i],pts[i+1],pts[j],pts[j+1])) return true;
        }
    }
    return false;
}
void makeLeftmostFirst(const vector<Vector>& pts, std::vector<Polygon>& polygons) {
    int minIdx = 0;
    Vector min = pts[0];
    for(int i=0;i<(int)polygons.size();i++) {
        const Polygon& p = polygons[i];
        for(int j= p.first;j<=p.second;j++) {
            const Vector& cur = pts[j];
            if(min.x==cur.x ? min.y<=cur.y : min.x<=cur.x) continue;
            minIdx = i;
            min = cur;
        }
    }
    std::swap(polygons[minIdx],polygons[0]);
}

bool containedInterior(const vector<Vector>& pts, const Polygon& p, const Polygon& contained) {    
    //assume p, contained does not intersects
    //have to check whether contained is totally inside or totally outside polygon p

    Vector x= pts[contained.first];
    Vector p1,p2=pts[p.first-1]-x;
    int w =0;

    for(int i=p.first;i<=p.second;i++) {
        p1 = p2;
        p2 = pts[i]-x;

        if(p1.y*p2.y<0) {
            double tmp = cross(p2,p1);
            if(p1.y<0) w += tmp<0 ?  2 : 0;
            else       w += tmp>0 ? -2 : 0;
        }
        else if(p1.y==0 && p1.x>0) {
            w += p2.y>0 ? 1 : -1;
        }
        else if(p2.y==0 && p2.x>0) {
            w += p1.y<0 ? 1 : -1;
        }
    }
    return w!=0;
}



//make polygons counter clockwise order
//make big polygon be first
//check selfintersecting polygons check all other holes are inside interior
bool precompute(vector<Vector>& pts, vector<Polygon>& polygons, IpeletHelper * helper) {
    
    //line sweep?
    for(Polygon& p : polygons) {    
        if(isClockwise(pts, p)) reverseOrder(pts, p);
        if(!selfIntersecting(pts, p)) continue;
        helper->messageBox("Input polygons must not selfIntersecting",NULL, IpeletHelper::EOkButton);
        return false;
    }
    for(int i= 0;i<(int)polygons.size();i++) {
        for(int j= i+1;j<(int)polygons.size();j++) {
            const Polygon& p  =polygons[i];
            const Polygon& q  =polygons[j];
            for(int k = p.first;k<=p.second;k++){
                for(int w = q.first;w<=q.second;w++) {
                    if(!intersects2(pts[k],pts[k+1],pts[w],pts[w+1])) continue;
                    helper->messageBox("Input polygons must not Intersect each other",NULL, IpeletHelper::EOkButton);      
                    return false;
                }
            }
        }
    }
    makeLeftmostFirst(pts, polygons);
    const Polygon& p = polygons[0];
    for(int i= 1;i<(int)polygons.size();i++) {
        const Polygon& contained = polygons[i];
        if(containedInterior(pts, p, contained)) continue;
        helper->messageBox("Input polygons must have one big polygon and several inner polygon for holes",NULL, IpeletHelper::EOkButton);
        return false;
    }

    for(int i= 1;i<(int)polygons.size();i++) {
        for(int j= i+1;j<(int)polygons.size();j++) {
            const Polygon& poly1 = polygons[i];
            const Polygon& poly2 = polygons[j];
            if(!containedInterior(pts,poly1,poly2) && !containedInterior(pts,poly2,poly1)) continue;

            helper->messageBox("holes must not contain other hole",NULL, IpeletHelper::EOkButton);
            return false;
        }
    }
    return true;
}

bool PolygonTriangulationIpelet::run(int num, IpeletData * data, IpeletHelper * helper) {
    //const static std::vector<Triangulation> fn = {MinAreaMaximize, MaxAreaMinimize};
    //if(num<0 || num>=(int)fn.size()) return false;

    std::vector<Vector> pts;
    std::vector<Polygon> polygons;
    std::vector<std::pair<int, int>> edges;
    if(!getSelectedPolygons(data,helper, pts, polygons)) return false;
    if(!precompute(pts, polygons,helper)) return false;
    return true;
    //fn[num](pts,polygons, edges);
    addGivenEdges(data, pts, edges);
    return true;
}
