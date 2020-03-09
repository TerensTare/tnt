-- script taken from
-- https://github.com/EmmanuelOga/easing/blob/master/lib/easing.lua

local pow = math.pow
local sin = math.sin
local cos = math.cos
local pi = math.pi
local sqrt = math.sqrt
local abs = math.abs
local asin = math.asin

local function linear(time, begin, change, duration)
    return change * time / duration + begin
end

local function inQuad(time, begin, change, duration)
    time = time / duration
    return change * pow(time, 2) + begin
end

local function outQuad(time, begin, change, duration)
    time = time / duration
    return -change * time * (time - 2) + begin
end

local function inOutQuad(time, begin, change, duration)
    time = time / duration * 2
    if time < 1 then
        return change / 2 * pow(time, 2) + begin
    else
        return -change / 2 * ((time - 1) * (time - 3) - 1) + begin
    end
end

local function outInQuad(time, begin, change, duration)
    if time < duration / 2 then
        return outQuad(time * 2, begin, change / 2, duration)
    else
        return inQuad((time * 2) - duration, begin + change / 2, change / 2,
                      duration)
    end
end

local function inCubic(time, begin, change, duration)
    time = time / duration
    return change * pow(time, 3) + begin
end

local function outCubic(time, begin, change, duration)
    time = time / duration - 1
    return change * (pow(time, 3) + 1) + begin
end

local function inOutCubic(time, begin, change, duration)
    time = time / duration * 2
    if time < 1 then
        return change / 2 * time * time * time + begin
    else
        time = time - 2
        return change / 2 * (time * time * time + 2) + begin
    end
end

local function outInCubic(time, begin, change, duration)
    if time < duration / 2 then
        return outCubic(time * 2, begin, change / 2, duration)
    else
        return inCubic((time * 2) - duration, begin + change / 2, change / 2,
                       duration)
    end
end

local function inQuart(time, begin, change, duration)
    time = time / duration
    return change * pow(time, 4) + begin
end

local function outQuart(time, begin, change, duration)
    time = time / duration - 1
    return -change * (pow(time, 4) - 1) + begin
end

local function inOutQuart(time, begin, change, duration)
    time = time / duration * 2
    if time < 1 then
        return change / 2 * pow(time, 4) + begin
    else
        time = time - 2
        return -change / 2 * (pow(time, 4) - 2) + begin
    end
end

local function outInQuart(time, begin, change, duration)
    if time < duration / 2 then
        return outQuart(time * 2, begin, change / 2, duration)
    else
        return inQuart((time * 2) - duration, begin + change / 2, change / 2,
                       duration)
    end
end

local function inQuint(time, begin, change, duration)
    time = time / duration
    return change * pow(time, 5) + begin
end

local function outQuint(time, begin, change, duration)
    time = time / duration - 1
    return change * (pow(time, 5) + 1) + begin
end

local function inOutQuint(time, begin, change, duration)
    time = time / duration * 2
    if time < 1 then
        return change / 2 * pow(time, 5) + begin
    else
        time = time - 2
        return change / 2 * (pow(time, 5) + 2) + begin
    end
end

local function outInQuint(time, begin, change, duration)
    if time < duration / 2 then
        return outQuint(time * 2, begin, change / 2, duration)
    else
        return inQuint((time * 2) - duration, begin + change / 2, change / 2,
                       duration)
    end
end

local function inSine(time, begin, change, duration)
    return -change * cos(time / duration * (pi / 2)) + change + begin
end

local function outSine(time, begin, change, duration)
    return change * sin(time / duration * (pi / 2)) + begin
end

local function inOutSine(time, begin, change, duration)
    return -change / 2 * (cos(pi * time / duration) - 1) + begin
end

local function outInSine(time, begin, change, duration)
    if time < duration / 2 then
        return outSine(time * 2, begin, change / 2, duration)
    else
        return inSine((time * 2) - duration, begin + change / 2, change / 2,
                      duration)
    end
end

local function inExpo(time, begin, change, duration)
    if time == 0 then
        return begin
    else
        return change * pow(2, 10 * (time / duration - 1)) + begin - change *
                   0.001
    end
end

local function outExpo(time, begin, change, duration)
    if time == duration then
        return begin + change
    else
        return change * 1.001 * (-pow(2, -10 * time / duration) + 1) + begin
    end
end

local function inOutExpo(time, begin, change, duration)
    if time == 0 then return begin end
    if time == duration then return begin + change end
    time = time / duration * 2
    if time < 1 then
        return change / 2 * pow(2, 10 * (time - 1)) + begin - change * 0.0005
    else
        time = time - 1
        return change / 2 * 1.0005 * (-pow(2, -10 * time) + 2) + begin
    end
end

local function outInExpo(time, begin, change, duration)
    if time < duration / 2 then
        return outExpo(time * 2, begin, change / 2, duration)
    else
        return inExpo((time * 2) - duration, begin + change / 2, change / 2,
                      duration)
    end
end

local function inCirc(time, begin, change, duration)
    time = time / duration
    return (-change * (sqrt(1 - pow(time, 2)) - 1) + begin)
end

local function outCirc(time, begin, change, duration)
    time = time / duration - 1
    return (change * sqrt(1 - pow(time, 2)) + begin)
end

local function inOutCirc(time, begin, change, duration)
    time = time / duration * 2
    if time < 1 then
        return -change / 2 * (sqrt(1 - time * time) - 1) + begin
    else
        time = time - 2
        return change / 2 * (sqrt(1 - time * time) + 1) + begin
    end
end

local function outInCirc(time, begin, change, duration)
    if time < duration / 2 then
        return outCirc(time * 2, begin, change / 2, duration)
    else
        return inCirc((time * 2) - duration, begin + change / 2, change / 2,
                      duration)
    end
