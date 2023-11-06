#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdlib>

#include "Game.hpp"
#include "Scene.hpp"

#define COLOR Token::Color

Game::Game(GameScene* _scene){
    scene = _scene;
    playerCount = scene->getPlayerCount();
    players.resize(playerCount);
    for(int i=1; i<=playerCount; i++){
        players[i-1].setNumber(i);
    }
    currentPlayerIndex = 0;
    initializeGame();
}

void Game::initializeGame(){
    // 発展カードの初期化
    deckLvl1 = loadDeckFromCSV("./csv/cardsLvl1.csv");
    deckLvl2 = loadDeckFromCSV("./csv/cardsLvl2.csv");
    deckLvl3 = loadDeckFromCSV("./csv/cardsLvl3.csv");

    // 貴族タイルの初期化
    nobles = loadNoblesFromCSV("./csv/nobles.csv");

    // 宝石トークンの初期化（2人:4, 3人:5, 4人:7）
    int initialTokens = (players.size() == 2) ? 4 : (players.size() == 3) ? 5 :7;
    for(const auto& color : Token::colorsExceptGOLD){
        availableTokens[color] = initialTokens;
    }
    availableTokens[COLOR::GOLD] = 5; //金トークンは常に5つ

    // 場の発展カード・貴族タイルの初期化
    for(int i=0; i<4; i++){
        int rndId = rand() % deckLvl3.size();
        fieldCards.push_back(deckLvl3[rndId]);
        deckLvl3.erase(deckLvl3.begin() + rndId);
    }
    for(int i=0; i<4; i++){
        int rndId = rand() % deckLvl2.size();
        fieldCards.push_back(deckLvl2[rndId]);
        deckLvl2.erase(deckLvl2.begin() + rndId);
    }
    for(int i=0; i<4; i++){
        int rndId = rand() % deckLvl1.size();
        fieldCards.push_back(deckLvl1[rndId]);
        deckLvl1.erase(deckLvl1.begin() + rndId);
    }

    for(int i=0; i<players.size()+1; i++){
        int rndId = rand() % nobles.size();
        fieldTiles.push_back(nobles[rndId]);
        nobles.erase(nobles.begin() + rndId);
    }
}

std::vector<Card> Game::loadDeckFromCSV(const std::string& filePath){
    std::vector<Card> cards;
    std::ifstream file(filePath);
    std::string line;

    if(!file.is_open()){
        std::cerr << "ファイルの読み込みに失敗しました: " << filePath << std::endl;
        return cards;
    }

    std::getline(file, line); //ヘッダを読み飛ばす
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string item;

        int id;
        int level;
        int points;
        Token::Color color;
        std::map<Token::Color, int> cost;

        std::getline(ss, item, ',');
        id = std::stoi(item);

        std::getline(ss, item, ',');
        level = std::stoi(item);

        std::getline(ss, item, ',');
        points = std::stoi(item);

        std::getline(ss, item, ',');
        color = Token::stringToColor(item);

        for(const auto& tokenColor : Token::colorsExceptGOLD){
            std::getline(ss, item, ',');
            cost[tokenColor] = std::stoi(item);
        }

        cards.emplace_back(id, level, points, color, cost);
    }

    file.close();
    return cards;
}

std::vector<NobleTile> Game::loadNoblesFromCSV(const std::string& filePath){
    std::vector<NobleTile> nobles;
    std::ifstream file(filePath);
    std::string line;

    if(!file.is_open()){
        std::cerr << "ファイルの読み込みに失敗しました: " << filePath << std::endl;
        return nobles;
    }

    std::getline(file, line); //ヘッダを読み飛ばす
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string item;

        int id;
        std::map<Token::Color, int> requiredCards;


        std::getline(ss, item, ',');
        id = std::stoi(item);

        for(const auto& color : Token::colorsExceptGOLD){
            std::getline(ss, item, ',');
            requiredCards[color] = std::stoi(item);
        }

        nobles.emplace_back(id, requiredCards);
    }

    file.close();
    return nobles;
}

void Game::takeTokensOther(std::list<int> tokens){
    Player& currentPlayer = getCurrentPlayer();
    for(int& token : tokens){
        COLOR color = static_cast<COLOR>(token);
        currentPlayer.addTokenCount(color, 1);
        availableTokens[color]--;
    }
}

void Game::takeTokensSame(int token){
    Player& currentPlayer = getCurrentPlayer();
    COLOR color = static_cast<COLOR>(token);
    currentPlayer.addTokenCount(color, 2);
    availableTokens[color] -= 2;
}

void Game::returnExcessTokens(std::vector<int> tokens){
    Player& currentPlayer = getCurrentPlayer();
    for(int i=0; i<5; i++){
        Token::Color tokenColor = static_cast<Token::Color>(i);
        currentPlayer.removeToken(tokenColor, tokens[i]);
        availableTokens[tokenColor] += tokens[i];
    }
}

