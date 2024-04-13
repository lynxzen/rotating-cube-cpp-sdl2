#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <vector>

class Screen {
private:
    // basic SDL properties needed
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;
    float scale = 1.0f;
    
public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(
            640*2, 480*2, 0, &window, &renderer
        );
        SDL_RenderSetScale(renderer,2,2);
    }

    ~Screen() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    // create the pixel and send to the vector of pixels
    void pixel(float x, float y) {
        points.emplace_back(SDL_FPoint{x * scale, y * scale});
    }

    void show() {
        // draw black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw white pixels
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto& point : points) {
            SDL_RenderDrawPointF(renderer, point.x, point.y);
        }

        // update the screen
        SDL_RenderPresent(renderer);
    }

    // clear the screen if you'd like
    void clear() {
        points.clear();
    }

    // event handling
    void input() {
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
            if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y > 0) {
                    scale *= 1.1f;
                }
                else if (e.wheel.y < 0) {
                    scale /= 1.1f;
              }
                SDL_RenderSetScale(renderer, scale, scale);
            }
        }
    }
};
