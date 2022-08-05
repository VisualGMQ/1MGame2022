local hazel = require "hazel"
local constants = require "constants"
local math = require "math"
local vmath = require "vmath"
local content = require "content"
local timer = require "timer"
local animation = require "animation"
local helpfuncs = require "helpfunc"

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
---@param type number
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
    Transform = 1,
    Controller = 2,
    RoleProp = 3,
    HpShow = 4,
    Gun = 5,
    Bullet = 6,
    Direction = 7,
    AI = 8,
    ColliBox = 9,
    Invincible = 10,
    Animator = 11,
    State = 12,
    Supply = 13,
    Image = 14,
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
function _M.CreateControllerComponent()
    local o = { isActive = true, name = ComponentType.Controller, parent = nil }

    ---@param self ControllerComponent
    o.Update = function(self)
        ---@type RolePropComponent
        local roleProp = self:GetParent():GetComponent(ComponentType.RoleProp)
        local speed = roleProp.speed

        ---@type TransformComponent
        local transform = self:GetParent():GetComponent(ComponentType.Transform)
        ---@type AnimatorComponent
        local animator = self:GetParent():GetComponent(ComponentType.Animator)
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

        local canvaSize = hazel.GetCanvaSize()
        if transform.position.x < 0 then transform.position.x = 0 end
        if transform.position.x + transform.size.x > canvaSize.x then transform.position.x = canvaSize.x - transform.size.x end
        if transform.position.y < constants.PlayerHpBarInfo.height then transform.position.y = constants.PlayerHpBarInfo.height end
        if transform.position.y + transform.size.y > canvaSize.y then transform.position.y = canvaSize.y - transform.size.y end

        ---@type Point
        local mousePos = hazel.GetGameMousePos()

        local playerCenterX = transform.position.x
        local playerCenterY = transform.position.y
        playerCenterX = playerCenterX + constants.TileSize / 2
        playerCenterY = playerCenterY + constants.TileSize / 2
        ---@type Point
        local mouseDir = hazel.CreatePos(mousePos.x - playerCenterX, mousePos.y - playerCenterY)
        self:GetParent():GetComponent(ComponentType.Direction):SetDir(mouseDir)

        if hazel.GetMouseButtonState(hazel.MouseButton.Left) == hazel.InputState.Press then
            ---@type GunComponent
            local gun = self:GetParent():GetComponent(ComponentType.Gun)
            if gun then
                gun:Fire(vmath.Normalize(mouseDir))
            end
        end
    end

    return _M.CreateComponent(o)
end

---@class InvincibleComponent:Component
---@field time number

