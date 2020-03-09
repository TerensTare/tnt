local class = require "class"
local tuple = class:create("tuple")

function tuple:new(...)
    local co = coroutine.wrap(function (...)
        coroutine.yield()
        while true do
            coroutine.yield(...)
        end
    end)
    co(...)
end