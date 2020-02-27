local utils = require "utils"

local class = {}
class.__index = class

function class:new() if utils.Debug == true then print("called class:new()") end end

function class:derive()
    local cls = {}
    cls["__call"] = class.__call
    cls.__index = cls
    cls.super = self
    setmetatable(cls, self)
    return cls
end

function class:__call(...)
    local inst = setmetatable({}, self)
    inst:new(...)
    return inst
end

return class
