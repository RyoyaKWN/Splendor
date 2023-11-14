#ifndef NOBLETILE_HPP
#define NOBLETILE_HPP

#include <map>
#include "Token.hpp"

//* 貴族タイルに関するデータ・関数
class NobleTile{
public:
    NobleTile(int id, const std::map<Token::Color, int>& requiredCards);
    const std::map<Token::Color, int>& getRequiredCards() const;
    std::string tileToString() const;
    
    friend bool operator==(const NobleTile& lhs, const NobleTile& rhs) {
        return lhs.id == rhs.id;
    }

private:
    int id;
    std::map<Token::Color, int> requiredCards;
};
#endif