-- A simple utility to use Hot Code Reloading in Lua.

--[[
    TODO:
    1. trigger live.reload() in case a file has changed.
        ?? make a new file type with a timestamp.
    2. live.reload_if(), live.reload_all().
]]

local live = {}
live.__index = live

function live.reload(mod)
    if package.loaded[mod] then
        package.loaded[mod] = nil
    end
    return require(mod)
end

function live.reload_if(mod, cond)
    if not not cond then
        if package.loaded[mod] then
            package.loaded[mod] = nil
        end
    return require(mod)
    end
end

function live:reload_all()
    -- WIP
end

function live:reload_all_if(cond)
    -- WIP
end

return live