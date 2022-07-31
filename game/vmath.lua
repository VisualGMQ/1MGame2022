local math = require "math"
local hazel = require "hazel"
local _M = {}

---@return number
---@param p Point
function _M.Len(p)
    return math.sqrt(p.x * p.x + p.y * p.y)
end

---@return Point
---@param p
function _M.Normalize(p)
    local l = _M.Len(p)
    return hazel.CreatePos(p.x / l, p.y / l)
end

---@return boolean
---@param r1 Rect
---@param r2 Rect
function _M.IsRectIntersect(r1, r2)
    return not (r1.x + r1.w < r2.x or
                r2.x + r2.w < r1.x or
                r2.y + r2.h < r1.y or
                r1.y + r1.h < r2.y)
end

return _M