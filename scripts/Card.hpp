#ifndef CARD_HPP
#define CARD_HPP
//* カードに関するデータ・関数

#include "Token.hpp"
#include <map>

class Card{
public:
    Card(int id, int level, int prestigePoints, Token::Color cardColor, const std::map<Token::Color, int>& cost);
    int getLevel() const;
    int getPrestige() const;
    Token::Color getCardColor() const;
    const std::map<Token::Color, int>& getCost() const;
    std::string cardToString() const;

    friend bool operator==(const Card& lhs, const Card& rhs) {
        // 例: id というメンバ変数を基に等価性を判定する場合
        return lhs.id == rhs.id;
        // 必要に応じて、他のメンバ変数も比較に利用できます
    }

private:
    int id;
    int level;
    int prestigePoints;
    Token::Color cardColor;
    std::map<Token::Color, int> cost;
};

#endif