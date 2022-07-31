local hazel = require "hazel"
local constants = require "constants"
local math = require "math"
local vmath = require "vmath"
local content = require "content"

---@class ECS
local _M = {}

---@class Entity
---@field name string
---@field component table<Component>
local Entity = { name = "", components = {}}

---@param component Component
function Entity.SetComponent(self, component)
    if not component then
        return
    end
    ---@type Component
    local oldComp = self.components[component:GetName()]
    if oldComp then
        oldComp.parent = nil
        hazel.Log.Logw(string.format("component %s already exists, replace it", oldComp:GetName()))
    end
    component.parent = self
    self.components[component:GetName()] = component
end

---@return Component|nil
---@param type string
function Entity.GetComponent(self, type)
    if not type then
        return nil
    end
    return self.components[type]
end

---@param type string
function Entity.RemoveComponent(self, type)
    self.components[type] = nil
end

_M.Entity = Entity

---@return Entity
---@param name string
function _M.CreateEntity(name)
    local o = {name = name, components = {}}
    setmetatable(o, {__index = Entity})
    return o
end

---@param self Entity
function Entity.Update(self)
    ---@param v Component
    for _, v in pairs(self.components) do
        if v:IsActive() and v.Update then
            v:Update()
        end
    end
end

---@class Component
local Component = {
    ---@param self Component
    ---@return boolean
    IsActive = function(self)
        return self.isActive
    end,

    ---@param self Component
    Disable = function(self)
        self.isActive = false
    end,

    ---@param self Component
    Enable = function(self)
        self.isActive = true
    end,

    ---@param self Component
    ---@return string
    GetName = function(self)
        return self.name
    end,

    ---@param self Component
    ---@return Entity
    GetParent = function(self)
        return self.parent
    end,
}

---@return Component
function _M.CreateComponent(o)
    local o = o or {}
    setmetatable(o , {__index = Component})
    return o
end

---@class ComponentType
local ComponentType = {
    Transform = "Transform",
    Image = "Image",
    Controller = "Controller",
    RoleProp = "RoleProp",
    HpShow = "HpShow",
    Gun = "Gun",
    Bullet = "Bullet",
    Direction = "Direction",
    AI = "AI",
    ColliBox = "ColliBox",
    Invincible = "Invincible",
}

_M.ComponentType = ComponentType

---@class TransformComponent:Component
---@field position Point
---@field size Point
---@field flip number

---@return TransformComponent
---@param pos Point
---@param size Point
---@param flip number|nil
function _M.CreateTransformComponent(pos, size, flip)
    local o = { isActive = true, name = ComponentType.Transform, position = pos, size = size, flip = flip or Flip.None, parent = nil }
    o.Update = function () end
    return _M.CreateComponent(o)
end

---@class ImageComponent:Component
---@field tilesheet TileSheet
---@field row number
---@field col number

---@return ImageComponent
---@param tilesheet TileSheet
---@param col number
---@param row number
function _M.CreateImageComponent(tilesheet, col, row)
    local o = { isActive = true, name = ComponentType.Image, tilesheet = tilesheet, col = col, row = row, parent = nil }

    ---@param self ImageComponent
    o.Update = function(self)
        ---@type TransformComponent
        local transform = self:GetParent():GetComponent(ComponentType.Transform)
        if tilesheet then
            tilesheet:Draw(self.col or 0, self.row or 0,
                           hazel.CreateRect(transform.position.x, transform.position.y, transform.size.x, transform.size.y),
                           transform.flip)
        end
    end

    return _M.CreateComponent(o)
end

---@class DirectionComponent:Component
---@field direction Point

function _M.CreateDirectionComponent(startRow)
    local o = { isActive = true, name = ComponentType.Direction, direction = hazel.CreatePos(0, 0), startRow = startRow, parent = nil }

    ---@param self DirectionComponent
    ---@param dir Point
    o.SetDir = function(self, dir)
        self.direction = dir
    end

    ---@param self DirectionComponent
    o.Update = function(self)
        local image = self:GetParent():GetComponent(ComponentType.Image)
        if not image then
            return
        end

        local absX = math.abs(self.direction.x)
        local absY = math.abs(self.direction.y)
        if self.direction.y <= 0 and absX <= absY then
            image.col = 0
            image.row = 1 + self.startRow
        end
        if self.direction.y > 0 and absX <= absY then
            image.col = 0
            image.row = 0 + self.startRow
        end
        if self.direction.x < 0 and absY < absX then
            image.col = 0
            image.row = 2 + self.startRow
        end
        if self.direction.x > 0 and absY < absX then
            image.col = 0
            image.row = 3 + self.startRow
        end

    end

    return _M.CreateComponent(o)
end

---@class ControllerComponent:Component

