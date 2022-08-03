---@class helpfunc

---@type helpfunc
_M = {}

local constants = require "constants"
---@type Content
local content = require "content"
---@type hazel
local hazel = require "hazel"

_M.GenerateSupply = function()
    local type = math.random(1, 3)
    local canvaSize = hazel.GetCanvaSize()
    ---@type Entity
    ---@type ECS
    local ECS = require "ecs"
    local supply = ECS.CreateSupply(type,
                                    hazel.CreatePos(math.random(0, canvaSize.x - constants.TileSize),
                                                    math.random(0, canvaSize.y - constants.TileSize)))
    table.insert(content.SupplyList, supply)
end

_M.IncKillNum = function()
    content.KillNum = content.KillNum + 1
    if content.KillNum % constants.SupplyFalldownKillNum == 0 then
        _M.GenerateSupply()
    end
end

return _M