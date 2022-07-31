local _M = {}

_M.TileSize = 32
_M.PlayerInfo = {
    velocity = 250,
    hp = 100,
}
_M.MonsterInfo = {
    velocity = 100,
    hp = 50,
    damage = 10,
}
_M.RoleColliBox = {
    w = 14,
    h = 32,
    x = 9,
    y = 0,
}
_M.MonsterBirthInterval = 1
_M.MonsterBirthInitNum = 1
_M.Invincible = 1
_M.BulletColliBox = {
    x = 10,
    y = 0,
    w = 12,
    h = 12,
}
_M.StonePutProbability = 0.1
_M.PlayerHpBarInfo = {
    width = _M.TileSize,
    height = 5,
}
_M.MonsetHpBarInfo= {
    width = _M.TileSize,
    height = 5,
}
_M.GunInfo = {
    cooldown = 0.1
}
_M.BulletInfo = {
    damage = 10,
    velocity = 500,
}
_M.GunInfo = {
    cooldown = 0.1
}
_M.BulletInfo = {
    damage = 10,
    velocity = 500,
}

return _M
