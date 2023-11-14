#ifndef CARD_HPP
#define CARD_HPP

#include "Token.hpp"
#include <map>

//* カードに関するデータ・関数
class Card{
public:
    Card(int id, int level, int prestigePoints, Token::Color cardColor, const std::map<Token::Color, int>& cost);
    int getLevel() const;
    int getPrestige() const;
    Token::Color getCardColor() const;
    const std::map<Token::Color, int>& getCost() const;
    std::string cardToString() const;

    friend bool operator==(const Card& lhs, const Card& rhs) {
        return lhs.id == rhs.id;
    }

private:
    int id;
    int level;
    int prestigePoints;
    Token::Color cardColor;
    std::map<Token::Color, int> cost;
};

#endif