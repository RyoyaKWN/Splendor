#ifndef PLAYER_HPP
#define PLAYER_HPP
//* プレイヤーに関するデータ・関数
#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "Card.hpp"
#include "Token.hpp"
#include "NobleTile.hpp"

class Player {
public:
    Player();

    //宝石トークンの取得・設定
    int getTokenCount(Token::Color color) const;
    int getTotalTokenCount() const;
    void addTokenCount(Token::Color color, int count);
    void removeToken(Token::Color color, int count);

    //発展カードの取得・設定
    bool isPurchasable(const Card& card) const;
    void addCard(const Card& card);
    int getCardTokenCount(Token::Color color) const;
    void reserveCard(const Card& card);
    void removeReserveCard(int index);
    std::vector<Card> getReservedCards() const;

    //貴族タイルの取得・設定
    bool addNobleTile(const NobleTile& tile);
    bool canReceiveNobleTile(const NobleTile& tile) const;
    std::vector<NobleTile> getOwnedNobleTiles() const;

    //威信点の計算
    int getPrestigePoints() const;

    std::string playerToString() const;
    void setNumber(int num);
    int getNumber() const;

private:
    int number; //プレイヤー番号
    std::map<Token::Color, int> tokens; //保有する宝石トークン
    std::vector<Card> ownedCards; //保有する発展カード
    std::vector<Card> reservedCards; //確保している発展カード
    std::map<Token::Color, int> cardTokens; //購入に利用できる発展カードのトークン
    std::vector<NobleTile> ownedTiles; //保有する貴族タイル
    int prestigePoints; //威信点
};

#endif