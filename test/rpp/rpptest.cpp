#include <iostream>

#include <SDL2/SDL.h>
#include "exp/Runtimer.hpp"


typedef int(*answer_t)();
answer_t answer = NULL;

int main([[maybe_unused]] int argc,[[maybe_unused]] char* argv[])
{
    using tnt::rpp::RuntimeManager;

	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
	{
		std::cout << "couldn't init sdl2!! Error: " << SDL_GetError() << '\n';
		return -1;
	}
	
	RuntimeManager* rpp{new RuntimeManager{}};

    rpp->LoadObject("test", "testlib.c");
	
	std::cout << "test error " << SDL_GetError() << '\n';
	
	answer = *static_cast<answer_t*>rpp->LoadFunction("test", "answer");
	
	bool running{true};
	
	while(running)
	{
		rpp->Update();
		
		
    if (answer != NULL)
            std::cout << answer();
        else
		{
			std::cout << "int answer() couldn't be loaded correctly! Error:" << SDL_GetError() 
				<<".\n exiting...";
			running = false;
		}
	}
//        if (answer() == 43)
//        {
//            tnt::logger::info("Succesfully exiting...");
//            running = false;
//        }
//        else
//            continue;
	
	delete rpp;
	SDL_Quit();
	
    return 0;
}