# include "../../include/ipelib.h"
# include "PolygonTriangulation.hpp"
# include "Utils.hpp"
/* without holes */
# include <vector>
# include <utility>

using namespace std;
using namespace ipe;


using Combine = double (*) (double, double, double);
using Compare = bool (*) (double, double); //compare(a,b) = true => a is better, false => b is better
using Calc    = double (*) (const Vector& , const Vector&, const Vector&);


struct Solver{    
    const vector<Vector>& pts;
    vector<vector<double>> dp;
    vector<vector<int>> dpChoose;
    vector<vector<bool>> haveLine;

    double dpinit;
    Combine combine;
    Compare compare;
    Calc calc;

    int N;

    Solver(const vector<Vector>& pts, double dpinit, Combine combine, Compare compare, Calc calc)
     : pts(pts), dpinit(dpinit), combine(combine), compare(compare), calc(calc) {
        N = pts.size()-2;
        initHaveLine();
        initDP();
    }
    void getEdges(vector<Edge>& edge) {
        int c = dpChoose[0][N-1];
        getEdgesHelper(edge, 0,c);
        getEdgesHelper(edge, c,N-1);
        return;
    }
private:
    bool calcHaveLineAt(int i, int j) {
        if( i+1 == j) return true;
       	for (int k = 0; k < N; k++) {
		    if (intersects(pts[i+1],pts[j+1], pts[k],pts[k+1])) 
			    return false;
	    }

        //check diagonal is totally inside or totally outside
        bool c1 = ccw(pts[i+1],pts[i],pts[i+2])<=0;
        bool c2 = ccw(pts[i+1],pts[i],pts[j+1])<=0;
        bool c3 = ccw(pts[i+1],pts[j+1],pts[i+2])<=0;

        if(c1&&c2&&c3) return true;
        if(!c1 && (c2||c3)) return true;
        return false;
    }

    double calcDPAt(int i, int j) {
        double ret = dpinit;
        for(int k= i+1; k!= j; k++) {
            if(!haveLine[i][k] || !haveLine[k][j]) continue;
            double cur = combine(dp[i][k],dp[k][j],
                calc(pts[i+1],pts[k+1],pts[j+1])
            );
            if(compare(ret,cur)) continue;
            ret = cur;
            dpChoose[i][j]=k;
        }
        return ret;
    }

    void initHaveLine() {
        haveLine.clear();
        for(int i= 0;i<N;i++) haveLine.push_back(std::vector<bool>(N, false));

        for(int i= 0;i<N;i++) {
            for(int j= i+1;j<N;j++) {
                haveLine[i][j] = calcHaveLineAt(i,j);
            }
        }
    }
    void initDP() {        
        dp.clear(); dpChoose.clear();
        for(int i= 0;i<N;i++) {
            dp.push_back(std::vector<double>(N, dpinit));
            dpChoose.push_back(std::vector<int>(N, 0));
        }

        for(int len = 2;len<=N-2;len++) {
            for(int s= 0,e=len;e<N;s++,e++) {
                if(!haveLine[s][e]) continue;
                dp[s][e] = calcDPAt(s,e);
            }
        }

        dp[0][N-1] = calcDPAt(0,N-1);
    }
    void getEdgesHelper(vector<Edge>& edge, int a, int b) {
        if(a+1 == b) return;
        edge.push_back({a+1,b+1});
        int c = dpChoose[a][b];
        getEdgesHelper(edge, a,c);
        getEdgesHelper(edge, c,b);
    }
};



void MinAreaMaximize(const vector<Vector>& pts, const vector<Polygon>& polys, vector<Edge>& edges) {
    auto combine = [](double d1, double d2, double d3) { 
        double tmp = std::max({d1,d2,d3});
        d1 = d1<0 ? tmp : d1;
        d2 = d2<0 ? tmp : d2;
        d3 = d3<0 ? tmp : d3;
        return std::min({d1,d2,d3});
    };
    auto compare = [](double d1, double d2) {return d1>d2;};
    auto calc = [] (const Vector& v1, const Vector& v2, const Vector& v3) {
        return std::abs(ccw(v1,v2,v3));
    };
    Solver solver(pts,-1,combine, compare, calc);
    solver.getEdges(edges);
}

