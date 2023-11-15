#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <map>
#include <list>

#include "Player.hpp"
#include "Card.hpp"
#include "Token.hpp"
#include "NobleTile.hpp"

class GameScene;

class Game{

public:
    Game(GameScene* _scene);

    // ゲームの初期化・開始
    void initializeGame();
    std::vector<Card> loadDeckFromCSV(const std::string& filePath);
    std::vector<NobleTile> loadNoblesFromCSV(const std::string& filePath);
    void Play();
    void PlayTurn();

    void clickCards(int mouseX, int mouseY);

    // ゲームデータの表示
    void displayGameData();

    // 宝石トークンの取得に関する関数
    void takeTokens();
    void takeTokensOther(std::list<int> tokens);
    void takeTokensSame(int token);
    void returnExcessTokens(std::vector<int> tokens);

    // カードの購入に関する関数
    std::vector<Card> getPurchasableCards();
    void purchaseFieldCard(int index);
    void purchaseReservedCard(int index);

    // カードの確保に関する関数
    void reserveCard(int index);
    void replenishCard(int level);

    // 貴族タイルの取得に関する関数
    void takeNobleTile();
    
    bool isGameOver();
    Player& getCurrentPlayer();
    void nextTurn();
    std::vector<Card> getfieldCards();
    std::vector<NobleTile> getfieldTiles();
    std::map<Token::Color, int> getAvailableTokens();
    std::vector<Player> getPlayers();

private:
    GameScene* scene;
    std::vector<Player> players;
    int playerCount;
    
    std::vector<Card> deckLvl1; //山札のカード（各レベル）
    std::vector<Card> deckLvl2;
    std::vector<Card> deckLvl3;
    std::vector<NobleTile> nobles; //貴族タイル
    std::vector<Card> fieldCardsLvl1; //場に出ているカード（各レベル）
    std::vector<Card> fieldCardsLvl2;
    std::vector<Card> fieldCardsLvl3;
    std::vector<Card> fieldCards;
    std::vector<NobleTile> fieldTiles; //場に出ている貴族タイル

    std::map<Token::Color, int> availableTokens; //場にある宝石トークン
    int currentPlayerIndex;
    Player winner;
};
#endif