#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Timer.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "AudioPlayer.hpp"
#include "InputManager.hpp"

namespace tnt
{
// this is the class that TnT is going to use as a base for creating games.
class Game
{
public:
    virtual void Run() = 0;

protected:
    Game();
    ~Game() noexcept;

    void EarlyUpdate();
    void Update();
    void LateUpdate();

    void Render();

    Scene *scene;
    Timer *timer;
    Window *window;
    AudioPlayer audio;
    InputManager input;
};
} // namespace tnt

#endif //!APPLICATION_HPP