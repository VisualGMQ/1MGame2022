local hazel = require "hazel"

---@class Timer
local _M = {}

---@param interval number   seconds
---@param loop number       -1 means infinite loop
---@param callback function|nil     a no param, no return value callback function
---@param onStopFunc function|nil   a no param, no return value callback function called when timer stop
function _M.CreateTimer(interval, loop, callback, onStopFunc)
    local o = {isStart = true, interval = interval, totleLoop = loop, loop = loop, callback = callback, onStopFunc = onStopFunc, counter = 0}
    setmetatable(o, {__index = _M})
    return o
end

---@param self Timer
function _M.Rewind(self)
    self.loop = self.totleLoop
end

---@param self Timer
function _M.Start(self)
    self.isStart = true
end

---@return boolean
---@param self Timer
function _M.IsRunning(self)
    return self.isStart
end

---@param self Timer
function _M.Stop(self)
    self.isStart = false
end

---@param self Timer
function _M.Update(self)
    if self.loop == 0 then
        if self:IsRunning()  then
            if self.onStopFunc then
                self.onStopFunc()
            end
            self:Stop()
        end
    end
    if self:IsRunning() then
        self.counter = self.counter + hazel.Time.GetElapseTime()
        while self.counter >= self.interval do
            self.counter = self.counter - self.interval
            self.loop = self.loop - 1
            if self.callback then
                self.callback()
            end
        end
    end
end

return _M