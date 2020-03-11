local utils = require "utils"
local class = require "class"
local actions = require "actions"

local test = class:derive("test", actions.action)

function test:new(num)
    local ret = {}
    setmetatable(ret, self)
    ret.number = num

    ret.update = function(dt)
        utils.unused(dt)
        for i = 1, num, 1 do
            io.write(i, " ")
        end
        print()
    end

    return ret
end

local li = actions.list:new(1)
local t1 = test:new(7)
local t2 = test:new(8)

li:insert(t1)
li:insert(t2)
li:update(10)