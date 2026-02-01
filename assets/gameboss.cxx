#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Boss Battle", 0, 0, 600, 1000, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // NẠP ẢNH: Code sẽ ưu tiên tìm wolf.png, nếu không có sẽ tìm boss_idle_1.png
    SDL_Texture* bossTex = IMG_LoadTexture(ren, "boss_idle_1.png");
    if (bossTex == NULL) {
        bossTex = IMG_LoadTexture(ren, "boss_idle_1.png");
    }

    bool run = true;
    SDL_Event ev;
    while (run) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) run = false;
        }

        // Xóa màn hình (Màu trắng)
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);

        // VẼ BOSS: Đây là lệnh quan trọng để hình hiện lên thay vì chỉ có chữ
        if (bossTex != NULL) {
            SDL_Rect bossPos = {150, 200, 300, 300}; // Vị trí Boss trên màn hình
            SDL_RenderCopy(ren, bossTex, NULL, &bossPos);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(bossTex);
    SDL_Quit();
    return 0;
}
