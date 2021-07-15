# include "include/ipelib.h"
# include <vector>

using namespace ipe;

class GeometricGraphsIpelet : public Ipelet {
public:
    virtual int ipelibVersion() const {return IPELIB_VERSION;}
    virtual bool run(int, IpeletData * data, IpeletHelper * helper);
};

IPELET_DECLARE Ipelet *newIpelet()
{
    return new GeometricGraphsIpelet;
}
