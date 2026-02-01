#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>

// --- CẤU TRÚC GAME ---
struct Fireball {
    float x, y, vx, vy;
    int frame;
};
std::vector<Fireball> bossBullets;

struct Enemy {
    float x; int hp, maxHp, type; 
    bool active, isBoss, flip; float speed;
};

struct ShopItem { 
    std::string name; int price; SDL_Color col;
};

struct DamageText {
    float x, y;
    int value;
    int life;
};
std::vector<DamageText> dmgTexts;

// BIẾN TOÀN CỤC
int Gold = 0, MonstersKilled = 0, BossThreshold = 13;
int MaxHP = 100, CurrentHP = 100, MaxEnergy = 50, CurrentEnergy = 50, Damage = 15, Armor = 0;
float playerX = 100, playerY = 730, walkCycle = 0;
float jumpV = 0; bool isJumping = false; 
bool isAtk = false, isShopOpen = false, hasSword = false, hL = false, hR = false;
int shopScrollY = 0, lastMouseY = 0;
std::vector<Enemy> enemies;
float kbV = 0; 
bool hasHelmet = false, hasChest = false, hasPants = false, hasBoots = false;
bool isHealing = false; int healTimer = 0; int healSpeed = 5; 
enum JumpState { JUMP_START, JUMP_UP, JUMP_FALL, JUMP_LAND };
JumpState jumpState = JUMP_LAND;
float atkAngle = 0;

// BIẾN LƯU ẢNH
SDL_Texture* bossTex[5];
SDL_Texture* fireTex[5];
SDL_Texture* bgTex = NULL;
SDL_Texture* wolfTex = NULL; 

// --- CÁC HÀM HỖ TRỢ VẼ (GIỮ NGUYÊN ĐỂ GAME CHẠY) ---
void drawChar(SDL_Renderer* r, char c, int x, int y, int sz, SDL_Color col) {
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
    switch(toupper(c)) {
        case 'A': SDL_RenderDrawLine(r, x, y+sz, x+sz/2, y); SDL_RenderDrawLine(r, x+sz/2, y, x+sz, y+sz); SDL_RenderDrawLine(r, x+sz/4, y+sz/2, x+3*sz/4, y+sz/2); break;
        case 'B': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x, y, x+sz, y+sz/4); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+sz/4); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+3*sz/4); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+3*sz/4); break;
        case 'C': SDL_RenderDrawLine(r, x+sz, y, x, y); SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz); break;
        case 'D': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz/2); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz/2); break;
        case 'E': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+sz/2); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz); break;
        case 'G': SDL_RenderDrawLine(r, x+sz, y, x, y); SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz); SDL_RenderDrawLine(r, x+sz, y+sz, x+sz, y+sz/2); SDL_RenderDrawLine(r, x+sz/2, y+sz/2, x+sz, y+sz/2); break;
        case 'H': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+sz/2); break;
        case 'I': SDL_RenderDrawLine(r, x+sz/2, y, x+sz/2, y+sz); SDL_RenderDrawLine(r, x, y, x+sz, y); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz); break;
        case 'K': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz/2, y, x+sz, y); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+sz); break;
        case 'L': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz); break;
        case 'M': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz); SDL_RenderDrawLine(r, x, y, x+sz/2, y+sz/2); SDL_RenderDrawLine(r, x+sz/2, y+sz/2, x+sz, y); break;
        case 'N': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz); SDL_RenderDrawLine(r, x, y, x+sz, y+sz); break;
        case 'O': { SDL_Rect ro={x,y,sz,sz}; SDL_RenderDrawRect(r, &ro); break; }
        case 'P': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x, y, x+sz, y); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz/2); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+sz/2); break;
        case 'Q': { SDL_Rect rq={x,y,sz,sz}; SDL_RenderDrawRect(r, &rq); SDL_RenderDrawLine(r, x+sz/2, y+sz/2, x+sz, y+sz); break; }
        case 'S': SDL_RenderDrawLine(r, x+sz, y, x, y); SDL_RenderDrawLine(r, x, y, x, y+sz/2); SDL_RenderDrawLine(r, x, y+sz/2, x+sz, y+sz/2); SDL_RenderDrawLine(r, x+sz, y+sz/2, x+sz, y+sz); SDL_RenderDrawLine(r, x+sz, y+sz, x, y+sz); break;
        case 'U': SDL_RenderDrawLine(r, x, y, x, y+sz); SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz); SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz); break;
        case 'X': SDL_RenderDrawLine(r, x, y, x+sz, y+sz); SDL_RenderDrawLine(r, x+sz, y, x, y+sz); break;
    }
}