void Game::purchaseFieldCard(int index){
    Player& currentPlayer = getCurrentPlayer();
    Card& card = fieldCards[index];

    for(const auto& color : Token::colorsExceptGOLD){
        int requiredTokens = card.getCost().at(color);
        
        // 発展カードで足りる分を補う
        int effectiveCardTokens = currentPlayer.getCardTokenCount(color);
        requiredTokens -= effectiveCardTokens;
        if(requiredTokens <= 0){
            continue;
        }

        // 通常の宝石トークンで足りる分を補う
        int ownedTokens = currentPlayer.getTokenCount(color);
        if(requiredTokens <= ownedTokens){
            currentPlayer.removeToken(color, requiredTokens);
            availableTokens[color] += requiredTokens;
        } else {
            currentPlayer.removeToken(color, ownedTokens);
            availableTokens[color] += ownedTokens;
            requiredTokens -= ownedTokens;
            // 金トークンでの補完
            int goldTokens = currentPlayer.getTokenCount(COLOR::GOLD);
            currentPlayer.removeToken(COLOR::GOLD, requiredTokens);
            availableTokens[COLOR::GOLD] += requiredTokens;
        }
    }
    currentPlayer.addCard(card);
    // カードの消去・補充
    replenishCard(index);

    takeNobleTile();
}

void Game::purchaseReservedCard(int index){
    Player& currentPlayer = getCurrentPlayer();
    Card& card = currentPlayer.getReservedCards()[index];

    for(const auto& color : Token::colorsExceptGOLD){
        int requiredTokens = card.getCost().at(color);
        
        // 発展カードで足りる分を補う
        int effectiveCardTokens = currentPlayer.getCardTokenCount(color);
        requiredTokens -= effectiveCardTokens;
        if(requiredTokens <= 0){
            continue;
        }

        // 通常の宝石トークンで足りる分を補う
        int ownedTokens = currentPlayer.getTokenCount(color);
        if(requiredTokens <= ownedTokens){
            currentPlayer.removeToken(color, requiredTokens);
            availableTokens[color] += requiredTokens;
        } else {
            currentPlayer.removeToken(color, ownedTokens);
            availableTokens[color] += ownedTokens;
            requiredTokens -= ownedTokens;
            // 金トークンでの補完
            int goldTokens = currentPlayer.getTokenCount(COLOR::GOLD);
            currentPlayer.removeToken(COLOR::GOLD, requiredTokens);
            availableTokens[COLOR::GOLD] += requiredTokens;
        }
    }
    currentPlayer.addCard(card);
    // カードの消去・補充
    currentPlayer.removeReserveCard(index);

    takeNobleTile();
}

void Game::reserveCard(int index){
    Player& currentPlayer = getCurrentPlayer();
    currentPlayer.reserveCard(fieldCards[index]);
    //金トークンの取得
    if(availableTokens[COLOR::GOLD] > 1 && currentPlayer.getTotalTokenCount() < 10){
        currentPlayer.addTokenCount(COLOR::GOLD, 1);
        availableTokens[COLOR::GOLD]--;
    }
    replenishCard(index);
    // nextTurn();
}

void Game::replenishCard(int index){
    int rnd;

    if(index >= 0 && index < 4){
        rnd = rand() % deckLvl3.size();
        fieldCards[index] = deckLvl3[rnd];
        deckLvl3.erase(deckLvl3.begin() + rnd);
    }else if(index >= 4 && index < 8){
        rnd = rand() % deckLvl2.size();
        fieldCards[index] = deckLvl2[rnd];
        deckLvl2.erase(deckLvl2.begin() + rnd);
    }else if(index >= 8 && index < 12){
        rnd = rand() % deckLvl1.size();
        fieldCards[index] = deckLvl1[rnd];
        deckLvl1.erase(deckLvl3.begin() + rnd);
    }
}

void Game::takeNobleTile(){
    Player& currentPlayer = getCurrentPlayer();
    for(const auto& noble : fieldTiles){
            if(currentPlayer.canReceiveNobleTile(noble)){
                currentPlayer.addNobleTile(noble);
                // std::cout << "Player" << currentPlayerIndex+1 << "は貴族タイルを獲得しました！" << " 威信点+3" << std::endl;
                fieldTiles.erase(std::remove(fieldTiles.begin(), fieldTiles.end(), noble), fieldTiles.end());
            }
    }

}

bool Game::isGameOver(){
    if(players[currentPlayerIndex].getPrestigePoints() >= 15){
        winner = players[currentPlayerIndex];
        return true;
    }
    return false;
}

Player& Game::getCurrentPlayer(){
    return players[currentPlayerIndex];
}

void Game::nextTurn(){
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

std::vector<Card> Game::getfieldCards(){
    return fieldCards;
}

std::vector<NobleTile> Game::getfieldTiles(){
    return fieldTiles;
}

std::map<Token::Color, int> Game::getAvailableTokens(){
    return availableTokens;
}

std::vector<Player> Game::getPlayers(){
    return players;
}