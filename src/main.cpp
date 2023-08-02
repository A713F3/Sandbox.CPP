#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "sandbox.cpp"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

using namespace std;
int main(int argc, char *argv[])
{

    // Init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL Initializing ERROR: %s\n", SDL_GetError());
    }

    // Init TTF.
    if (TTF_Init() != 0) {
        printf("TTF Initializing ERROR: %s\n", TTF_GetError());
    }
    
    // Create the window.
    SDL_Window* win = SDL_CreateWindow("Sandbox",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    // Create renderer.
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, render_flags);

    TTF_Font* font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 20);
    if (!font) {
        printf("TTF Font Loading ERROR: %s\n", TTF_GetError());
    }

    SDL_Color text_color = {200, 200, 200, 255};
    SDL_Surface *surf = TTF_RenderText_Blended(font, "Press 'w' for water, 's' for sand", text_color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    
    int iW, iH;
    SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
    int text_x = SCREEN_WIDTH / 2 - iW / 2;
    int text_y = iH / 2 + 5;

    SDL_Rect text_rect = {.x = text_x, .y = text_y, .w = iW, .h = iH};

    int mouse_x, mouse_y, cell_size = 10;

    cell_state state;
    Board board = Board(50, 50, cell_size);

    // Main loop.
    bool running = true;
    while (running) {
        SDL_Event event;

        // Handle events.
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_w)
                {
                    state = WATER;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    state = SAND;
                }
                else 
                {
                    state = EMPTY;
                }
                break;

            case SDL_KEYUP:
                state = EMPTY;
                break;

            default:
                break;
            }
        }

        if (state != EMPTY)
        {
            SDL_GetMouseState(&mouse_x, &mouse_y);

            board.set_cell(mouse_x / cell_size, mouse_y / cell_size, state);
        }

        SDL_SetRenderDrawColor(renderer, 51, 51, 51, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        board.render(renderer);
        board.update();

        SDL_RenderCopy(renderer, texture, NULL, &text_rect);

        // Render screen.
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    // Quit SDL.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}