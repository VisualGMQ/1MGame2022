local _M = {}

_M.ShowLicenseTime = 2.5

_M.SoundName = {
    Shoot = "SHOOT",
    PlayerHurt = "PLAYER_HURT",
    MonsterHurt = "MONSTER_HURT",
    GameOver = "GAMEOVER",
}

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
_M.SupplyColliBox = {
    w = 32,
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

_M.BulletType = {
    Normal = 1,
    Ice = 2,
    Fire = 3,
}

_M.BulletInfo = {
    [_M.BulletType.Normal] = {
        damage = 10,
        velocity = 500,
        cooldown = 0.1,
        initNum = -1,
    },
    [_M.BulletType.Ice] = {
        damage = 5,
        velocity = 400,
        cooldown = 0.1,
        initNum = 100,
    },
    [_M.BulletType.Fire] = {
        damage = 7,
        velocity = 600,
        cooldown = 0.1,
        fireDamage = 1,
        initNum = 100,
    },
}

_M.BulletEffectTime = {
    IceTime = 2,
    FireTime = 2,
}
_M.RoleState = {
    Normal = 1,
    Ice = 2,
    Fire = 3,
}
_M.SupplyFalldownKillNum = 120

_M.SupplyType = {
    IceGun = 1,
    FireGun = 2,
    HpRecover = 3,
}
_M.SupplyItem = {
    [_M.SupplyType.IceGun] = { num = 100 },
    [_M.SupplyType.FireGun] = { num = 100 },
    [_M.SupplyType.HpRecover] = { recover = 50},
}

return _M