---@return ControllerComponent 
---@param speed number
function _M.CreateControllerComponent()
    local o = { isActive = true, name = ComponentType.Controller, parent = nil }

    ---@param self ControllerComponent
    o.Update = function(self)
        ---@type RolePropComponent
        local roleProp = self:GetParent():GetComponent(ComponentType.RoleProp)
        local speed = roleProp.speed

        ---@type TransformComponent
        local transform = self:GetParent():GetComponent(ComponentType.Transform)
        local elapseTime = hazel.Time.GetElapseTime()
        if hazel.IsKeyPressing(hazel.Key.A) then
            transform.position.x = transform.position.x - speed * elapseTime
        end
        if hazel.IsKeyPressing(hazel.Key.D) then
            transform.position.x = transform.position.x + speed * elapseTime
        end
        if hazel.IsKeyPressing(hazel.Key.W) then
            transform.position.y = transform.position.y - speed * elapseTime
        end
        if hazel.IsKeyPressing(hazel.Key.S) then
            transform.position.y = transform.position.y + speed * elapseTime
        end

        ---@type Point
        local mousePos = hazel.GetGameMousePos()

        local playerCenterX = transform.position.x
        local playerCenterY = transform.position.y
        playerCenterX = playerCenterX + constants.TileSize / 2
        playerCenterY = playerCenterY + constants.TileSize / 2
        ---@type Point
        local mouseDir = hazel.CreatePos(mousePos.x - playerCenterX, mousePos.y - playerCenterY)
        self:GetParent():GetComponent(ComponentType.Direction):SetDir(mouseDir)
    end

    return _M.CreateComponent(o)
end

---@class InvincibleComponent:Component
---@field time number

---@return InvincibleComponent
---@param time number
function _M.CreateInvincibleComponent(time)
    local o = { isActive = true, name = ComponentType.Invincible, cooldown = time, timeCountDown = time, parent = nil }

    ---@return boolean
    ---@param self InvincibleComponent
    o.IsInvincibleState = function(self)
        return self.timeCountDown > 0
    end

    ---@param self InvincibleComponent
    o.IntoInvincible = function(self)
        if self.timeCountDown <= 0 then
            self.timeCountDown = self.cooldown
        end
    end

    ---@param self InvincibleComponent
    o.Update = function(self)
        self.timeCountDown = self.timeCountDown - hazel.Time.GetElapseTime()
        if self.timeCountDown < 0 then
            self.timeCountDown = 0
        end
    end

    return _M.CreateComponent(o)
end

---@class RolePropComponent:Component
---@field hp number
---@field maxHp number
---@field speed number
---@field damage number

---@return RolePropComponent
---@param hp number
---@param speed number
---@param damage number|nil
function _M.CreateRolePropComponent(hp, speed, damage)
    local o = { isActive = true, name = ComponentType.RoleProp, maxHp = hp, hp = hp, speed = speed, damage = damage or 0, parent = nil }
    ---@param self RolePropComponent
    o.IsDie = function(self)
        return self.hp <= 0
    end
    o.Update = function(self)
        if self.hp < 0 then
            self.hp = 0
        end
    end
    return _M.CreateComponent(o)
end

---@class HpShowComponent:Component
---@field size Point

---@return HpShowComponent
---@param size Point
function _M.CreateHpShowComponent(size)
    local o = { isActive = true, name = ComponentType.HpShow, size = size, parent = nil }

    ---@param self HpShowComponent
    o.Update = function(self)
        ---@type Point
        local position = self:GetParent():GetComponent(ComponentType.Transform).position
        local pos = hazel.CreatePos(position.x, position.y - self.size.y - 2)

        ---@type RolePropComponent
        local roleProp = self:GetParent():GetComponent(ComponentType.RoleProp)
        hazel.Renderer.SetDrawColor(1, 1, 1, 1) 
        hazel.Renderer.FillRect(pos.x, pos.y, size.x, size.y)

        hazel.Renderer.SetDrawColor(0, 1, 0, 1) 
        hazel.Renderer.FillRect(pos.x, pos.y, size.x * roleProp.hp / roleProp.maxHp, size.y)

        hazel.Renderer.SetDrawColor(0, 0, 0, 1) 
        hazel.Renderer.DrawRect(pos.x, pos.y, size.x, size.y)
    end
    return _M.CreateComponent(o)
end


---@class AIComponent:Component

---@return AIComponent
function _M.CreateAIComponent()
    local o = { isActive = true, name = ComponentType.AI, parent = nil }

    ---@param self AIComponent
    o.Update = function(self)
        ---@type DirectionComponent
        local direction = self:GetParent():GetComponent(ComponentType.Direction)

        local playerCenterX = content.PlayerEntity:GetComponent(ComponentType.Transform).position.x + constants.TileSize / 2
        local playerCenterY = content.PlayerEntity:GetComponent(ComponentType.Transform).position.y + constants.TileSize / 2
        local myselfCenterX = self:GetParent():GetComponent(ComponentType.Transform).position.x + constants.TileSize / 2
        local myselfCenterY = self:GetParent():GetComponent(ComponentType.Transform).position.y + constants.TileSize / 2

        ---@type Point
        local dir = vmath.Normalize(hazel.CreatePos(playerCenterX - myselfCenterX, playerCenterY - myselfCenterY))
        direction:SetDir(dir)

        ---@type Point
        local position = self:GetParent():GetComponent(ComponentType.Transform).position
        local velocity = self:GetParent():GetComponent(ComponentType.RoleProp).speed
        local elapseTime = hazel.Time.GetElapseTime()
        position.x = position.x + velocity * dir.x * elapseTime
        position.y = position.y + velocity * dir.y * elapseTime
    end

    return _M.CreateComponent(o)
