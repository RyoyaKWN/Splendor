#include <cmath>
#include "SDLHelper.hpp"

const char* FONT_PATH = "./assets/fonts/mplus-2p-black.ttf"; // TTFフォントへのパス

SDLHelper::SDLHelper(const std::string& windowTitle, int width, int height){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDLHelper::~SDLHelper(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

SDL_Texture* SDLHelper::loadTexture(){
    SDL_Surface* loadedSurface = SDL_LoadBMP(FONT_PATH);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

SDL_Texture* SDLHelper::renderText(const std::string& text, SDL_Color color, int fontSize){
    TTF_Font* font = TTF_OpenFont(FONT_PATH, fontSize);
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    return textTexture;
}

void SDLHelper::drawText(const std::string& text, SDL_Color color, int fontSize, int x, int y){
    SDL_Texture* textTexture = renderText(text, color, fontSize);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
}

void SDLHelper::drawText(SDL_Texture* textTexture, int x, int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
}

void SDLHelper::drawRect(int x, int y, int width, int height, SDL_Color color, SDL_Texture* textTexture){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);

    // 矩形の中心にテキスト
    drawTextCenter(textTexture, rect);
}

void SDLHelper::drawCircle(int x, int y, int r, SDL_Color color, SDL_Texture* textTexture){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int dx, dy, px, py1, py2;
    for(dx = -r; dx <= r; dx++){
        dy = sqrt(r * r - dx * dx) + 0.5;
        px = x + dx;
        py1 = y - dy;
        py2 = y + dy;
        SDL_RenderDrawLine(renderer, px, py1, px, py2);
    }

    // 円の中心にテキスト
    drawTextCenter(textTexture, {x, y, 0, 0});
}

void SDLHelper::drawTextCenter(SDL_Texture* textTexture, SDL_Rect rect){
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);
    int textX = rect.x + (rect.w - textWidth) / 2;
    int textY = rect.y + (rect.h - textHeight) / 2;
    drawText(textTexture, textX, textY);
}

void SDLHelper::setRenderColor(const SDL_Color& color){
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SDLHelper::present(){
    SDL_RenderPresent(renderer);
}

void SDLHelper::clear(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

SDL_Renderer* SDLHelper::getRenderer(){
    return renderer;
}