void drawString(SDL_Renderer* r, std::string s, int x, int y, int sz, SDL_Color col) {
    for(int i=0; i<(int)s.length(); i++) drawChar(r, s[i], x + i*(sz+8), y, sz, col);
}

void drawNum(SDL_Renderer* r, int num, int x, int y, int sz, SDL_Color col) {
    std::string s = std::to_string(num); SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
    for (char c : s) {
        int n = c - '0';
        if (n!=1 && n!=4) SDL_RenderDrawLine(r, x, y, x+sz, y);
        if (n!=1 && n!=2 && n!=3 && n!=7) SDL_RenderDrawLine(r, x, y, x, y+sz);
        if (n!=5 && n!=6) SDL_RenderDrawLine(r, x+sz, y, x+sz, y+sz);
        if (n!=0 && n!=1 && n!=7) SDL_RenderDrawLine(r, x, y+sz, x+sz, y+sz);
        if (n==0 || n==2 || n==6 || n==8) SDL_RenderDrawLine(r, x, y+sz, x, y+2*sz);
        if (n!=2) SDL_RenderDrawLine(r, x+sz, y+sz, x+sz, y+2*sz);
        if (n!=1 && n!=4 && n!=7) SDL_RenderDrawLine(r, x, y+2*sz, x+sz, y+2*sz);
        x += sz + 10;
    }
}

void drawBox(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, 255); SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(r, &rect); SDL_SetRenderDrawColor(r, 255, 255, 255, 255); SDL_RenderDrawRect(r, &rect);
}

void drawPlayer(SDL_Renderer* r, float px, float py, bool attacking, bool hasSword, float cycle, int armor, bool jumping) {
    int x = (int)px, y = (int)py;
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_Rect head = {x-10, y-30, 20, 20}; SDL_RenderDrawRect(r, &head);
    if (hasHelmet) { SDL_SetRenderDrawColor(r, 150, 150, 150, 255); SDL_Rect hel = {x-12, y-35, 24, 12}; SDL_RenderFillRect(r, &hel); }
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255); SDL_RenderDrawLine(r, x, y-10, x, y+25); 
    if (hasChest) { SDL_SetRenderDrawColor(r, 130, 130, 130, 255); SDL_Rect chs = {x-8, y-5, 16, 25}; SDL_RenderFillRect(r, &chs); }
    float legSwing = jumping ? 0 : sin(cycle) * 20;
    int lx1 = x-10+legSwing, ly1 = y+70, lx2 = x+10-legSwing, ly2 = y+70;
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255); SDL_RenderDrawLine(r, x, y+25, lx1, ly1); SDL_RenderDrawLine(r, x, y+25, lx2, ly2);
    float armSwing = jumping ? 0 : sin(cycle + 3.14f) * 15;
    if (attacking) {
        float slashRad = (atkAngle - 45) * 3.14159f / 180.0f;
        int handX = x + (int)(cos(slashRad)*25), handY = y + (int)(sin(slashRad)*25);
        SDL_RenderDrawLine(r, x, y+5, handX, handY);
        if (hasSword) { int swordEndX = handX + (int)(cos(slashRad)*55), swordEndY = handY + (int)(sin(slashRad)*55); SDL_SetRenderDrawColor(r, 200, 200, 200, 255); SDL_RenderDrawLine(r, handX, handY, swordEndX, swordEndY); }
    } else {
        int hX = x+15-armSwing, hY = y+25; SDL_RenderDrawLine(r, x, y+5, hX, hY);
        if (hasSword) { SDL_SetRenderDrawColor(r, 150, 150, 150, 255); SDL_RenderDrawLine(r, hX, hY, hX+15, hY+40); }
    }
}

