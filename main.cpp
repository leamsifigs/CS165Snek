#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

int main (int argc, char* argv[])
{
    srand((unsigned int) time(NULL));
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

    class Snakes {
        public:
            Snakes() {
                size = 1;
                head = {400,300,10,10};
            }
            void resetLocation() {
                size = 1;
                head.x = 400;
                head.y = 300;
            };

            SDL_Rect head;
            std::deque<SDL_Rect> segments;
            int size;
    };

    class Foods {
        public:
        Foods() {
            for (int i = 0; i < foodGenNum; i++)
            {   
                auto temp = SDL_Rect{rand()%80*10, rand()%60*10, 10, 10};
                foodVector.emplace_back(temp);
            }
            count = foodGenNum;   
        }
        void regenerate_food() 
            {
                std::for_each(foodVector.begin(), foodVector.end(), [&](auto& entity) {
                    entity.x = rand()%80*10;
                    entity.y = rand()%60*10;
                });
                count = foodGenNum;
            }
        int     getCount()          {return count;}
        void    decrementCount()    {count--;}
        void    incrementCount()    {count++;}
        int     getFoodGenNum()     {return foodGenNum;}
        
        std::vector<SDL_Rect>foodVector;
        private:
            int count = 0;
            int foodGenNum = 10;
    };

    Snakes snake;
    Foods food;
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
            snake.head.y += 10; break;
        case UP:
            snake.head.y -= 10; break;
        case LEFT:
            snake.head.x -= 10; break;
        case RIGHT:
            snake.head.x += 10; break;
        default:
            dir = 0;
        }


        //collision detection w food
        std::for_each(food.foodVector.begin(), food.foodVector.end(), [&](auto& entity)
        {
            if (snake.head.x == entity.x && snake.head.y == entity.y)
            {
                snake.size += 5;
                entity.x = -100;
                entity.y = -100;
                food.decrementCount();
            }

            if (food.getCount() == 0) {food.regenerate_food();}
        } );

        //collision detection w self
        std::for_each(snake.segments.begin(), snake.segments.end(), [&](auto& snakeSeg)
        {
            if(snake.head.x == snakeSeg.x && snake.head.y == snakeSeg.y)
            {
                snake.size = 1;
                food.regenerate_food();
                // placeholder for reset state/gameover

            }
        });

        //add newest head to snake
        snake.segments.push_front(snake.head);

        // 
        while (snake.segments.size() > snake.size)
        {
            snake.segments.pop_back();
        }
        
        // colision detection with walls
        if (snake.head.x == 0 || snake.head.y == 0 || snake.head.x == 800 || snake.head.y == 600)
        {
            snake.resetLocation();
            food.regenerate_food();
        }

        //clear window
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        //draw body
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        std::for_each(snake.segments.begin(), snake.segments.end(), [&](auto& snakeSeg)
        {
            SDL_RenderFillRect(renderer, &snakeSeg);
        });

        //draw food
        SDL_SetRenderDrawColor(renderer, 255,0,0,255);
        std::for_each(food.foodVector.begin(), food.foodVector.end(), [&](auto& entity)
        {
            SDL_RenderFillRect(renderer, &entity);
        });

        //display
        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }

    return EXIT_SUCCESS;
}
