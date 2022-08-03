---@class libhazel
local libhazel = require "libhazel"

---@class hazel
local _M = {}

---@class Point
---@field x number
---@field y number

---@class Rect
---@field x number
---@field y number
---@field w number
---@field h number


---@return Point
---@param x number
---@param y number
function _M.CreatePos(x, y)
    return {x = x, y = y}
end

---@return Point
---@param w number
---@param h number
function _M.CreateSize(w, h)
    return {x = w, y = h}
end


---@return Rect
---@param x number
---@param y number
---@param w number
---@param h number
function _M.CreateRect(x, y, w, h)
    return {x = x, y = y, w = w, h = h}
end

---@class InputState
_M.InputState = {
    Release = 0,
    Press = 1,
    Repeat = 2,
}

---@class Key
_M.Key = {
    Unkown = -1,
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    K_0 = 48,
    K_1 = 49,
    K_2 = 50,
    K_3 = 51,
    K_4 = 52,
    K_5 = 53,
    K_6 = 54,
    K_7 = 55,
    K_8 = 56,
    K_9 = 57,
    Semicolon = 59,
    Equal = 61,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    Left_bracket = 91,
    Backslash = 92,
    Right_bracket = 93,
    Grave_accent = 96,
    World_1 = 161,
    World_2 = 162,
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    Page_up = 266,
    Page_down = 267,
    Home = 268,
    End = 269,
    Caps_lock = 280,
    Scroll_lock = 281,
    Num_lock = 282,
    Print_screen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    KP_0 = 320,
    KP_1 = 321,
    KP_2 = 322,
    KP_3 = 323,
    KP_4 = 324,
    KP_5 = 325,
    KP_6 = 326,
    KP_7 = 327,
    KP_8 = 328,
    KP_9 = 329,
    KP_decimal = 330,
    KP_divide = 331,
    KP_multiply = 332,
    KP_subtract = 333,
    KP_add = 334,
    KP_enter = 335,
    KP_equal = 336,
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,
}

---@class MouseButton
_M.MouseButton = {
    Left = 0,
    Right = 1,
    Middle = 2,
}

---@return InputState
---@param key Key
function _M.GetKeyState(key)
    return libhazel.GetKeyState(key)
end

---@return boolean
---@param key number
function _M.IsKeyPressing(key)
    return _M.GetKeyState(key) == _M.InputState.Press
end

---@return boolean
---@param key number
function _M.IsKeyReleasing(key)
    return _M.GetKeyState(key) == _M.InputState.Release
end

---@return Point
function _M.GetMousePos()
    local x, y = libhazel.GetMousePos()
    return _M.CreatePos(x, y)
end

---@return Point
function _M.GetGameMousePos()
    local x, y = libhazel.GetMousePos()
    local windowSize = _M.GetWindowSize()
    local canvaSize = _M.GetCanvaSize()
    return _M.CreatePos(x * canvaSize.x / windowSize.x, y * canvaSize.y / windowSize.y)
end

function _M.HideCursor()
    libhazel.HideCursor()
end

function _M.ShowCursor()
    libhazel.ShowCursor()
end

---@return InputState
---@param btn MouseButton
function _M.GetMouseButtonState(btn)
    return libhazel.GetMouseButtonState(btn)
end

---@return number
function _M.GetTime()
    return libhazel.GetTime()
end

---@param filename string
function _M.SetWindowIcon(filename)
    libhazel.WindowSetIcon(filename)
end

---@return Point
function _M.GetCanvaSize()
    local x, y = libhazel.GetCanvaSize()
    return _M.CreatePos(x, y)
end

---@return Point
function _M.GetWindowSize()
    local x, y = libhazel.GetWindowSize()
    return _M.CreatePos(x, y)
end

---@class Texture
---@field w number
---@field h number

---@return Texture
---@param filename string
function _M.LoadTexture(filename)
    local texture = libhazel.LoadTexture(filename)
    local w, h = libhazel.TextureGetSize(texture)
    return {rawPointer = texture, w = w, h = h}
end

---@param texture Texture
function _M.DestroyTexture(texture)
    libhazel.DestroyTexture(texture.rawPointer)
end

---@return Point
---@param texture Texture
function _M.GetTextureSize(texture)
    local x, y = libhazel.TextureGetSize(texture)
    return _M.CreatePos(x, y)
end

function _M.SayHello()
    libhazel.SayHello()
end

---@class Flip
Flip = {
    None = 0,
    Vertical = 1,
    Horizontal = 2,
    Both = 3,
}

_M.Flip = Flip

