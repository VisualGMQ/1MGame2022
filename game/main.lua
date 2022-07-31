local hazel = require "hazel"
local ECS = require "ecs"
local constants = require "constants"
local math = require "math"
local content = require "content"
local vmath = require "vmath"

local function drawCurosr()
    hazel.Renderer.SetDrawColor(1, 0, 0, 1)
    local pos = hazel.GetGameMousePos()
    local halfW = 10
    local halfH = 10
    hazel.Renderer.DrawRect(pos.x - halfW, pos.y - halfH, halfW * 2, halfH * 2)
    hazel.Renderer.DrawLine(pos.x - halfW + 5, pos.y, pos.x + halfW - 5, pos.y)
    hazel.Renderer.DrawLine(pos.x, pos.y - halfH + 5, pos.x, pos.y + halfH - 5)
end

local FloorMap = {}

local function generateFloors()
    local xNum = math.ceil(hazel.GetCanvaSize().x / constants.TileSize)
    local yNum = math.ceil(hazel.GetCanvaSize().y / constants.TileSize)
    for x = 0, xNum do
        for y = 0, yNum do
            FloorMap[y + x * yNum] = math.random() <= constants.StonePutProbability
        end
    end
end

local function drawFloors()
    local xNum = math.ceil(hazel.GetCanvaSize().x / constants.TileSize)
    local yNum = math.ceil(hazel.GetCanvaSize().y / constants.TileSize)
    local tileIndex = {row = 9, col = 0}
    for x = 0, xNum do
        for y = 0, yNum do
            if FloorMap[y + x * yNum] then
                tileIndex.col = 1
            else
                tileIndex.col = 0
            end
            content.Tilesheet:Draw(tileIndex.col, tileIndex.row,
                                   hazel.CreateRect(x * constants.TileSize, y * constants.TileSize,
                                                    constants.TileSize, constants.TileSize),
                                   hazel.Flip.None)
        end
    end
end

local function updateBullet()
    for k, _ in pairs(content.BulletList) do
        ---@type Entity
        local bullet = content.BulletList[k]
        bullet:Update()
        ---@type Point
        local position = bullet:GetComponent(ECS.ComponentType.Transform).position
        if position.x + constants.TileSize <= 0 or
           position.x >= hazel.GetCanvaSize().x or
           position.y + constants.TileSize <= 0 or
           position.y >= hazel.GetCanvaSize().y then
            content.BulletList[k] = nil
        end
    end
end

local function updateMonster()
    ---@param v Entity
    for _, v in pairs(content.MonsterList) do
        v:Update()
    end
end

local function collisionDeal()
    ---@type Rect
    local playerBox = content.PlayerEntity:GetComponent(ECS.ComponentType.ColliBox).rect
    ---@type Point
    local playerPos = content.PlayerEntity:GetComponent(ECS.ComponentType.Transform).position
    ---@type Rect
    local playerColliBox = hazel.CreateRect(playerPos.x + playerBox.x, playerPos.y + playerBox.y, playerBox.w, playerBox.h)
    ---@type m Entity
    for km, _ in pairs(content.MonsterList) do
        ---@type Entity
        local monster = content.MonsterList[km]
        ---@type Point
        local monsterPos = monster:GetComponent(ECS.ComponentType.Transform).position
        ---@type ColliBoxComponent
        local monsterBoxComponent = monster:GetComponent(ECS.ComponentType.ColliBox)
        if monsterBoxComponent then
            ---@type Rect
            local monsterBox = monsterBoxComponent.rect
            ---@type Rect
            local monsterColliBox = hazel.CreateRect(monsterPos.x + monsterBox.x, monsterPos.y + monsterBox.y,
                                                     monsterBox.w, monsterBox.h)
            ---@type RolePropComponent
            local monsterRoleProp = monster:GetComponent(ECS.ComponentType.RoleProp)
            if vmath.IsRectIntersect(playerColliBox, monsterColliBox) then
                ---@type InvincibleComponent
                local playerInvincible = content.PlayerEntity:GetComponent(ECS.ComponentType.Invincible)
                if not playerInvincible:IsInvincibleState() then
                    ---@type RolePropComponent
                    local playerRoleProp = content.PlayerEntity:GetComponent(ECS.ComponentType.RoleProp)
                    playerRoleProp.hp = playerRoleProp.hp - monsterRoleProp.damage
                    if playerRoleProp:IsDie() then
                        content.PlayerEntity:RemoveComponent(ECS.ComponentType.Controller)
                        content.PlayerEntity:RemoveComponent(ECS.ComponentType.Gun)
                        content.PlayerEntity:RemoveComponent(ECS.ComponentType.Direction)
                        content.PlayerEntity:RemoveComponent(ECS.ComponentType.HpShow)
                        ---@type ImageComponent
                        local image = content.PlayerEntity:GetComponent(ECS.ComponentType.Image)
                        image.row = 4
                        image.col = 0
                    else
                        playerInvincible:IntoInvincible()
                    end
                end
            end

            for kb, _ in pairs(content.BulletList) do
                ---@type Entity
                local bullet = content.BulletList[kb]
                ---@type Point
                local bulletPos = bullet:GetComponent(ECS.ComponentType.Transform).position
                ---@type Rect
                local bulletBox = bullet:GetComponent(ECS.ComponentType.ColliBox).rect
                ---@type Rect
                local bulletColliBox = hazel.CreateRect(bulletPos.x + bulletBox.x, bulletPos.y + bulletBox.y, bulletBox.w, bulletBox.h)
                if vmath.IsRectIntersect(bulletColliBox, monsterColliBox) then
                    content.BulletList[kb] = nil
                    local damage = bullet:GetComponent(ECS.ComponentType.Bullet).damage
                    monsterRoleProp.hp = monsterRoleProp.hp - damage
                    if monsterRoleProp:IsDie() then
                        monster:RemoveComponent(ECS.ComponentType.Direction)
                        monster:RemoveComponent(ECS.ComponentType.AI)
                        monster:RemoveComponent(ECS.ComponentType.HpShow)
                        monster:RemoveComponent(ECS.ComponentType.ColliBox)
                        ---@type ImageComponent
                        local image = monster:GetComponent(ECS.ComponentType.Image)
                        image.row = 4
                        image.col = 2
                    end
                end
            end
        end
    end
end

function GameStart()
    hazel.SetWindowIcon("resources/icon.png")
    content.Texture = hazel.LoadTexture("resources/tilesheet.png")
    content.Tilesheet = hazel.CreateTileSheet(content.Texture, 3, 10)
    content.PlayerEntity = ECS.CreatePlayer(hazel.CreatePos(constants.TileSize * 10, constants.TileSize * 10))
    table.insert(content.MonsterList, ECS.CreateMonster(hazel.CreatePos(500, 500)))

    hazel.HideCursor()
    generateFloors()
end

function GameLoop()
    hazel.Time.RecordElapseTime()

    hazel.Renderer.SetClearColor(0, 0, 0, 1)
    hazel.Renderer.Clear()

    drawFloors()
    updateMonster()
    content.PlayerEntity:Update()
    updateBullet()
    collisionDeal()
    drawCurosr()
end

function GameQuit()
    hazel.ShowCursor()
    hazel.DestroyTexture(content.Texture)
end
