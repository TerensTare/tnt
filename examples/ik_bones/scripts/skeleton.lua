function update(dt)
    local x, y = input:mouse_pos()
    bones:follow(id, vector:new(x, y))
end