// --- HÀM VẼ QUÁI (DÙNG ẢNH WOLF) ---
void drawMonster(SDL_Renderer* r, Enemy& e) {
    int x = (int)e.x, gY = 800; SDL_Color red = {255, 0, 0}; 
    SDL_RendererFlip flip = e.flip ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (e.isBoss) {
        int currentFrame = (SDL_GetTicks() / 150) % 5;
        SDL_Rect bRect = {x - 130, gY - 260, 260, 260}; 
        if(bossTex[currentFrame]) SDL_RenderCopy(r, bossTex[currentFrame], NULL, &bRect);
        drawString(r, "BOSS", x-45, gY-280, 16, {0, 0, 0});
        drawBox(r, x-70, gY-310, (int)(140.0f * e.hp / e.maxHp), 15, red);
    } else {
        SDL_Rect wRect = {x - 50, gY - 100, 100, 100}; 
        if(wolfTex) SDL_RenderCopyEx(r, wolfTex, NULL, &wRect, 0, NULL, flip);
        else drawBox(r, x-20, gY-80, 40, 20, red);
        drawBox(r, x-40, gY-120, (int)(80.0f * e.hp / e.maxHp), 10, red);
    }
}

void spawn(float x, int hp, float spd, int type) {
    Enemy e; e.x = x; e.hp = e.maxHp = hp; e.speed = spd; e.type = type;
    e.isBoss = (type == 0); e.active = true; e.flip = false; enemies.push_back(e);
}

std::vector<ShopItem> shopItems = { {"KIEM", 50, {100,100,100}}, {"MAU", 30, {200,0,0}}, {"NANG LUONG", 25, {0,0,200}}, {"MU GIAP", 40, {120,120,120}}, {"AO GIAP", 70, {120,120,120}}, {"QUAN GIAP", 60, {120,120,120}}, {"GIAY GIAP", 35, {120,120,120}} };

