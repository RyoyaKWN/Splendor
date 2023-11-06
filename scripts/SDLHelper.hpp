#ifndef SDLHelper_HPP
#define SDLHelper_HPP

// #pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<string>

class SDLHelper{
public:
    SDLHelper(const std::string& windowTitle, int width, int Height);
    ~SDLHelper();

    // テクスチャの読み込み
    SDL_Texture* loadTexture();

    // テキストを描画
    SDL_Texture* renderText(const std::string& text, SDL_Color color, int fontSize);
    void drawText(const std::string& text, SDL_Color color, int fontSize, int x, int y);
    void drawText(SDL_Texture* textTexture, int x, int y);

    // 矩形・円形の描画
    void drawRect(int x, int y, int width, int height, SDL_Color color, SDL_Texture* textTexture);
    void drawCircle(int x, int y, int radius, SDL_Color color, SDL_Texture* textTexture);

    // テキストをセンタリングして描画
    void drawTextCenter(SDL_Texture* textTexture, SDL_Rect rect);
    
    // 描画色の設定
    void setRenderColor(const SDL_Color& color);

    // 描画の反映
    void present();

    // レンダラの内容をクリア
    void clear();

    SDL_Renderer* getRenderer();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};
#endif