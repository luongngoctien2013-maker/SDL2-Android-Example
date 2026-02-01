#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // 1. Khởi tạo SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Bird Walk Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 2. Nạp ảnh (Sử dụng tên file bạn đã đặt)
    SDL_Texture* frame1 = IMG_LoadTexture(renderer, "bird_walk1.png");
    SDL_Texture* frame2 = IMG_LoadTexture(renderer, "bird_walk2.png");

    // Kiểm tra lỗi nếu không tìm thấy ảnh
    if (!frame1 || !frame2) {
        printf("Loi nap anh: %s\n", IMG_GetError());
        return -1;
    }

    // 3. Thiết lập vị trí và kích thước con chim
    SDL_Rect birdRect = {100, 200, 150, 150}; // x, y, width, height
    int speed = 5;
    bool running = true;
    SDL_Event event;

    // 4. Vòng lặp game
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Cập nhật vị trí di chuyển qua lại
        birdRect.x += speed;
        if (birdRect.x > 600 || birdRect.x < 0) speed = -speed;

        // Xóa màn hình (màu trắng)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // 5. Logic chuyển khung hình (Animation)
        // Cứ mỗi 150ms thì đổi ảnh một lần
        int frame = (SDL_GetTicks() / 150) % 2;
        
        if (frame == 0) {
            SDL_RenderCopy(renderer, frame1, NULL, &birdRect);
        } else {
            SDL_RenderCopy(renderer, frame2, NULL, &birdRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Giới hạn khoảng 60 khung hình/giây
    }

    // Giải phóng bộ nhớ
    SDL_DestroyTexture(frame1);
    SDL_DestroyTexture(frame2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
