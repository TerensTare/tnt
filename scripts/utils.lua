-- TODO:
-- 1. add utility to get the running os.

local time = os.time

local utils = {}
utils.Debug = false

function utils.sleep(s)
    local t = time() + tonumber(s)
    repeat until time() > t
end

function utils.file_size(file)
    local current = file:seek()
    local size = file:seek("end")
    file:seek("set", current)
    return size
end

return utils
