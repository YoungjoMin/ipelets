label = "Geometric Graphs"

about = "Computes Geomtric Graphs"

-- this variable will store the C++ ipelet when it has been loaded
ipelet = false

function run(model, num)
  if not ipelet then ipelet = assert(ipe.Ipelet(dllname)) end
  model:runIpelet(methods[num].label, ipelet, num)
end

methods = {
    { label="Delaunay Triangulation" },
    { label="Gabriel Graph" },
    { label="Relative Neighborhood Graph" },
    { label="Euclidean Minimum Spanning Tree" },
}