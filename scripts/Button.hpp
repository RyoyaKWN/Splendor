#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class SDLHelper;

//* ボタン・ラジオボタンに関するクラス
class Button{
public:
    enum class Shape{
        RECTANGLE,
        CIRCLE
    };
    
    Button(SDLHelper& helper, const std::string& text, int x, int y, int width, int height, SDL_Color defaultColor, SDL_Color textColor, int fontSize);
    Button(SDLHelper& helper, const std::string& text, int x, int y, int radius, SDL_Color color, SDL_Color textColor, int fontSize);
    Button(SDLHelper& helper);

    void setButton(const std::string& text, int x, int y, int width, int height, SDL_Color defaultColor, SDL_Color textColor, int fontSize);
    void setInvisibleButton();

    // 描画
    virtual void render();

    // マウスホバーの処理
    bool isMouseOver(int mouseX, int mouseY);
    void setHover();
    void outHover();
    void isHover(int mouseX, int mouseY);

    // 位置・サイズの変更
    void setPosition(int x, int y);
    void setSize(int width, int height);
    void setRadius(int radius);

private:
    SDLHelper& sdlHelper;
    SDL_Rect rect;
    int radius;
    bool hovered = false;
    SDL_Texture* textTexture;
    Shape shape;

protected:
    SDL_Color currentColor;
    SDL_Color defaultColor;
};

class RadioButton : public Button{
public:
    RadioButton(SDLHelper& helper, const std::string& text, int x, int y, int width, int height, SDL_Color color, SDL_Color textColor, SDL_Color selectedColor, int fontSize);
    RadioButton(SDLHelper& helper, const std::string& text, int x, int y, int radius, SDL_Color color, SDL_Color textColor, SDL_Color selectedColor, int fontSize);

    void select();
    void deselect();
    bool isSelected() const;

    void render() override;

private:
    SDL_Color selectedColor;
    bool selected = false;
};
#endif