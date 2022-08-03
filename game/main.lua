local hazel = require "hazel"
local ECS = require "ecs"
local constants = require "constants"
local content = require "content"
local vmath = require "vmath"
local timer = require "timer"
local animation = require "animation"
local helpfunc = require "helpfunc"

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

local function updateSupply()
    ---@param v Entity
    for _, v in pairs(content.SupplyList) do
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
                if playerInvincible and not playerInvincible:IsInvincibleState() then
                    ---@type RolePropComponent
                    local playerRoleProp = content.PlayerEntity:GetComponent(ECS.ComponentType.RoleProp)
                    local oldHp = playerRoleProp.hp
                    playerRoleProp.hp = playerRoleProp.hp - monsterRoleProp.damage
                    hazel.Sound.Play(constants.SoundName.PlayerHurt)
                    if playerRoleProp:IsDie() then
                        content.PlayerEntity:RemoveComponent(ECS.ComponentType.Invincible)
                        if oldHp > 0 then
                            hazel.Sound.Play(constants.SoundName.GameOver)
                        end
                        content.PlayerEntity:RemoveComponent(ECS.ComponentType.Controller)
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
                    local oldHp = monsterRoleProp.hp
                    local bulletType = bullet:GetComponent(ECS.ComponentType.Bullet):GetType()
                    if bulletType == constants.BulletType.Ice then
                        monster:GetComponent(ECS.ComponentType.State):IntoIce()
                    elseif bulletType == constants.BulletType.Fire then
                        monster:GetComponent(ECS.ComponentType.State):IntoFire()
                    end
                    monsterRoleProp.hp = monsterRoleProp.hp - damage

                    hazel.Sound.Play(constants.SoundName.MonsterHurt)
                    if monsterRoleProp:IsDie() then
                        if oldHp > 0 then
                            content.Score = content.Score + 1
                            helpfunc.IncKillNum()
                        end
                    end
                end
            end
        end

        ---@param v Entity
        for i, v in pairs(content.SupplyList) do
            ---@type Point
            local supplyPos = v:GetComponent(ECS.ComponentType.Transform).position
            ---@type Rect
            local supplyBox = v:GetComponent(ECS.ComponentType.ColliBox).rect
            ---@type Rect
            local supplyColliBox = hazel.CreateRect(supplyPos.x + supplyBox.x, supplyPos.y + supplyBox.y, supplyBox.w, supplyBox.h)
            if vmath.IsRectIntersect(supplyColliBox, playerColliBox) then
                content.SupplyList[i] = nil
                ---@type GunComponent
                local gun = content.PlayerEntity:GetComponent(ECS.ComponentType.Gun)
                local type = v:GetComponent(ECS.ComponentType.Supply).type
                print(type)
                if type == constants.SupplyType.HpRecover then
                    ---@type RolePropComponent
                    local roleProp = content.PlayerEntity:GetComponent(ECS.ComponentType.RoleProp)
                    roleProp.hp = roleProp.hp + constants.SupplyItem[type].recover
                    if roleProp.hp > constants.PlayerInfo.hp then
                        roleProp.hp = constants.PlayerInfo.hp
                    end
                elseif type == constants.SupplyType.IceGun then
                    gun:SetType(constants.BulletType.Ice)
                    gun.bulletNum = gun.bulletNum + constants.SupplyItem[type].num
                elseif type == constants.SupplyType.FireGun then
                    gun:SetType(constants.BulletType.Fire)
                    gun.bulletNum = gun.bulletNum + constants.SupplyItem[type].num
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
    content.KillNum = 0
    content.Score = 0
    content.BulletList = {}
    content.MonsterList = {}
    content.SupplyList = {}
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

---@param num number
---@param x number
---@param y number
local function drawNum(num, x, y)
    if num < 0 then
        content.NumberTilesheet:Draw(10, 0, hazel.CreateRect(x, y, 32, 32))
        return
    end
    local scoreStr = tostring(num)
    for i = 1, #scoreStr do
        local col = tonumber(string.sub(scoreStr, i, i))
        if col == -1 then
            col = 10
        end
        content.NumberTilesheet:Draw(col, 0, hazel.CreateRect(x + (i - 1) * 32, y, 32, 32))
    end
end

local function updateRoles()
    ---@param monster Entity
    for _, monster in pairs(content.MonsterList) do
        ---@type RolePropComponent
        local roleProp = monster:GetComponent(ECS.ComponentType.RoleProp)
        if roleProp and roleProp:IsDie() then
            monster:RemoveComponent(ECS.ComponentType.Direction)
            monster:RemoveComponent(ECS.ComponentType.AI)
            monster:RemoveComponent(ECS.ComponentType.HpShow)
            monster:RemoveComponent(ECS.ComponentType.ColliBox)
            monster:RemoveComponent(ECS.ComponentType.State)
            ---@type ImageComponent
            local image = monster:GetComponent(ECS.ComponentType.Image)
            image.row = 4
            image.col = 2
        end
    end
end

function GameStart()
    hazel.SetWindowIcon("resources/icon.png")
    content.Texture = hazel.LoadTexture("resources/tilesheet.png")
    content.RestartHintTexture = hazel.LoadTexture("resources/RestartHint.png")
    content.LicensTexture = hazel.LoadTexture("resources/License.png")
    content.StartHintTexture = hazel.LoadTexture("resources/StartHint.png")
    content.Tilesheet = hazel.CreateTileSheet(content.Texture, 3, 12)
    content.NumberTexture = hazel.LoadTexture("resources/numbers.png")
    content.NumberTilesheet = hazel.CreateTileSheet(content.NumberTexture, 11, 1)
    hazel.Sound.Load("resources/gameover.wav", constants.SoundName.GameOver)
    hazel.Sound.Load("resources/player_hurt.wav", constants.SoundName.PlayerHurt)
    hazel.Sound.Load("resources/monster_hurt.wav", constants.SoundName.MonsterHurt)
    hazel.Sound.Load("resources/shoot.wav", constants.SoundName.Shoot)

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

    if content.GameState == content.GameStateEnum.WaitStart or content.GameState == content.GameStateEnum.Gaming then
        drawFloors()
        updateSupply()
        updateMonster()
        content.PlayerEntity:Update()
        updateBullet()
        collisionDeal()
        drawCurosr()
        updateRoles()
    end

    ---@type RolePropComponent
    local playerRoleInfo = content.PlayerEntity:GetComponent(ECS.ComponentType.RoleProp)
    if not playerRoleInfo or playerRoleInfo.hp <= 0 then
        showRestartHint()
        if hazel.GetKeyState(hazel.Key.R) == hazel.InputState.Press then
            initGame() 
        end
    end

    if content.GameState == content.GameStateEnum.Gaming then
        ---@type GunComponent 
        local gun = content.PlayerEntity:GetComponent(ECS.ComponentType.Gun)
        local x = hazel.GetCanvaSize().x - 128
        drawNum(content.Score, x, 32)
        if gun then
            drawNum(gun:GetBulletNum(), x, 80)
        end
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