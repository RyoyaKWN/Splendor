#include <sstream>

#include "NobleTile.hpp"

NobleTile::NobleTile(int id, const std::map<Token::Color, int>& requiredCards) : id(id), requiredCards(requiredCards){}

const std::map<Token::Color, int>& NobleTile::getRequiredCards() const{
    return requiredCards;
}