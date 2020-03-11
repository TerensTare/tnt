local enum = {}

function enum:new(data)
    return setmetatable(
        {},
        {
            __index = data,
            __newindex = function(data, key, value)
                error("Trying to modify enum values")
            end,
            __metatable = false
        }
    )
end

return enum