void MaxAreaMinimize(const vector<Vector>& pts, const vector<Polygon>& polys, vector<Edge>& edges) {
    auto combine = [](double d1, double d2, double d3) { return std::max({d1,d2,d3});};
    auto compare = [](double d1, double d2) {return 0<d1 && d1<=d2;};
    auto calc = [] (const Vector& v1, const Vector& v2, const Vector& v3) {
        return std::abs(ccw(v1,v2,v3));
    };
    Solver solver(pts,-1,combine, compare, calc);
    solver.getEdges(edges);
}

void SumLengthMaximize(const vector<Vector>& pts, const vector<Polygon>& polys, vector<Edge>& edges) {
    auto combine = [](double d1, double d2, double d3) { return d1+d2+d3;};
    auto compare = [](double d1, double d2) {return d1>d2;};
    auto calc = [] (const Vector& v1, const Vector& v2, const Vector& v3) {
        return (v1-v2).len() + (v2-v3).len() + (v3-v1).len();
    };
    Solver solver(pts,0.0,combine, compare, calc);
    solver.getEdges(edges);
}

void SumLengthMinimize(const vector<Vector>& pts, const vector<Polygon>& polys, vector<Edge>& edges) {
    auto combine = [](double d1, double d2, double d3) { return d1+d2+d3;};
    auto compare = [](double d1, double d2) {return 0<d1 && d1<=d2;};
    auto calc = [] (const Vector& v1, const Vector& v2, const Vector& v3) {
        return (v1-v2).len() + (v2-v3).len() + (v3-v1).len();
    };
    Solver solver(pts,0.0,combine, compare, calc);
    solver.getEdges(edges);
}

void MinAngleMaximize(const vector<Vector>& pts, const vector<Polygon>& polys, vector<Edge>& edges) {
    //Min -cos(Angle) Maximize
    auto combine = [](double d1, double d2, double d3) { 
        double tmp = std::max({d1,d2,d3});
        d1 = d1<-1.5 ? tmp : d1;
        d2 = d2<-1.5 ? tmp : d2;
        d3 = d3<-1.5 ? tmp : d3;
        return std::min({d1,d2,d3});
    };
    auto compare = [](double d1, double d2) {return d1>d2;};
    auto calc = [] (const Vector& v1, const Vector& v2, const Vector& v3) {
        Vector t1 = v1-v2, t2 = v2-v3, t3 = v3-v1;
        double d1 = t1.len(), d2 = t2.len(), d3 = t3.len();
        return std::min({dot(t1,t2)/d1/d2, dot(t2,t3)/d2/d3, dot(t3,t1)/d3/d1});
    };
    Solver solver(pts,-2.0,combine, compare, calc);
    solver.getEdges(edges);
}

void MaxAngleMinimize(const vector<Vector>& pts, const vector<Polygon>& polys, vector<Edge>& edges) {
    //Max -cos(Angle) Minimize
    auto combine = [](double d1, double d2, double d3) { return std::max({d1,d2,d3}); };
    auto compare = [](double d1, double d2) {return -1.5<d1 && d1<d2;};
    auto calc = [] (const Vector& v1, const Vector& v2, const Vector& v3) {
        Vector t1 = v1-v2, t2 = v2-v3, t3 = v3-v1;
        double d1 = t1.len(), d2 = t2.len(), d3 = t3.len();
        return std::max({dot(t1,t2)/d1/d2, dot(t2,t3)/d2/d3, dot(t3,t1)/d3/d1});
    };
    Solver solver(pts,-2.0,combine, compare, calc);
    solver.getEdges(edges);
}