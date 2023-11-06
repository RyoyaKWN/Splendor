#ifndef NOBLETILE_HPP
#define NOBLETILE_HPP
//* 貴族タイルに関するデータ・関数

#include <map>

#include "Token.hpp"

class NobleTile{
public:
    NobleTile(int id, const std::map<Token::Color, int>& requiredCards);
    const std::map<Token::Color, int>& getRequiredCards() const;
    std::string tileToString() const;
    
    friend bool operator==(const NobleTile& lhs, const NobleTile& rhs) {
        // 例: id というメンバ変数を基に等価性を判定する場合
        return lhs.id == rhs.id;
        // 必要に応じて、他のメンバ変数も比較に利用できます
    }

private:
    int id;
    std::map<Token::Color, int> requiredCards;
};
#endif