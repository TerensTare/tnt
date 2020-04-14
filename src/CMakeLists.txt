set(SOURCE_FILES
    fileIO/VirtualFileSystem.cpp
    gui/Widget.cpp core/Window.cpp utils/Actions.cpp
    utils/Timer.cpp core/Application.cpp 
    fileIO/AssetManager.cpp core/AudioPlayer.cpp
    core/Camera.cpp InputManager.cpp LuaSandbox.cpp
    math/Matrix.cpp ecs/Object.cpp core/Scene.cpp)

if (TNT_DO_EXPERIMENTS)
    list(APPEND SOURCE_FILES exp/Runtimer.cpp)
endif()