label = "Sampling"

about = [[
sampling in axis parellel rectangle
]]

function wrongInput(model)
  model:warning("it requires axis-aligned rectangle")
end

function getRegion(obj, rec)
    local lbx = 1000000
    local lby = 1000000
    local rtx = -1000000
    local rty = -1000000
    local pts = {rec[1][1], rec[1][2], rec[2][2], rec[3][2]}
    for i = 1, #pts do
        local p0 = pts[i]
        lbx = math.min(lbx, p0.x)
        lby = math.min(lby, p0.y)
        rtx = math.max(rtx, p0.x)
        rty = math.max(rty, p0.y)
    end
    return ipe.Vector(lbx,lby), ipe.Vector(rtx, rty)
end

function isAxisAlligned(obj, rec, lb, rt)
    local pts = {rec[1][1], rec[1][2], rec[2][2], rec[3][2]}
    for i = 1, #pts do
        if pts[i].x ~= lb.x and pts[i].x ~= rt.x then return false end
        if pts[i].y ~= lb.y and pts[i].y ~= rt.y then return false end
    end
    return true
end

function randPos(lb,rt)
    local x = lb.x + math.random() * (rt.x - lb.x)
    local y = lb.y + math.random() * (rt.y - lb.y)
    return ipe.Vector(x,y)
end

function getSampleCnt(model)
    local str = model:getString("Enter number of sampling points")
    if not str or str:match("^%s*$)") then return end
    return tonumber(str)
end

function SamplingInRectangle(model)
    local p = model:page()
    local prim = p:primarySelection()
    if not prim then model.ui:explain("no selection") return end
    
    local obj = p[prim]
    if obj:type() ~= "path" then wrongInput(model) return end
    local shape = obj:shape()
    if #shape ~= 1 then wrongInput(model) return end

    local s = shape[1]
    if #s ~= 3 or s.type ~= "curve" or s.closed ~= true then wrongInput(model) return end
    lb, rt = getRegion(obj, s)
    if not isAxisAlligned(obj, s, lb, rt) then wrongInput(model) return end
    
    local k = getSampleCnt(model)
    if not k then model:warning("Enter a number") return end

    local samplePts = {}
    --math.randomseed(os.clock())
    for i = 1,k do
        local pt = randPos(lb, rt)
        samplePts[#samplePts + 1] = ipe.Reference(model.attributes, model.attributes.markshape, pt)
    end
    local obj = ipe.Group(samplePts)
    model:creation("create points", obj)
end

methods = {
    { label="InRectangle", run = SamplingInRectangle },
}