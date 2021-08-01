label = "Polygon Triangulation"

about = "Computes Polygon Triangulation from selected Polygons"

-- this variable will store the C++ ipelet when it has been loaded
ipelet = false

function run(model, num)
  if not ipelet then ipelet = assert(ipe.Ipelet(dllname)) end
  model:runIpelet(methods[num].label, ipelet, num)
end

methods = {
    { label="min area maximize" },
    { label="max area minimize" },    
    { label="sum length maximize" },
    { label="sum length minimize" },
    { label="min angle maximize" },
    { label="max angle minimize" },    
}