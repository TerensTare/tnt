local scale
local pos
local params
local p1

function tnt.init(win)
    p1 = objects:get_data(0)
    scale = p1.scale
    pos = p1.pos

    params = vector:new(sprites.sprite[0].crop.w, sprites.sprite[0].crop.h)

    imgui.init(win)
end

function tnt.update(dt)
    objects.angle[0] = objects.angle[0] + (dt/100)
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