end

---@class GunComponent:Component
---@field damage number
---@field speed Point

---@return GunComponent
---@param damage number
---@param velocity number
function _M.CreateGunComponent(damage, velocity)
    local o = { isActive = true, name = ComponentType.Gun, damage = damage, timeCounter = 0, parent = nil }
    ---@param self GunComponent
    o.Update = function(self)
        if self.timeCounter < constants.GunInfo.cooldown then
            self.timeCounter = self.timeCounter + hazel.Time.GetElapseTime()
        end
        if self.timeCounter >= constants.GunInfo.cooldown then
            self.timeCounter = constants.GunInfo.cooldown
        end
        if hazel.GetMouseButtonState(hazel.MouseButton.Left) == hazel.InputState.Press and self.timeCounter >= constants.GunInfo.cooldown then
            self.timeCounter = self.timeCounter - constants.GunInfo.cooldown
            ---@type Point
            local position = self:GetParent():GetComponent(ComponentType.Transform).position
            ---@type Point
            local mousePos = hazel.GetGameMousePos()
            local playerCenterX = position.x + constants.TileSize / 2
            local playerCenterY = position.y + constants.TileSize / 2
            local nMouseDir = vmath.Normalize(hazel.CreatePos(mousePos.x - playerCenterX, mousePos.y - playerCenterY))

            local bullet = _M.CreateBullet(hazel.CreatePos(playerCenterX - constants.TileSize / 2, playerCenterY - constants.TileSize / 2), self.damage, hazel.CreatePos(nMouseDir.x * velocity, nMouseDir.y * velocity))
            table.insert(content.BulletList, bullet)
        end
    end
    return _M.CreateComponent(o)
end



---@class BulletComponent:Component
---@field damage number
---@field speed Point

---@return BulletComponent
---@param damage number
---@param speed Point
function _M.CreateBulletComponent(damage, speed)
    local o = { isActive = true, name = ComponentType.Bullet, damage = damage, speed = speed, parent = nil }
    ---@param self BulletComponent
    o.Update = function(self)
        local position = self:GetParent():GetComponent(ComponentType.Transform).position
        local elapseTime = hazel.Time.GetElapseTime()
        position.x = position.x + self.speed.x * elapseTime
        position.y = position.y + self.speed.y * elapseTime
    end
    return _M.CreateComponent(o)
end

---@class ColliBoxComponent:Component
---@field rect Rect

---@return ColliBoxComponent
---@param rect Rect
function _M.CreateColliBoxComponent(rect)
    local o = { isActive = true, name = ComponentType.ColliBox, rect = rect, parent = nil }
    o.Update = function(self) end
    return _M.CreateComponent(o)
end


---@return Entity
---@param pos Point
---@param damage number
---@param speed Point
function _M.CreateBullet(pos, damage, speed)
    ---@type Entity
    local entity = _M.CreateEntity("Bullet")
    entity:SetComponent(_M.CreateTransformComponent(pos, hazel.CreateSize(constants.TileSize, constants.TileSize)))
    entity:SetComponent(_M.CreateBulletComponent(damage, speed))
    entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 1, 4))
    entity:SetComponent(_M.CreateColliBoxComponent(constants.BulletColliBox))
    return entity
end

---@return Entity
---@param pos Point
function _M.CreatePlayer(pos)
    local entity = _M.CreateEntity("player")
    entity:SetComponent(_M.CreateTransformComponent(hazel.CreatePos(pos.x, pos.y), hazel.CreateSize(constants.TileSize, constants.TileSize), Flip.Vertical))
    entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 0, 0))
    entity:SetComponent(_M.CreateRolePropComponent(constants.PlayerInfo.hp, constants.PlayerInfo.velocity))
    entity:SetComponent(_M.CreateDirectionComponent(0))
    entity:SetComponent(_M.CreateColliBoxComponent(constants.RoleColliBox))
    entity:SetComponent(_M.CreateInvincibleComponent(constants.Invincible))
    return entity
end

---@return Entity
---@param pos Point
function _M.CreateMonster(pos)
    local entity = _M.CreateEntity("monster")
    entity:SetComponent(_M.CreateTransformComponent(hazel.CreatePos(pos.x, pos.y), hazel.CreateSize(constants.TileSize, constants.TileSize), Flip.Vertical))
    entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 0, 5))
    entity:SetComponent(_M.CreateRolePropComponent(constants.MonsterInfo.hp, constants.MonsterInfo.velocity, constants.MonsterInfo.damage))
    entity:SetComponent(_M.CreateHpShowComponent(hazel.CreateSize(constants.MonsetHpBarInfo.width, constants.MonsetHpBarInfo.height)))
    entity:SetComponent(_M.CreateDirectionComponent(5))
    entity:SetComponent(_M.CreateColliBoxComponent(constants.RoleColliBox))
    return entity
end


return _M