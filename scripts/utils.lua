-- TODO:
-- 1. add utility to get the running os.

local time = os.time

local utils = {}
utils.Debug = false

function utils.sleep(s)
    local t = time() + tonumber(s)
    repeat
    until time() > t
end

function utils.file_size(file)
    local current = file:seek()
    local size = file:seek("end")
    file:seek("set", current)
    return size
end

function utils.to_bool(x)
    return not (not x)
end

function utils.unused(...)
    return ...
end

function utils.swap(tab)
    local ret = {}
    for k, v in tab do
        ret[v] = k
    end
    return ret
end

function utils.index(tab, val)
    local t = utils.swap(tab)
    return t[val]
end

-- code taken from
-- http://lua-users.org/wiki/LuaHacks
function utils.cout(str)
    if str ~= nil then
        io.write(tostring(str), " ")
    else
        io.write("\n")
    end
    return utils.cout
end

return utils
