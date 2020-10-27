module;
#include <vector>
module TnT.Input;

inline static struct input_handle_t final
{
    inline input_handle_t()
    {
        prevkb.assign(currentkb, currentkb + keyLength);
        if (int const &jnum{SDL_NumJoysticks()}; jnum > 0)
        {
            for (int i{0}; i < jnum; ++i)
                if (SDL_Joystick *joy = SDL_JoystickOpen(i);
                    joy)
                    joysticks.emplace_back(joy);
            joySense.reserve(jnum);
            std::fill_n(&joySense[0], joySense.size(), 8000);
        }
    }

    inline ~input_handle_t() noexcept
    {
        for (auto const &it : joysticks)
            SDL_JoystickClose(it);
    }

    int keyLength{0};
    int mX{0}, mY{0};

    Uint32 currentMouse{0};
    Uint32 prevMouse{0};
    Uint32 lastMouse{0};

    const Uint8 *currentkb{SDL_GetKeyboardState(&keyLength)};
    std::vector<Uint8> prevkb;
    std::vector<SDL_Joystick *> joysticks;
    std::vector<Sint16> joySense;
} input_handle;

bool tnt::input::keyDown(SDL_Scancode key) noexcept
{
    return (input_handle.currentkb[key] != 0);
}

bool tnt::input::keyPressed(SDL_Scancode key) noexcept
{
    return ((input_handle.prevkb[key] == 0) && (input_handle.currentkb[key] != 0));
}

bool tnt::input::keyReleased(SDL_Scancode key) noexcept
{
    return ((input_handle.prevkb[key] != 0) && (input_handle.currentkb[key] == 0));
}

SDL_Scancode tnt::input::lastKeyPressed(const SDL_Event &e) noexcept
{
    return e.key.keysym.scancode;
}

bool tnt::input::mouseButtonDown(Uint32 button) noexcept
{
    return ((input_handle.currentMouse & (1u << button)) != 0);
}

bool tnt::input::mouseButtonPressed(Uint32 button) noexcept
{
    return (((input_handle.prevMouse & (1u << button)) == 0) &&
            ((input_handle.currentMouse & (1u << button)) != 0));
}

bool tnt::input::mouseButtonReleased(Uint32 button) noexcept
{
    return (((input_handle.prevMouse & (1u << button)) != 0) &&
            ((input_handle.currentMouse & (1u << button)) == 0));
}

unsigned tnt::input::lastMouseButton() noexcept
{
    for (Uint32 i{0}; i < 5; ++i)
        if (input_handle.currentMouse & (1 << i))
            input_handle.lastMouse = i;
    for (Uint32 i{0}; i < 5; ++i)
        if (input_handle.prevMouse & (1 << i))
            input_handle.lastMouse = i;

    return input_handle.lastMouse;
}

void tnt::input::updateCurrent()
{
    input_handle.currentMouse = SDL_GetMouseState(&input_handle.mX, &input_handle.mY);
    input_handle.currentkb = SDL_GetKeyboardState(&input_handle.keyLength);
    SDL_PumpEvents();
}

void tnt::input::updatePrevious()
{
    input_handle.prevkb.assign(input_handle.currentkb, input_handle.currentkb + input_handle.keyLength);
    input_handle.prevMouse = input_handle.currentMouse;
}

void tnt::input::updateJoystick()
{
    if (std::size_t const &size{input_handle.joySense.size()};
        size != SDL_NumJoysticks())
    {
        int jnum{SDL_NumJoysticks()};
        for (std::size_t i{size}; i < jnum; ++i)
            if (SDL_Joystick *joy = SDL_JoystickOpen((int)i);
                joy)
                input_handle.joysticks.emplace_back(joy);

        input_handle.joySense.reserve(SDL_NumJoysticks() - size);
        std::fill_n(&input_handle.joySense[size], input_handle.joySense.capacity() - size, 8000);
    }
}

void tnt::input::Update()
{
    updatePrevious();
    updateCurrent();
    updateJoystick();
}

std::pair<int, int> tnt::input::mousePosition() noexcept
{
    return std::make_pair(input_handle.mX, input_handle.mY);
}

void tnt::input::setDefaultDeadZone(Sint16 sense) noexcept
{
    std::fill_n(&input_handle.joySense[0], input_handle.joySense.size(), sense);
}

void tnt::input::setDeadZone(SDL_JoystickID id, Sint16 sense) noexcept
{
    input_handle.joySense[id] = sense;
}

Sint16 tnt::input::getDeadZone(SDL_JoystickID id) noexcept
{
    return input_handle.joySense[id];
}