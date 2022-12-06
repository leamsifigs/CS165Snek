#include <iostream>
#include <SDL2/SDL.h>

int main (int argc, char* argv[])
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window *window = SDL_CreateWindow("Hellow WOrld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI );
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    bool running = true;

    while (running)
    {
        //check inputs
        //
        while (SDL_PollEvent( &event ) ) 
        {
            if (event.type == SDL_QUIT) {running = false;}
        }

        //move

        //clear window
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        //draw body
        SDL_RenderPresent(renderer);
        SDL_Delay(25);
    }

    return EXIT_SUCCESS;
}
