#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    // Tạo cửa sổ theo thông số bạn đã chỉnh cho vừa màn hình
    SDL_Window* win = SDL_CreateWindow("Epic Forest", 0, 0, 750, 1420, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // NẠP ẢNH
    SDL_Texture* bgTex = IMG_LoadTexture(ren, "/sdcard/Download/background.png");
    // Nạp thêm chim để kiểm tra độ khớp
    SDL_Texture* bird1 = IMG_LoadTexture(ren, "/sdcard/Download/bird_walk1.png");

    bool run = true;
    SDL_Event ev;

    while (run) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) run = false;
        }

        // 1. Xóa màn hình
        SDL_RenderClear(ren);

        // 2. VẼ HÌNH NỀN (Sử dụng thông số 750, 1420 bạn đã chọn)
        if (bgTex) {
            SDL_Rect bgRect = {0, 0, 750, 1420}; 
            SDL_RenderCopy(ren, bgTex, NULL, &bgRect);
        }

        // 3. VẼ NHÂN VẬT (Ví dụ con chim đang đứng trên cỏ)
        if (bird1) {
            // Tôi đặt y = 1100 để con chim đứng trên thảm cỏ trong ảnh của bạn
            SDL_Rect birdRect = {300, 1100, 100, 100}; 
            SDL_RenderCopy(ren, bird1, NULL, &birdRect);
        }

        // Cập nhật màn hình
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(bird1);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
