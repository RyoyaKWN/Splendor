#ifndef TOKEN_HPP
#define TOKEN_HPP
//* トークンに関するデータ・関数

#include <SDL2\SDL.h>
#include <string>
#include <vector>

class Token{
public:
    enum Color{
        RED, GREEN, BLUE, WHITE, BLACK, GOLD
    };

    static const std::vector<Color> allColors;
    static const std::vector<Color> colorsExceptGOLD;

    static Token::Color stringToColor(const std::string& str);
    static std::string colorToString(const Token::Color& clr);
    static SDL_Color colorToSDLColor(const Token::Color& clr);
};
#endif