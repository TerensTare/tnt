local class = require "class"

local queue = class:create("queue")

function queue:new(...)
    self.s = select("#", ...)
    for _, v in pairs{...} do
        self:push(v)
    end
    return self
end

function queue:front()
    if self.s > 0 then
        return self[1]
    end
    return -1
end

function queue:back()
    if self.s > 0 then
        return self[#self]
    end
    return -1
end

function queue:empty()
    return (self.s <= 0)
end

function queue:size()
    return self.s
end

function queue:push(val)
    if val and (self.s >= 0) then
        self.s = self.s + 1
        self[self.s] = val
    end
end

function queue:pop()
    if self.s > 0 then
        table.remove(self, 1)
        self.s = self.s - 1
    end
end

-- used in dequeue
-- function queue:erase(val)
--     if self.s > 0 then
--         for k, _ in pairs(self) do
--             if self[k] == val then
--                 self[k] = nil
--             end
--         end
--     end
-- end

-- used in dequeue
-- function queue:clear()
--     if self.s > 0 then
--         for _, v in pairs(self) do
--             self:erase(v)
--         end
--     end
-- end

-- used in dequeue
-- function queue:at(idx)
--     if self.s > 0 and idx > 0 then
--         return self[idx]
--     end
--     return -1
-- end

return queue
