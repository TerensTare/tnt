local class = require "class"
local utils = require "utils"

local sqrt = math.sqrt

-- just for fun, not intended to be public or to be used anywhere.

local Vector = class:derive("Vector")

function Vector:new(x, y)
    self.x = x
    self.y = y
if utils.Debug then
    print("Vector:new(".. x .. ", " .. y .. ") called succesfully!")
end
    return self
end

function Vector:add(other)
    self.x = self.x + other.x
    self.y = self.y + other.y
end

function Vector:sub(other)
    self.x = self.x - other.x
    self.y = self.y - other.y
end

function Vector:unm()
    return {-self.x, -self.y}
end

function Vector:len()
    return sqrt(self.x*self.x+self.y*self.y)
end

return Vector