int main(int argc, char* argv[]) {
    srand(time(0)); SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Epic Wolf Game", 0, 0, 600, 1000, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // --- TẢI ẢNH TỪ THƯ MỤC DOWNLOAD ---
    bgTex = IMG_LoadTexture(ren, "/sdcard/Download/background.png");
    wolfTex = IMG_LoadTexture(ren, "/sdcard/Download/wolf.png");
    for (int i = 0; i < 5; i++) {
        char bName[128], fName[128];
        sprintf(bName, "/sdcard/Download/boss_idle_%d.png", i + 1);
        bossTex[i] = IMG_LoadTexture(ren, bName);
        sprintf(fName, "/sdcard/Download/fireball_%d.png", i + 1);
        fireTex[i] = IMG_LoadTexture(ren, fName);
    }

    spawn(500, 200, 1.6f, 1);
    bool run = true; SDL_Event ev; int dmgC = 0;
    while (run) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) run = false;
            if (ev.type == SDL_MOUSEBUTTONDOWN) {
                lastMouseY = ev.button.y; int mx = ev.button.x, my = ev.button.y;
                if (isShopOpen) { if (my > 700) isShopOpen = false; else { for(int i=0; i<(int)shopItems.size(); i++) { int itemY = 280 + i*95 + shopScrollY; if (mx > 80 && mx < 520 && my > itemY && my < itemY+85 && Gold >= shopItems[i].price) { Gold -= shopItems[i].price; if(i==0) { Damage += 20; hasSword = true; } else if(i==1) { MaxHP += 20; CurrentHP = MaxHP; isHealing = false; } else if(i==3) { hasHelmet = true; Armor += 1; } } } } }
                else { if (my < 100 && mx > 220 && mx < 370) isShopOpen = true; if (my >= 830) { if (mx >= 10 && mx <= 230) hL = true; else if (mx >= 240 && mx <= 360 && !isJumping) { isJumping = true; jumpV = -20; } else if (mx >= 370 && my < 950) hR = true; else if (my >= 950 && CurrentEnergy >= 10) { isAtk = true; CurrentEnergy -= 10; for(auto &e : enemies) if(e.active && std::abs(playerX - e.x) < 120) { e.hp -= Damage; dmgTexts.push_back({e.x, 700, Damage, 40}); if(e.hp <= 0) { e.active = false; Gold += 15; MonstersKilled++; if(MonstersKilled % BossThreshold == 0) spawn(600, 1000, 0.8f, 0); } } } } }
            }
            if (ev.type == SDL_MOUSEBUTTONUP) hL = hR = isAtk = false;
        }

        if (!isShopOpen) {
            playerX += kbV; kbV *= 0.88f; 
            if (hL) playerX -= 7; if (hR) playerX += 7;
            if (!isJumping && (hL || hR)) walkCycle += 0.5;
            if (isJumping) { playerY += jumpV; jumpV += 1.2f; if (playerY >= 730) { playerY = 730; isJumping = false; jumpV = 0; } }
            if (isAtk) { atkAngle += 25; if (atkAngle > 120) atkAngle = 120; } else atkAngle = 0;
            if (CurrentEnergy < MaxEnergy) CurrentEnergy++; if (dmgC > 0) dmgC--;

            for(auto &e : enemies) if(e.active && e.isBoss && (SDL_GetTicks() % 1200 < 20)) {
                float dx = playerX - e.x, dy = (playerY - 20) - 600.0f; 
                float dist = sqrt(dx*dx + dy*dy); bossBullets.push_back({e.x, 600.0f, (dx/dist)*9.0f, (dy/dist)*9.0f, 0});
            }
            for(int i=0; i < (int)bossBullets.size(); i++) {
                bossBullets[i].x += bossBullets[i].vx; bossBullets[i].y += bossBullets[i].vy;
                SDL_Rect pRect = {(int)playerX-10, (int)playerY-30, 30, 100}, fRect = {(int)bossBullets[i].x - 70, (int)bossBullets[i].y - 70, 140, 140}; 
                if(SDL_HasIntersection(&pRect, &fRect)) { CurrentHP -= std::max(5, 15 - Armor); bossBullets.erase(bossBullets.begin() + i); i--; dmgC = 30; }
            }
            for(auto &e : enemies) { if(!e.active) continue; e.flip = (e.x < playerX); if (e.x > playerX + 60) e.x -= e.speed; else if (e.x < playerX - 60) e.x += e.speed; if (std::abs(playerX - e.x) < 70 && dmgC <= 0 && !isJumping) { CurrentHP -= std::max(5, 10 - Armor); dmgC = 60; } }
            if(!enemies.size() || std::all_of(enemies.begin(), enemies.end(), [](const Enemy& e){return !e.active;})) spawn(600, 200, 1.6f, 1);
            if (CurrentHP <= 0) { CurrentHP = MaxHP; playerX = 300; playerY = 730; bossBullets.clear(); }
        }

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); SDL_RenderClear(ren);
        if(bgTex) SDL_RenderCopy(ren, bgTex, NULL, NULL);
        drawPlayer(ren, playerX, playerY, isAtk, hasSword, walkCycle, Armor, isJumping);
        for(auto &e : enemies) if(e.active) drawMonster(ren, e);
        for(auto &b : bossBullets) {
            SDL_Rect r = {(int)b.x - 100, (int)b.y - 100, 200, 200};
            if(fireTex[0]) SDL_RenderCopyEx(ren, fireTex[(SDL_GetTicks()/100)%5], NULL, &r, (atan2(b.vy, b.vx)*180/3.14)+180, NULL, SDL_FLIP_NONE);
        }
        drawBox(ren, 10, 110, (int)(150.0f * CurrentHP / MaxHP), 20, {255,0,0});
        drawBox(ren, 10, 140, (int)(150.0f * CurrentEnergy / MaxEnergy), 20, {0,0,255});
        drawBox(ren, 220, 20, 150, 70, {150,0,0}); drawString(ren, "SHOP", 255, 45, 10, {255,255,255});
        drawBox(ren, 400, 20, 180, 70, {212,175,55}); drawNum(ren, Gold, 420, 35, 10, {255,255,255});
        if (isShopOpen) { drawBox(ren, 40, 240, 520, 480, {50,40,40}); }
        drawBox(ren, 10, 830, 220, 100, {120,120,120}); drawBox(ren, 240, 830, 120, 100, {80,80,80}); 
        drawBox(ren, 370, 830, 220, 100, {120,120,120}); drawBox(ren, 310, 955, 280, 200, {0,0,0}); 
        SDL_RenderPresent(ren); SDL_Delay(16);
    }
    SDL_Quit(); return 0;
}