---@return InvincibleComponent
---@param time number
function _M.CreateInvincibleComponent(time)
    local o = { isActive = true, name = ComponentType.Invincible, isInvincible = false, parent = nil }
    o.flashTimer = nil
    ---@type Timer
    o.cdTimer = timer.CreateTimer(time, 1, function ()
        o.isInvincible = false
    end)
    o.cdTimer:Stop()

    ---@return boolean
    ---@param self InvincibleComponent
    o.IsInvincibleState = function(self)
        return self.cdTimer:IsRunning()
    end

    ---@param self InvincibleComponent
    o.IntoInvincible = function(self)
        if self.isInvincible then
            return
        end
        self.isInvincible = true
        self.cdTimer:Rewind()
        self.cdTimer:Start()
        self.flashTimer = timer.CreateTimer(0.1, time / 0.1,
            function()
                ---@type ImageComponent
                local image = o:GetParent():GetComponent(ComponentType.Image)
                if image then
                    if image:IsActive() then
                        image:Disable()
                    else
                        image:Enable()
                    end
                end
            end,
            function()
                ---@type ImageComponent
                local image = o:GetParent():GetComponent(ComponentType.Image)
                if image then
                    image:Enable()
                end
            end)
    end

    ---@param self InvincibleComponent
    o.Update = function(self)
        self.cdTimer:Update()
        if self.flashTimer then
            self.flashTimer:Update()
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
        local state = self:GetParent():GetComponent(ComponentType.State):GetState()
        if state == constants.RoleState.Ice then
            return
        end
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
---@param type number
---@param bulletNum number|nil
function _M.CreateGunComponent(type, bulletNum)
    local o = { isActive = true, name = ComponentType.Gun, type = type, canShoot = true, parent = nil }
    o.bulletNum = constants.BulletInfo[type or constants.BulletType.Normal].initNum
    o.cdTimer = timer.CreateTimer(constants.BulletInfo[type].cooldown, -1, function()
        o.canShoot = true
    end)

    o.SetType = function(self, type)
        self.type = type or constants.BulletType.Normal
        self.bulletNum = constants.BulletInfo[self.type].initNum
    end

    o.GetBulletNum = function(self)
        return self.bulletNum
    end

    ---@param self GunComponent
    ---@param dir Point
    o.Fire = function(self, dir)
        if  not self.canShoot or self.bulletNum == 0  then
            return
        end
        hazel.Sound.Play(constants.SoundName.Shoot)
        ---@type Point
        local position = self:GetParent():GetComponent(ComponentType.Transform).position
        local playerCenterX = position.x + constants.TileSize / 2
        local playerCenterY = position.y + constants.TileSize / 2
        local ndir = vmath.Normalize(dir)
        local velocity = constants.BulletInfo[type].velocity
        local bullet = _M.CreateBullet(hazel.CreatePos(playerCenterX - constants.TileSize / 2, playerCenterY - constants.TileSize / 2),
                                       constants.BulletInfo[type].damage,
                                       hazel.CreatePos(ndir.x * velocity, ndir.y * velocity),
                                       self.type)
        table.insert(content.BulletList, bullet)

        self.canShoot = false
        hazel.Sound.Play(constants.SoundName.Shoot)
        self.bulletNum = self.bulletNum - 1

        if self.bulletNum == 0 then
            self:SetType(constants.BulletType.Normal)
        end 
    end

    ---@param self GunComponent
    o.Update = function(self)
        self.cdTimer:Update()
    end

    return _M.CreateComponent(o)
end


---@class BulletComponent:Component
---@field damage number
---@field speed Point

---@return BulletComponent
---@param damage number
---@param speed Point
---@param type number
function _M.CreateBulletComponent(damage, speed, type)
    local o = { isActive = true, name = ComponentType.Bullet, damage = damage, type = type, speed = speed, parent = nil }
    ---@param self BulletComponent
    o.GetType = function(self)
        return self.type
    end

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


---@class SupplyComponent:Component
---@field rect Rect

---@return SupplyComponent
---@param type number
function _M.CreateSupplyComponent(type)
    local o = { isActive = true, name = ComponentType.Supply, type = type, parent = nil }
    o.Update = function(self) end
    return _M.CreateComponent(o)
end


---@class StateComponent:Component
---@field rect Rect

---@return StateComponent
---@param state number|nil
function _M.CreateStateComponent(state)
    local o = { isActive = true, name = ComponentType.State, state = state or constants.RoleState.Normal, parent = nil }
    ---@type Animation
    o.fireAnimation = animation.CreateAnimation(content.Tilesheet, {
        {row = 10, col = 2, time = 0.2},
        {row = 11, col = 0, time = 0.2},
    }, function()
        o.fireAnimation:Rewind()
        o.fireAnimation:Play()
    end)
    o.GetState = function(self)
        return self.state
    end
    o.IntoIce = function(self)
        self.state = constants.RoleState.Ice
        self.cdTimer = timer.CreateTimer(constants.BulletEffectTime.IceTime, 1, function()
            if self.state == constants.RoleState.Ice then
                self.state = constants.RoleState.Normal
            end
        end)
    end
    o.IntoFire = function(self)
        self.state = constants.RoleState.Fire
        self.fireAnimation:Play()
        self.cdTimer = timer.CreateTimer(constants.BulletEffectTime.FireTime, 1, function()
            self.fireAnimation:Stop()
            if self.state == constants.RoleState.Fire then
                self.state = constants.RoleState.Normal
            end
        end)
    end
    ---@param self StateComponent
    o.Update = function(self)
        self.fireAnimation:Update()
        if self.fireAnimation:IsPlaying() then
            local frame = self.fireAnimation:GetCurFrame()
            ---@type Point
            local position = self:GetParent():GetComponent(ComponentType.Transform).position
            self.fireAnimation:GetTilesheet():Draw(frame.col, frame.row, hazel.CreateRect(position.x, position.y, constants.TileSize, constants.TileSize))
        end
        if self.state == constants.RoleState.Ice then
            local position = self:GetParent():GetComponent(ComponentType.Transform).position
            content.Tilesheet:Draw(2, 9, hazel.CreateRect(position.x, position.y, constants.TileSize, constants.TileSize))
        elseif self.state == constants.RoleState.Fire then
            ---@type RolePropComponent
            local roleProp = self:GetParent():GetComponent(ComponentType.RoleProp)
            local oldHp = roleProp.hp
            roleProp.hp = roleProp.hp - constants.BulletInfo[constants.BulletType.Fire].fireDamage
            if oldHp > 0 and roleProp.hp <= 0 then
                content.Score = content.Score + 1
                helpfuncs.IncKillNum()
            end
        end
        if self.cdTimer then
            self.cdTimer:Update()
        end
    end
    return _M.CreateComponent(o)
