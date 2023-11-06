#include <iostream>

#include "Token.hpp"

const std::vector<Token::Color> Token::allColors = {
        Color::RED,
        Color::GREEN,
        Color::BLUE,
        Color::WHITE,
        Color::BLACK,
        Color::GOLD
};

const std::vector<Token::Color> Token::colorsExceptGOLD = {
        Color::RED,
        Color::GREEN,
        Color::BLUE,
        Color::WHITE,
        Color::BLACK
};

Token::Color Token::stringToColor(const std::string& str){
    if(str == "RED") return Color::RED;
    else if(str == "GREEN") return Color::GREEN;
    else if(str == "BLUE") return Color::BLUE;
    else if(str == "WHITE") return Color::WHITE;
    else if(str == "BLACK") return Color::BLACK;
    else if(str == "GOLD") return Color::GOLD;
    else{
        std::cerr << "カラーが定義されていません: " << str << std::endl;
        throw std::invalid_argument("Invalid color string");
    }
}

std::string Token::colorToString(const Token::Color& clr){
    if(clr == Token::Color::RED) return "RED";
    else if(clr == Token::Color::GREEN) return "GREEN";
    else if(clr == Token::Color::BLUE) return "BLUE";
    else if(clr == Token::Color::WHITE) return "WHITE";
    else if(clr == Token::Color::BLACK) return "BLACK";
    else if(clr == Token::Color::GOLD) return "GOLD";
    else return "";
}

SDL_Color Token::colorToSDLColor(const Token::Color& clr){
    if(clr == Token::Color::RED) return {255, 0, 0, 255};
    else if(clr == Token::Color::GREEN) return {0, 255, 0, 255};
    else if(clr == Token::Color::BLUE) return {0, 0, 255, 255};
    else if(clr == Token::Color::WHITE) return {255, 255, 255, 255};
    else if(clr == Token::Color::BLACK) return {0, 0, 0, 255};
    else if(clr == Token::Color::GOLD) return {255, 255, 0, 255};
    else return {0, 0, 0, 0};
}