#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

int main (int argc, char* argv[])
{
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window *window = SDL_CreateWindow("Hellow WOrld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI );
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    enum Direction{
        DOWN,
        LEFT,
        RIGHT,
        UP
    };
    
    SDL_Rect head{400,300,10,10};

    //body of snake
    std::deque<SDL_Rect> rectq;
    int size = 1;


    //food and foodgen 
    std::vector<SDL_Rect> food;
    for (int i = 0; i < 10; i++)
    {   
        auto temp = SDL_Rect{rand()%100*10, rand()%100*10, 10, 10};
        food.emplace_back(temp);
    }

    bool running = true;
    int dir = 0;
    while (running)
    {
        //check inputs
        //
        while (SDL_PollEvent( &event ) ) 
        {
            if (event.type == SDL_QUIT) {running = false;}
            if(event.type== SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_DOWN)   {dir = DOWN;    }
                if(event.key.keysym.sym == SDLK_UP)     {dir = UP;      }
                if(event.key.keysym.sym == SDLK_LEFT)   {dir = LEFT;    }
                if(event.key.keysym.sym == SDLK_RIGHT)  {dir = RIGHT;   }
            }
        }
        //check dir of moving
        switch (dir)
        {
        case DOWN:
            head.y += 10; break;
        case UP:
            head.y -= 10; break;
        case LEFT:
            head.x -= 10; break;
        case RIGHT:
            head.x += 10; break;
        default:
            dir = 0;
        }


        //collision detection w food
        std::for_each(food.begin(), food.end(), [&](auto& entity)
        {
            if (head.x == entity.x && head.y == entity.y)
            {
                size+=5;
                entity.x = -100;
                entity.y = -100;
            }
        } );

        //collision detection w self
        std::for_each(rectq.begin(), rectq.end(), [&](auto& snakeSeg)
        {
            if(head.x == snakeSeg.x && head.y == snakeSeg.y)
            {
                size = 1;
            }
        });

        //add newest head to snake
        rectq.push_front(head);

        // 
        while (rectq.size() >size)
        {
            rectq.pop_back();
        }
        


        //clear window
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        //draw body
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        std::for_each(rectq.begin(), rectq.end(), [&](auto& snakeSeg)
        {
            SDL_RenderFillRect(renderer, &snakeSeg);
        });

        //draw food
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        std::for_each(food.begin(), food.end(), [&](auto& entity)
        {
            SDL_RenderFillRect(renderer, &entity);
        });

        //display
        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }

    return EXIT_SUCCESS;
}
