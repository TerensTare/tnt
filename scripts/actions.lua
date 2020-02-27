local action = {}

function action:new()
    self.finished = false
    self.blocks = false
end

local blocking = {}

function blocking:new()
    self.blocks = true
    self.finished = false
end

local delay = {}

function delay:new(dt)
    self.blocks = true
    self.delay = dt
    self.current = 0
    self.finished = false
end

function delay:update(dt)
    self.current = self.current + dt
    if self.current >= self.delay then
        self.finished = true
    end
end

local actionlist = {}

return {action, blocking, delay, actionlist}