local hazel = require "hazel"

---@class Animation
local _M = {}

---@class Frame
---@field row number
---@field col number
---@field time number

---@param tilesheet TileSheet
---@param frames table<Frame>
---@param onEndCallback function
function _M.CreateAnimation(tilesheet, frames, onEndCallback)
    local o = {tilesheet = tilesheet, frames = frames, index = 1, counter = 0, isPlaying = false, onEndCallback = onEndCallback}
    setmetatable(o, {__index = _M})
    return o
end

---@param self Animation
function _M.Play(self)
    self.isPlaying = true
end

---@param self Animation
function _M.Stop(self)
    self.isPlaying = false
    self.index = 1
    self.counter = 0
end

---@param self Animation
function _M.Pause(self)
    self.isPlaying = false
end

function _M.GetCurFrame(self)
    if self.index > #self.frames then
        return self.frames[#self.frames]
    else
        return self.frames[self.index]
    end
end

---@return TileSheet
---@param self Animation
function _M.GetTilesheet(self)
    return self.tilesheet
end

---@param self Animation
function _M.Rewind(self)
    self.index = 1
    self.counter = 0
end

---@param self Animation
function _M.IsPlaying(self)
    return self.isPlaying
end

---@param self Animation
function _M.Update(self)
    if not self.frames or not self:IsPlaying() then
        return
    end
    if self.index > #self.frames then
        self:Pause()
        if self.onEndCallback then
            self.onEndCallback(self)
        end
        return
    end

    self.counter = self.counter + hazel.Time.GetElapseTime()
    ---@type Frame
    local curFrame = self.frames[self.index]
    if self.counter >= curFrame.time then
        self.counter = self.counter - curFrame.time
        self.index = self.index + 1
    end
end

return _M