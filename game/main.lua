local hazel = require "hazel"
local ECS = require "ecs"
local constants = require "constants"
local content = require "content"
local vmath = require "vmath"
local timer = require "timer"
local animation = require "animation"

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

local function showRestartHint()
    ---@type Point
    local canvaSize = hazel.GetCanvaSize()
    local drawW = content.RestartHintTexture.w * 2
    local drawH = content.RestartHintTexture.h * 2
    local dstRect = hazel.CreateRect((canvaSize.x - drawW) / 2,
                                     (canvaSize.y - drawH) / 2,
                                     drawW, drawH)
    hazel.Renderer.DrawTexture(content.RestartHintTexture, nil, dstRect, hazel.Flip.None)
end

local function showStartHint()
    ---@type Point
    local canvaSize = hazel.GetCanvaSize()
    local drawW = content.StartHintTexture.w * 2
    local drawH = content.StartHintTexture.h * 2
    local dstRect = hazel.CreateRect((canvaSize.x - drawW) / 2,
                                     (canvaSize.y - drawH) / 2,
                                     drawW, drawH)
    hazel.Renderer.DrawTexture(content.StartHintTexture, nil, dstRect, hazel.Flip.None)
end

local function initGame()
    content.BulletList = {}
    content.MonsterList = {}
    content.PlayerEntity = ECS.CreatePlayer(hazel.CreatePos(constants.TileSize * 16, constants.TileSize * 13))
    content.MonsterBirthNum = constants.MonsterBirthInitNum
    content.GameState = content.GameStateEnum.WaitStart
    ---@param v Animation
    for _, v in pairs(content.Animations) do
        v:Rewind()
        v:Stop()
    end
end

local function generateMonster()
    if content.MonsterBirthCountDown > 0 then
        content.MonsterBirthCountDown = content.MonsterBirthCountDown - hazel.Time.GetElapseTime()
        return
    end
    local dir = math.random(1, 4)
    ---@type Point
    local pos = hazel.CreatePos(0, 0)
    ---@type Point
    local canvaSize = hazel.GetCanvaSize()
    if dir == 1 or dir == 3 then
        pos.y = math.random(-constants.TileSize, canvaSize.y + constants.TileSize)
        if dir == 1 then
            pos.x = -constants.TileSize
        else
            pos.x = canvaSize.x + constants.TileSize
        end
    else
        pos.x = math.random(-constants.TileSize, canvaSize.x + constants.TileSize)
        if dir == 2 then
            pos.y = -constants.TileSize
        else
            pos.y = canvaSize.y + constants.TileSize
        end
    end
    ---@type Entity
    local monster = ECS.CreateMonster(pos)
    monster:SetComponent(ECS.CreateAIComponent())
    table.insert(content.MonsterList, monster)
    content.MonsterBirthCountDown = constants.MonsterBirthInterval
end

---@type Timer
local showLicenseTimer = nil

---@return Animation
---@param row number
---@param time number
local function createAnimation(row, time)
    local frame = {}
    for i = 0, 2 do
        table.insert(frame, {row = row, col = i, time = time})
    end
    return animation.CreateAnimation(content.Tilesheet, frame, function(self)
        self:Rewind()
    end)
end

function GameStart()
    hazel.SetWindowIcon("resources/icon.png")
    content.Texture = hazel.LoadTexture("resources/tilesheet.png")
    content.RestartHintTexture = hazel.LoadTexture("resources/RestartHint.png")
    content.LicensTexture = hazel.LoadTexture("resources/License.png")
    content.StartHintTexture = hazel.LoadTexture("resources/StartHint.png")
    content.Tilesheet = hazel.CreateTileSheet(content.Texture, 3, 10)

    initGame()

    content.GameState = content.GameStateEnum.ShowLogo

    content.Animations.PlayerWalkDown = createAnimation(0, 0.1)
    content.Animations.PlayerWalkUp = createAnimation(1, 0.1)
    content.Animations.PlayerWalkRight = createAnimation(2, 0.1)
    content.Animations.PlayerWalkLeft = createAnimation(3, 0.1)

    content.Animations.EnemyWalkDown = createAnimation(5, 0.1)
    content.Animations.EnemyWalkUp = createAnimation(6, 0.1)
    content.Animations.EnemyWalkRight = createAnimation(7, 0.1)
    content.Animations.EnemyWalkLeft = createAnimation(8, 0.1)

    showLicenseTimer = timer.CreateTimer(constants.ShowLicenseTime, 1, function()
        content.GameState = content.GameStateEnum.WaitStart
    end)

    hazel.HideCursor()
    generateFloors()
end

function GameLoop()
    hazel.Time.RecordElapseTime()

    if showLicenseTimer then
        showLicenseTimer:Update()
    end

    hazel.Renderer.SetClearColor(0, 0, 0, 1)
    hazel.Renderer.Clear()

    if content.GameState == content.GameStateEnum.ShowLogo then
        local dstrect = hazel.CreateRect(0, 0, 0, 0)
        dstrect.w = content.LicensTexture.w * 5
        dstrect.h = content.LicensTexture.h * 5
        dstrect.x = (hazel.GetCanvaSize().x - dstrect.w) / 2
        dstrect.y = (hazel.GetCanvaSize().y - dstrect.h) / 2
        hazel.Renderer.DrawTexture(content.LicensTexture, nil, dstrect)
    end

    ---@type RolePropComponent
    local playerRoleInfo = content.PlayerEntity:GetComponent(ECS.ComponentType.RoleProp)
    if not playerRoleInfo or playerRoleInfo.hp <= 0 then
        showRestartHint()
        if hazel.GetKeyState(hazel.Key.R) == hazel.InputState.Press then
            initGame() 
        end
    end

    if content.GameState == content.GameStateEnum.WaitStart or content.GameState == content.GameStateEnum.Gaming then
        drawFloors()
        updateMonster()
        content.PlayerEntity:Update()
        updateBullet()
        collisionDeal()
        drawCurosr()
    end

    if content.GameState == content.GameStateEnum.WaitStart then
        showStartHint()
        if hazel.GetMouseButtonState(hazel.MouseButton.Left) == hazel.InputState.Press then
            content.GameState = content.GameStateEnum.Gaming
            content.PlayerEntity:SetComponent(ECS.CreateControllerComponent())
            content.PlayerEntity:SetComponent(ECS.CreateHpShowComponent(hazel.CreateSize(constants.PlayerHpBarInfo.width, constants.PlayerHpBarInfo.height)))

            for _, v in pairs(content.MonsterList) do
                v:SetComponent(ECS.CreateAIComponent())
            end
        end
    end

    if content.GameState == content.GameStateEnum.Gaming then
        for i = 0, content.MonsterBirthNum do
            generateMonster()
        end
    end
end

function GameQuit()
    hazel.ShowCursor()
    hazel.DestroyTexture(content.Texture)
    hazel.DestroyTexture(content.RestartHintTexture)
    hazel.DestroyTexture(content.StartHintTexture)
    hazel.DestroyTexture(content.LicensTexture)
end
