local scale = vector:new(10, 10)
local pos = vector:new(100, 160)
local params
local p1

function tnt.init(win)
    p1 = player:new(win)
    p1.angle = 1
    p1.scale = scale
    p1.pos = pos

    params = vector:new(p1.w, p1.h)

    imgui.init(win)
end

function tnt.update(dt)
    p1:update(dt)
    p1:rotate(dt/100)
end

function tnt.draw(win)
    p1:draw(win)
end

function tnt.do_imgui(win)
    if imgui.Begin(win, "Properties", 300, 300) then
        if imgui.begin_section(win, "Transform") then
            if imgui.hslider_vec(win, "Scale", 0.5, 50, 0.5, 50, scale) then
                p1.scale = scale
            end
            if imgui.hslider_vec(win, "Position",
                    params.x/2, screen_w - (params.x/2),
                    params.y/2, screen_h - (params.y/2), pos) then
                p1.pos = pos
            end

            imgui.end_section()
        end
        imgui.End()
    end
end