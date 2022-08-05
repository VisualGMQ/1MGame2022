---@class Content
local _M = {}

---@type Texture
_M.Texture = nil

---@type Texture
_M.RestartHintTexture = nil

---@type Texture
_M.StartHintTexture = nil

---@type Texture
_M.LicensTexture = nil

---@type Texture
_M.NumberTexture = nil

---@type TileSheet
_M.Tilesheet = nil

---@type TileSheet
_M.NumberTilesheet = nil

---@type Entity
_M.PlayerEntity = nil

---@type table<Entity>
_M.BulletList = {}

_M.GameStateEnum = {
    ShowLogo = 1,
    WaitStart = 2,
    Gaming = 3,
}

_M.HpRecoverListAnim = {}

_M.GameState = _M.GameStateEnum.ShowLogo

---@type table<Entity>
_M.MonsterList = {}

---@type table<Entity>
_M.MonsterCorpseList = {}

_M.MonsterBirthNum = 0

_M.MonsterBirthCountDown = 0

_M.Animations = {
    ---@type Animation
    PlayerWalkDown = nil,
    ---@type Animation
    PlayerWalkUp = nil,
    ---@type Animation
    PlayerWalkRight = nil,
    ---@type Animation
    PlayerWalkLeft = nil,

    ---@type Animation
    EnemyWalkDown = nil,
    ---@type Animation
    EnemyWalkUp = nil,
    ---@type Animation
    EnemyWalkRight = nil,
    ---@type Animation
    EnemyWalkLeft = nil,
}

---@type number
_M.KillNum = 0
---@type number
_M.Score = 0

---@type table<Entity>
_M.SupplyList = {}

return _M