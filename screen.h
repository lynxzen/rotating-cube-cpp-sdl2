#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <vector>

class Screen {
private:
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

    void pixel(float x, float y) {
        points.emplace_back(SDL_FPoint{x * scale, y * scale});
    }

    void show() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto& point : points) {
            SDL_RenderDrawPointF(renderer, point.x, point.y);
        }

        SDL_RenderPresent(renderer);
    }

    void clear() {
        points.clear();
    }

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