---@class Renderer
local Renderer = {
    ---@param r number
    ---@param g number
    ---@param b number
    ---@param a number
    SetClearColor = function(r, g, b, a)
        libhazel.RenderSetClearColor(r, g, b, a or 1)
    end,

    ---@param r number
    ---@param g number
    ---@param b number
    ---@param a number
    SetDrawColor = function(r, g, b, a)
        libhazel.RenderSetDrawColor(r, g, b, a or 1)
    end,

    ---@param x1 number
    ---@param y1 number
    ---@param x2 number
    ---@param y2 number
    DrawLine = function(x1, y1, x2, y2)
        libhazel.RenderDrawLine(x1, y1, x2, y2)
    end,

    ---@param p1 Point
    ---@param p2 Point
    DrawLineByPoints = function(p1, p2)
        libhazel.RenderDrawLine(p1.x, p1.y, p2.x, p2.y)
    end,

    ---@param x number
    ---@param y number
    ---@param w number
    ---@param h number
    DrawRect = function(x, y, w, h)
        libhazel.RenderDrawRect(x, y, w, h)
    end,

    ---@param rect Rect
    DrawRectByRect = function(rect)
        libhazel.RenderDrawRect(rect.x, rect.y, rect.w, rect.h)
    end,

    ---@param x number
    ---@param y number
    ---@param w number
    ---@param h number
    FillRect = function(x, y, w, h)
        libhazel.RenderFillRect(x, y, w, h)
    end,

    ---@param rect Rect
    FillRectByRect = function(rect)
        libhazel.RenderFillRect(rect.x, rect.y, rect.w, rect.h)
    end,

    ---@param texture Texture
    ---@param srcrect Rect|nil
    ---@param dstrect Rect|nil
    ---@param flip number|nil
    DrawTexture = function(texture, srcrect, dstrect, flip)
        srcrect = srcrect or _M.CreateRect(0, 0, texture.w, texture.h)
        local size = _M.GetCanvaSize();
        dstrect = dstrect or _M.CreateRect(0, 0, size.x, size.y)
        libhazel.RenderDrawTexture(texture.rawPointer,
                                   srcrect.x, srcrect.y, srcrect.w, srcrect.h,
                                   dstrect.x, dstrect.y, dstrect.w, dstrect.h,
                                   flip or Flip.None)
    end,

    Clear = function()
        libhazel.RenderClear()
    end
}

_M.Renderer = Renderer

---@class Log
local Log = {}

function Log.Loge(...)
    print("[Lua Error]:", ...)
end

function Log.Logw(...)
    print("[Lua Warn]:", ...)
end

function Log.Logi(...)
    print("[Lua Info]:", ...)
end

_M.Log = Log


---@class TextureStorage
local TextureStorage = {
    storage = {}
}

---@param filename string
---@param name string
---@return Texture
function TextureStorage.LoadTexture(filename, name)
    local oldTexture = TextureStorage.storage[name]
    if oldTexture then
        _M.Log.Logw(string.format("%s already exists, delete it", name))
        _M.DestroyTexture(oldTexture)
    end
    local texture = _M.LoadTexture(filename)
    TextureStorage.storage[name] = texture
    return texture
end

function TextureStorage.ClearAll()
    for _, v in pairs(TextureStorage.storage) do
        if v then
            _M.DestroyTexture(v)
        end
    end
end

---@return Texture
---@param name string
function TextureStorage.Get(name)
    return TextureStorage.storage[name]
end

_M.TextureStorage = TextureStorage

---@class TileSheet
---@field column number
---@field row number
---@field texture Texture
---@field tileWidth number
---@field tileHeight number
local TileSheet = {}

---@return TileSheet
---@param texture Texture
---@param column number
---@param row number
function _M.CreateTileSheet(texture, column, row)
    local o = {texture = texture, column = column, row = row, tileWidth = texture.w / column, tileHeight = texture.h / row}
    setmetatable(o, {__index = TileSheet})
    return o
end

---@param self TileSheet
---@param column number
---@param row number
---@param dstrect Rect
---@param flip number
function TileSheet.Draw(self, column, row, dstrect, flip)
    local srcrect = _M.CreateRect(column * self.tileWidth, row * self.tileHeight, self.tileWidth, self.tileHeight)
    _M.Renderer.DrawTexture(self.texture, srcrect, dstrect, flip)
end

_M.TileSheet = TileSheet


local Time = { curTime = 0, elapseTime = 0 }

function Time.RecordElapseTime()
    local oldTime = Time.curTime
    Time.curTime = libhazel.GetTime()
    Time.elapseTime = Time.curTime - oldTime
end

---@return number  the passed seconds between two frames
function Time.GetElapseTime()
    return Time.elapseTime
end

_M.Time = Time


local Sound = {}

---@param filename string
---@param name string
function Sound.Load(filename, name)
    libhazel.LoadSound(filename, name);
end

---@param name string
function Sound.Play(name)
    libhazel.PlaySound( name);
end

_M.Sound = Sound


return _M