end

local function inElastic(time, begin, change, duration, a, p)
    if time == 0 then return begin end

    time = time / duration

    if time == 1 then return begin + change end

    if not p then p = duration * 0.3 end

    local s

    if not a or a < abs(change) then
        a = change
        s = p / 4
    else
        s = p / (2 * pi) * asin(change / a)
    end

    time = time - 1

    return
        -(a * pow(2, 10 * time) * sin((time * duration - s) * (2 * pi) / p)) +
            begin
end

-- a: amplitude
-- p: period
local function outElastic(time, begin, change, duration, a, p)
    if time == 0 then return begin end

    time = time / duration

    if time == 1 then return begin + change end

    if not p then p = duration * 0.3 end

    local s

    if not a or a < abs(change) then
        a = change
        s = p / 4
    else
        s = p / (2 * pi) * asin(change / a)
    end

    return a * pow(2, -10 * time) * sin((time * duration - s) * (2 * pi) / p) +
               change + begin
end

-- p = period
-- a = amplitud
local function inOutElastic(time, begin, change, duration, a, p)
    if time == 0 then return begin end

    time = time / duration * 2

    if time == 2 then return begin + change end

    if not p then p = duration * (0.3 * 1.5) end
    if not a then a = 0 end

    local s

    if not a or a < abs(change) then
        a = change
        s = p / 4
    else
        s = p / (2 * pi) * asin(change / a)
    end

    if time < 1 then
        time = time - 1
        return -0.5 *
                   (a * pow(2, 10 * time) *
                       sin((time * duration - s) * (2 * pi) / p)) + begin
    else
        time = time - 1
        return a * pow(2, -10 * time) *
                   sin((time * duration - s) * (2 * pi) / p) * 0.5 + change +
                   begin
    end
end

-- a: amplitud
-- p: period
local function outInElastic(time, begin, change, duration, a, p)
    if time < duration / 2 then
        return outElastic(time * 2, begin, change / 2, duration, a, p)
    else
        return inElastic((time * 2) - duration, begin + change / 2, change / 2,
                         duration, a, p)
    end
end

local function inBack(time, begin, change, duration, s)
    if not s then s = 1.70158 end
    time = time / duration
    return change * time * time * ((s + 1) * time - s) + begin
end

local function outBack(time, begin, change, duration, s)
    if not s then s = 1.70158 end
    time = time / duration - 1
    return change * (time * time * ((s + 1) * time + s) + 1) + begin
end

local function inOutBack(time, begin, change, duration, s)
    if not s then s = 1.70158 end
    s = s * 1.525
    time = time / duration * 2
    if time < 1 then
        return change / 2 * (time * time * ((s + 1) * time - s)) + begin
    else
        time = time - 2
        return change / 2 * (time * time * ((s + 1) * time + s) + 2) + begin
    end
end

local function outInBack(time, begin, change, duration, s)
    if time < duration / 2 then
        return outBack(time * 2, begin, change / 2, duration, s)
    else
        return inBack((time * 2) - duration, begin + change / 2, change / 2,
                      duration, s)
    end
end

local function outBounce(time, begin, change, duration)
    time = time / duration
    if time < 1 / 2.75 then
        return change * (7.5625 * time * time) + begin
    elseif time < 2 / 2.75 then
        time = time - (1.5 / 2.75)
        return change * (7.5625 * time * time + 0.75) + begin
    elseif time < 2.5 / 2.75 then
        time = time - (2.25 / 2.75)
        return change * (7.5625 * time * time + 0.9375) + begin
    else
        time = time - (2.625 / 2.75)
        return change * (7.5625 * time * time + 0.984375) + begin
    end
end

local function inBounce(time, begin, change, duration)
    return change - outBounce(duration - time, 0, change, duration) + begin
end

local function inOutBounce(time, begin, change, duration)
    if time < duration / 2 then
        return inBounce(time * 2, 0, change, duration) * 0.5 + begin
    else
        return outBounce(time * 2 - duration, 0, change, duration) * 0.5 +
                   change * .5 + begin
    end
end

local function outInBounce(time, begin, change, duration)
    if time < duration / 2 then
        return outBounce(time * 2, begin, change / 2, duration)
    else
        return inBounce((time * 2) - duration, begin + change / 2, change / 2,
                        duration)
    end
end

return {
    linear = linear,
    inQuad = inQuad,
    outQuad = outQuad,
    inOutQuad = inOutQuad,
    outInQuad = outInQuad,
    inCubic = inCubic,
    outCubic = outCubic,
    inOutCubic = inOutCubic,
    outInCubic = outInCubic,
    inQuart = inQuart,
    outQuart = outQuart,
    inOutQuart = inOutQuart,
    outInQuart = outInQuart,
    inQuint = inQuint,
    outQuint = outQuint,
    inOutQuint = inOutQuint,
    outInQuint = outInQuint,
    inSine = inSine,
    outSine = outSine,
    inOutSine = inOutSine,
    outInSine = outInSine,
    inExpo = inExpo,
    outExpo = outExpo,
    inOutExpo = inOutExpo,
    outInExpo = outInExpo,
    inCirc = inCirc,
    outCirc = outCirc,
    inOutCirc = inOutCirc,
    outInCirc = outInCirc,
    inElastic = inElastic,
    outElastic = outElastic,
    inOutElastic = inOutElastic,
    outInElastic = outInElastic,
    inBack = inBack,
    outBack = outBack,
    inOutBack = inOutBack,
    outInBack = outInBack,
    inBounce = inBounce,
    outBounce = outBounce,
    inOutBounce = inOutBounce,
    outInBounce = outInBounce
}