end

---@class AnimatorComponent:Component
---@field Play function
---@field Stop function
---@field Pause function
---@field Rewind function
---@field SetAnimation function
---@field GetAnimation function

---@param ani Animation
function _M.CreateAnimatorComponent(ani)
    local o = { isActive = true, name = ComponentType.Animator, ani = ani, parent = nil }

    o.Play = function(self) self.ani:Play() end
    o.Pause = function(self) self.ani:Pause() end
    o.Rewind = function(self) self.ani:Rewind() end
    o.Stop = function(self)
        self.ani:Stop()
        self.ani:Rewind()
    end
    o.SetAnimation = function(self, ani)
        if self.ani then
            self.ani:Stop()
            self.ani:Rewind()
            self.ani = ani
        end
    end
    o.GetAnimation = function(self) return self.ani end

    ---@param self AnimatorComponent
    o.Update = function(self)
        if not self.ani then
            return
        end
        self.ani:Update()
        ---@type ImageComponent
        local image = self:GetParent():GetComponent(ComponentType.Image)
        if image then
            local frame = self.ani:GetCurFrame()
            if frame then
                image.row = frame.row
                image.col = frame.col
                image.tilesheet = self.ani:GetTilesheet()
            end
        end
    end
    return _M.CreateComponent(o)
end


---@return Entity
---@param pos Point
---@param damage number
---@param speed Point
---@param type number|nil
function _M.CreateBullet(pos, damage, speed, type)
    type = type or constants.BulletType.Normal
    ---@type Entity
    local entity = _M.CreateEntity("Bullet")
    entity:SetComponent(_M.CreateTransformComponent(pos, hazel.CreateSize(constants.TileSize, constants.TileSize)))
    entity:SetComponent(_M.CreateBulletComponent(damage, speed, type))
    if type == constants.BulletType.Normal then
        entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 1, 4))
    elseif type == constants.BulletType.Ice then
        entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 0, 10))
    elseif type == constants.BulletType.Fire then
        entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 1, 10))
    end
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
    entity:SetComponent(_M.CreateGunComponent(constants.BulletType.Fire))
    entity:SetComponent(_M.CreateColliBoxComponent(constants.RoleColliBox))
    entity:SetComponent(_M.CreateInvincibleComponent(constants.Invincible))
    entity:SetComponent(_M.CreateStateComponent())
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
    entity:SetComponent(_M.CreateStateComponent())
    return entity
end


---@return Entity
---@param type number
---@param pos Point
function _M.CreateSupply(type, pos)
    local entity = _M.CreateEntity("supply")
    entity:SetComponent(_M.CreateTransformComponent(pos, hazel.CreateSize(constants.TileSize, constants.TileSize)))
    entity:SetComponent(_M.CreateImageComponent(content.Tilesheet, 1, 11))
    entity:SetComponent(_M.CreateColliBoxComponent(constants.SupplyColliBox))
    entity:SetComponent(_M.CreateSupplyComponent(type))
    return entity
end

return _M