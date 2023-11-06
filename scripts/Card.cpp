#include <sstream>

#include "Card.hpp"

Card::Card(int id, int lvl, int points, Token::Color color, const std::map<Token::Color, int>& cst) : id(id), level(lvl), prestigePoints(points), cardColor(color), cost(cst){}

int Card::getLevel() const{
    return level;
}

int Card::getPrestige() const{
    return prestigePoints;
}

Token::Color Card::getCardColor() const{
    return cardColor;
}

const std::map<Token::Color, int>& Card::getCost() const{
    return cost;
}

std::string Card::cardToString() const{
    std::stringstream ss;
    ss << "レベル:" << level << " 威信点:" << prestigePoints << " 色:" << Token::colorToString(cardColor) << " コスト...";
    for(const auto& color : Token::colorsExceptGOLD){
        if(cost.at(color) > 0){
            ss << Token::colorToString(color) << ":" << cost.at(color) << " ";
        }
    } 

    std::string str = ss.str();
    return str;
}