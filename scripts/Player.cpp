#include <iostream>
#include <sstream>
#include "Player.hpp"

#define COLOR Token::Color

Player::Player() : prestigePoints(0){
    //初期化処理
    for(const auto& color : Token::allColors){
        tokens[color] = 0;
    }
    tokens[Token::Color::GOLD] = 0;
}

int Player::getTokenCount(Token::Color color) const {
    return tokens.at(color);
}

int Player::getTotalTokenCount() const{
    int totalTokens = 0;

    for(const auto& color : Token::allColors){
        totalTokens += getTokenCount(color);
    }

    return totalTokens;
}

void Player::addTokenCount(Token::Color color, int count) {
    tokens.at(color) += count;
}

void Player::removeToken(Token::Color color, int count) {
    if(tokens.at(color) >= count){
        tokens.at(color) -= count;
    }else{
        // エラーハンドリング
        tokens.at(color) = 0;
    }
}

int Player::getCardTokenCount(Token::Color color)const {
    int count = 0;
    for(const Card& card : ownedCards){
        if(card.getCardColor() == color){
            count++;
        }
    }

    return count;
}

void Player::addCard(const Card& card){
    ownedCards.push_back(card);
    prestigePoints += card.getPrestige(); //発展カードの威信点を加算
    return;
}

bool Player::isPurchasable(const Card& card) const{
    int goldTokens = tokens.at(COLOR::GOLD);
    for(const auto& color : Token::colorsExceptGOLD){
        int requiredTokens = card.getCost().at(color);
        
        // 発展カードで補う
        int effectiveCardTokens = getCardTokenCount(color);
        requiredTokens -= effectiveCardTokens;

        int ownedTokens = tokens.at(color);
        // 通常の宝石トークンで補う
        requiredTokens -= ownedTokens;

        if(requiredTokens > 0){
            // 金トークンでの補完 
            goldTokens -= requiredTokens;
        }
    }
    if(goldTokens < 0){
        return false; // 足りない場合はカードを追加しない
    }
    return true;
}

void Player::reserveCard(const Card& card){
    reservedCards.push_back(card);
}

void Player::removeReserveCard(int index){
    reservedCards.erase(reservedCards.begin() + index);
}

std::vector<Card> Player::getReservedCards() const{
    return reservedCards;
}

bool Player::addNobleTile(const NobleTile& tile){
    if(canReceiveNobleTile(tile)){
        ownedTiles.push_back(tile);
        prestigePoints += 3; //貴族タイルの威信点を加算
        return true;
    }
    return false;
}

bool Player::canReceiveNobleTile(const NobleTile& tile) const{
    for(const auto& pair : tile.getRequiredCards()){
        Token::Color color = pair.first;
        int requiredCount = pair.second;
        if(getCardTokenCount(color) < requiredCount){//バグ：貴族タイル取得が複数回実行される
            return false;
        }
    }
    return true;
}

std::vector<NobleTile> Player::getOwnedNobleTiles() const{
    return ownedTiles;
}

int Player::getPrestigePoints() const{
    return prestigePoints;
}

std::string Player::playerToString() const{
    std::stringstream ss;
    ss << "威信点:" << prestigePoints << " " << "(宝石トークン,カード)";
    for(const auto& color : Token::colorsExceptGOLD){
        ss << Token::colorToString(color) << ":(" << getTokenCount(color) << "," << getCardTokenCount(color) << ") ";
    }
    ss << "GOLD:" << tokens.at(COLOR::GOLD) << " 確保カード枚数:" << reservedCards.size() << " 保有貴族タイル枚数:" << ownedTiles.size();

    std::string str = ss.str();
    return str;
}

void Player::setNumber(int num){
    number = num;
}

int Player::getNumber() const{
    return number;
}