#ifndef SDLHelper_HPP
#define SDLHelper_HPP
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<string>
#include<map>

class SDLHelper{
public:
    SDLHelper(const std::string& windowTitle, int width, int Height);
    ~SDLHelper();

    // テクスチャのキャッシュ
    SDL_Texture* getCachedTexture(const std::string& text, SDL_Color color, int fontSize);
    void clearTextureCache();

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
    std::map<std::string, SDL_Texture*> textureCache;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
#endif