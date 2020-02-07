1. Global functions, classes/structs/enums/unions and static class function names are written Uppercase.
2. Constants are written FULLY UPPERCASE.
3. Others are written starting lowercase.

TODO:
1. Add a level parser for the Direct2D version of the engine.
2. Support multiple languages by using JSON files.

- The main idea of this project is to generate and then build header & source files from YAML/JSON/XML files, which is partially done for JSON files.

- In SDL_RenderCopy, srcRect is used for animations, while dstRect is used for the "Camera".

- Consider making **AssetManager** a NON Singleton and adding an **AssetManager** for each Scene.

- Every object that should be rendered should be pushed in the render array/queue/stack/vetor that **Renderer**
has and then be rendered in their order.

- Create a Console **Widget**.

- Create a tool to convert tntlua to C++ OR use lua for projects.

- Add a python script to translate files.

- Engine structure:
https://app.gitmind.com/doc/d8b92aae9e6178ac938ffdb1163f8fce

``` C++
#include "Scene.hpp"

struct Game
{
    // Scene is linked with the previous and the next scene for each desirable condition.
    tnt::Scene* scenes;
    tnt::Window* win;
    
    tnt::InputHandler* h{InputHandler::This()};
    // ...
}

struct Scene
{
    tnt::Widget* baseW; // Widgets store a pointer to their parent.
    tnt::Entity* entities[];
    tnt::AssetManager* assets;
};
```