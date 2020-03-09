-- TODO:
-- recheck list and resumable
-- add a queue table in list
-- add different types of actions, like resumable, sync, if-else, for, etc...
-- add action do block a lane/some lanes until another action has finished (partial_sync ??)
-- handle blocking and non-blocking actions in list:update()
-- list:insert with vararg
-- rewrite the :new() functions to be similar to list:new()

-- TODO(maybe):
-- onexit ??
-- register this file like luafilesystem does ??
-- do not return action and blocking ??
-- add the ownerlist to each actiontype ??

local class = require "class"
local queue = require "queue"

local action = class:create("action")

function action:new()
    local ret = {}
    setmetatable(ret, self)
    ret.finished = false
    ret.blocks = false
    ret.running = false
    return ret
end

-- function action:running()
--     return not self.finished
-- end

local blocking = class:derive("blocking", action)

function blocking:new()
    setmetatable(blocking, action)
    self.blocks = true
end

local delay = class:derive("delay", blocking)

function delay:new(time)
    setmetatable(delay, blocking)
    self.delay = time
    self.count = 0
end

function delay:update(dt)
    self.count = self.count + dt
    if self.count >= self.delay then
        self.finished = true
    end
end

-- sync stops running when the desired actions have finished.
local sync = class:derive("sync", blocking)

function sync:new()
    setmetatable(sync, action)
end

function sync:update(dt)
    if self.running then
        self.finished = true
    end
end

local resumable = {}

function resumable:new(func, ...)
    setmetatable(resumable, action)
    self.blocks = false
    self.paused = false
    self.finished = false
    self.callback = func
    self.cor = coroutine.create(func, ...)
end

function resumable:pause()
    self.paused = true
    coroutine.yield()
end

function resumable:resume()
    self.paused = false
    coroutine.resume()
end

function resumable:is_paused()
    return self.paused
end

function resumable:update()
    while not self.finished do
        local dt = coroutine.yield()
    end
    self.finished = true
end

local list = class:create("list")

function list:new(num_lanes)
    local ret = {}
    setmetatable(ret, self)

    ret.data = queue:new()
    ret.current = nil
    ret.lanes = {}

    for i = 1, num_lanes + 1, 1 do
        ret.lanes[i] = (1 << i)
    end

    return ret
end

function list:insert(val)
    self.data:push(val)
end

function list:update(dt)
    local function helper()
        if self.data:size() > 0 then
            self.current = self.data:front()
            self.current.running = true
            self.current:update(dt)
            self.data:pop()
            return helper()
        else
            self.current = 0
            return
        end
    end

    helper() -- maybe with return ??
end

return {
    action = action,
    blocking = blocking,
    delay = delay,
    sync = sync,
    resumable = resumable,
    list = list
}
