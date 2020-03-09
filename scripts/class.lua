local utils = require "utils"

-- TODO:
-- multiple inheritance and interfaces
-- enums, unions, arrays, tuples
-- test if derive() works correctly

local class = {}
class.__index = class

function class:new()
    if utils.Debug == true then
        print("called class:new()")
    end
end

function class:type()
    return self.ty
end

function class:create(name)
    local cls = {}
    cls.name = name
    cls.base_type = function()
        return name
    end
    cls["__call"] = class.__call
    cls.__index = cls
    cls.super = self
    setmetatable(cls, self)
    return cls
end

function class:derive(name, base)
    if utils.to_bool(base) then
        local new_class = {}
        local class_mt = {__index = new_class}

        function new_class:create()
            local ret = {}
            setmetatable(ret, class_mt)
            return ret
        end

        setmetatable(new_class, {__index = base})

        function new_class:base_type()
            return base:base_type()
        end

        return new_class
    else
        self.name = name

        setmetatable(
            self,
            {
                base_type = function()
                    return name
                end
            }
        )

        return self:create(name)
    end
end

function class:__call(...)
    local inst = setmetatable({}, self)
    inst:new(...)
    return inst
end

return class
