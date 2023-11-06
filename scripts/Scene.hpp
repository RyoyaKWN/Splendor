#ifndef SCENE_HPP
#define SCENE_HPP

// #pragma once

#include <SDL2/SDL.h>
#include "SDLHelper.hpp"
#include "Button.hpp"
// #include "Game.hpp"
#include "Player.hpp"
#include "Card.hpp"
#include "NobleTile.hpp"
#include "Token.hpp"
#include <vector>
#include <list>


class Scene{
public:
    virtual ~Scene() = default;

    //更新
    virtual void update(const SDL_Event& e) = 0;

    //描画
    virtual void render(SDL_Renderer* renderer) = 0;

    //変更されたかを返す
    virtual bool isSceneChange() = 0;

    //次のシーンを取得
    virtual Scene* getNextScene() = 0;

};

class TitleScene : public Scene{
public:
    TitleScene(SDLHelper& sdlHelper);
    ~TitleScene();

    void update(const SDL_Event& e) override;
    void render(SDL_Renderer* renderer) override;
    bool isSceneChange() override;
    Scene* getNextScene() override;

private:
    SDLHelper& helper;
    std::vector<RadioButton*> radioButtons;
    Button* startButton;
    int selectedPlayerCount;
    bool changeScene;
    SDL_Texture* titleTexture;
    SDL_Texture* subtitleTexture;
};


class Game;

class GameScene : public Scene{
public:
    GameScene(SDLHelper& helper, int numPlayers);
    ~GameScene();

    void update(const SDL_Event& e) override;
    void render(SDL_Renderer* renderer) override;
    bool isSceneChange() override;
    Scene* getNextScene() override;
    int getPlayerCount() const;
    void nextTurn();

    // 行動の処理
    bool clickCards(int mouseX, int mouseY);
    bool clickToken(int mouseX, int mouseY);
    bool clickReservedCards(int mouseX, int mouseY);
    void clickActionButtons(int mouseX, int mouseY);
    void clickExcessToken(int mouseX, int mouseY);

    // カード・貴族タイルの描画
    void renderCard(const Card& card, int x, int y);
    void renderNobleTile(const NobleTile& tile, int x, int y);

    // プレイヤー情報の描画
    void renderMessageLog();
    void renderPlayerInfo(const Player& player, int y);
    void rcpReservedCards(const Player& plsayer);
    void renderAllPlyerInfo(int numPlayers);

    // フィールドの描画
    void renderFieldCards();
    void renderFieldNobleTiles();
    void renderAvailableTokens();

    // 行動選択ボタンの描画
    void renderActionButtons();
    void fieldCardAction();
    void tokenAction();
    void reservedCardAction();
    void invisibleActionButton();
    void returnExcessToken();

    // ゲーム終了
    void gameOver(int winner);

private:
    SDLHelper& helper;
    Game* game;
    Player currentPlayer;
    std::string message;
    bool actionBind = false;
    bool isGameOver = false;
    SDL_Texture* result;
    
    std::vector<Card> fieldCards;
    std::vector<Token::Color> availableTokens;
    std::vector<Card> reservedCards;
    std::vector<RadioButton*> cardsRadio;
    std::vector<RadioButton*> tokenRadio;
    std::vector<RadioButton*> reservedCardsRadio;
    
    int selectCardIndex = -1;
    std::list<int> selectToken = {};
    int selectReservedCardIndex = -1;
    std::vector<int> selectExcessToken = {0, 0, 0, 0, 0};

    std::pair<Button*, bool> actionButton1;
    std::pair<Button*, bool> actionButton2;
    std::pair<Button*, bool> returnButton;
    Button* cancelButton;
    std::vector<Button*> excessTokenRadio;

    bool changeScene;

    //プレイヤー情報
    std::vector<Player*> players;
    int playerCount = 0;
};
#endif