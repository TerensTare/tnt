set(SOURCE_FILES
    fileIO/VirtualFileSystem.cpp
    gui/Widget.cpp gui/Window.cpp physics/Contact.cpp
    physics/ForceGenerator.cpp physics/Link.cpp
    physics/Particle.cpp physics/World.cpp
    utils/Actions.cpp utils/Timer.cpp
    Application.cpp AssetManager.cpp AudioPlayer.cpp
    Camera.cpp InputManager.cpp LuaSandbox.cpp
    Matrix.cpp Object.cpp Scene.cpp)

if (TNT_DO_EXPERIMENTS)
    list(APPEND SOURCE_FILES exp/Runtimer.cpp)
endif()