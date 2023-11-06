#include <cmath>
#include <iostream>

#include "Button.hpp"
#include "SDLHelper.hpp"

Button::Button(SDLHelper& helper, const std::string& text, int x, int y, int width, int height, SDL_Color defaultColor, SDL_Color textColor, int fontSize)
    : sdlHelper(helper), rect{x, y, width, height}, defaultColor(defaultColor), shape(Shape::RECTANGLE) {

    currentColor = defaultColor;
 
    // ボタンのテキストをテクスチャとして作成
    textTexture = sdlHelper.renderText(text, textColor, fontSize);
}

Button::Button(SDLHelper& helper, const std::string& text, int x, int y, int radius, SDL_Color defaultColor, SDL_Color textColor, int fontSize)
    : sdlHelper(helper), rect{x - radius, y - radius, radius * 2, radius * 2}, radius(radius), defaultColor(defaultColor), shape(Shape::CIRCLE) {

    // ボタンのテキストをテクスチャとして作成
    textTexture = sdlHelper.renderText(text, textColor, fontSize);
}

Button::Button(SDLHelper& helper) : sdlHelper(helper){
    rect = {0, 0, 0, 0};
    defaultColor = {0, 0, 0, 0};
}

void Button::setButton(const std::string& text, int x, int y, int width, int height, SDL_Color defColor, SDL_Color textColor, int fontSize){
    textTexture = sdlHelper.renderText(text, textColor, fontSize);
    rect = {x, y, width, height};
    currentColor = defColor;
    shape = Shape::RECTANGLE;
}

void Button::setInvisibleButton() {
    setButton("", 0, 0, 0, 0, {0 ,0, 0, 0}, {0 ,0, 0, 0}, 0);
}


void Button::render(){
    if(shape == Shape::RECTANGLE){
        sdlHelper.drawRect(rect.x, rect.y, rect.w, rect.h, currentColor, textTexture);
    }else if(shape == Shape::CIRCLE){
        sdlHelper.drawCircle(rect.x + radius, rect.y + radius, radius, currentColor, textTexture);
    }

    // sdlHelper.drawTextCenter(textTexture, rect);
}

bool Button::isMouseOver(int mouseX, int mouseY){
    if(shape == Shape::RECTANGLE){
        return (mouseX > rect.x && mouseX < rect.x + rect.w && mouseY > rect.y && mouseY < rect.y + rect.h);
    }else{
        int dx = mouseX - (rect.x + radius);
        int dy = mouseY - (rect.y + radius);
        return (std::pow(dx, 2) + std::pow(dy, 2) <= std::pow(radius, 2));
    }
}

void Button::setHover(){
    currentColor.a = static_cast<Uint8>(defaultColor.a * 0.7);
}

void Button::outHover(){
    currentColor.a = defaultColor.a;
}

void Button::isHover(int mouseX, int mouseY){
    if(isMouseOver(mouseX, mouseY)){
        setHover();
    }else{
        outHover();
    }
}

void Button::setPosition(int x, int y){
    rect.x = x;
    rect.y = y;
}

void Button::setSize(int width, int height){
    rect.w = width;
    rect.h = height;
}

void Button::setRadius(int r){
    radius = r;
    rect.x = rect.x - r;
    rect.y = rect.y - r;
    rect.w = r * 2;
    rect.h = r * 2;
}


RadioButton::RadioButton(SDLHelper& helper, const std::string& text, int x, int y, int width, int height, SDL_Color defColor, SDL_Color textColor, SDL_Color selectedColor, int fontSize)
    : Button(helper, text, x, y, width, height, defColor, textColor, fontSize), selectedColor(selectedColor), selected(false) {
        currentColor = defaultColor;
    }

// コンストラクタ（円形ボタンの場合）
RadioButton::RadioButton(SDLHelper& helper, const std::string& text, int x, int y, int radius, SDL_Color color, SDL_Color textColor, SDL_Color selectedColor, int fontSize)
    : Button(helper, text, x, y, radius, color, textColor, fontSize), selectedColor(selectedColor), selected(false) {}

void RadioButton::select(){
    selected = true;
    currentColor = selectedColor; 
}

void RadioButton::deselect(){
    selected = false;
    currentColor = defaultColor;
    // std::cout << "oh" << std::endl;
}

bool RadioButton::isSelected() const {
    return selected;
}

void RadioButton::render(){
    Button::render();
}