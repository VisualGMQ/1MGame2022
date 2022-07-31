---@class Content
local _M = {}

---@type Texture
_M.Texture = nil

---@type Texture
_M.RestartHintTexture = nil

---@type Texture
_M.StartHintTexture = nil

---@type boolean
_M.IsStartGame = false

---@type TileSheet
_M.Tilesheet = nil

---@type Entity
_M.PlayerEntity = nil

---@type table<Entity>
_M.BulletList = {}

---@type table<Entity>
_M.MonsterList = {}

_M.MonsterBirthNum = 0

_M.MonsterBirthCountDown = 0

return _M