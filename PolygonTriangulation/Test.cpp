# include <algorithm>
# include <iostream>

# include "../../include/ipelib.h"
# include "Utils.hpp"

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);std::cout.tie(0);
    ipe::Vector v1,v2,v3,v4;
    std::cin>>v1.x>>v1.y;
    std::cin>>v2.x>>v2.y;
    std::cin>>v3.x>>v3.y;
    std::cin>>v4.x>>v4.y;

    std::cout<<(intersects(v1,v2,v3,v4) ? "intersecs!\n" : "not intersects!\n");
    std::cout<<(intersects2(v1,v2,v3,v4) ? "intersecs2!\n" : "not intersects2!\n");
    return 0;
}