local utils = require "utils"
local class = require "class"
local actions = require "actions"

local test = class:derive("test", actions.action)

function test:new(num)
    local ret = {}
    setmetatable(ret, self)
    ret.number = num

    return ret
end

function test:update(dt)
    utils.unused(dt)
    for i = 0, self.number, 1 do
        print(i)
    end
end

local li = actions.list:new(1)
local t1 = test:new(7)
local t2 = test:new(8)

assert(t1, "the first action is nil")
assert(t2, "the second action is nil")

li:insert(t1)
li:insert(t2)
li:update(10)
