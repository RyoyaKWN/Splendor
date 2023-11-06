#include <sstream>

#include "NobleTile.hpp"

NobleTile::NobleTile(int id, const std::map<Token::Color, int>& requiredCards) : id(id), requiredCards(requiredCards){}

const std::map<Token::Color, int>& NobleTile::getRequiredCards() const{
    return requiredCards;
}

std::string NobleTile::tileToString() const{
    std::stringstream ss;
    ss << "威信点:3 コスト...";
    for(const auto& color : Token::colorsExceptGOLD){
        if(requiredCards.at(color) > 0){
            ss << Token::colorToString(color) << ":" << requiredCards.at(color) << " ";
        }
    }

    std::string str = ss.str();
    return str;
}