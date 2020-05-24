-- A simple utility to use Hot Code Reloading in Lua.

--[[
    TODO:
    1. trigger live.reload() in case a file has changed.
        ?? make a new file type with a timestamp.
    2.(partial) live.reload_all() and live.reload_all_if(). Add check for LuaJIT modules there.
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

function live.reload_all()
    for k, _ in pairs(package.loaded) do
        -- we don't wanna mess with the default lua modules.
        if k ~= "debug" and k ~= "package" and
            k ~= "coroutine" and k ~= "bit32" and
            k ~= "utf8" and k ~= "math" and
            k ~= "_G" and k ~= "string" and
            k ~= "io" and k ~= "os" and
            k ~= "table" then
            package.loaded[k] = nil
            package.loaded[k] = require(k)
        end
    end
end

function live.reload_all_if(cond)
    if not not cond then
        for k, _ in pairs(package.loaded) do
            -- we don't wanna mess with the default lua modules.
            if k ~= "debug" and k ~= "package" and
                k ~= "coroutine" and k ~= "bit32" and
                k ~= "utf8" and k ~= "math" and
                k ~= "_G" and k ~= "string" and
                k ~= "io" and k ~= "os" and
                k ~= "table" then
                package.loaded[k] = nil
                package.loaded[k] = require(k)
            end
        end
    end
end

return live