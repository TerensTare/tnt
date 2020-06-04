local angle = 360
local scale = player.scale
local params = vector:new(player.w, player.h)
local pos = player.pos

do_imgui = function()
    if imgui:Begin(window, "Properties", 300, 300) then
        if imgui:begin_section(window, "Transform") then

            if imgui:hslider_float(window, "Rotation", 0, 720, angle) then
                player.angle = angle
            end
            if imgui:hslider_vec(window, "Scale", 0.5, 50, 0.5, 50, scale) then
                player.scale = scale
            end
            if imgui:hslider_vec(window, "Position",
                    params.x/2, screen_w - (params.x/2),
                    params.y/2, screen_h - (params.y/2), pos) then
                player.pos = pos
            end

            imgui:end_section()
        end
        imgui:End()
    end
end