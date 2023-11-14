#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL2/SDL.h>
#include "SDLHelper.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "Card.hpp"
#include "NobleTile.hpp"
#include "Token.hpp"
#include <vector>
#include <list>

//* シーンの描画に関する関数
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
    std::vector<RadioButton*> radioButtons; //人数選択ボタン
    Button* startButton; //ゲームスタートボタン
    int selectedPlayerCount;
    bool changeScene;
    SDL_Texture* titleTexture; //"Splendor"
    SDL_Texture* subtitleTexture; //"宝石の煌めき"
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
    Player currentPlayer; //行動中のプレイヤー
    std::string message; //左下に表示するメッセージ
    bool actionBind = false; //特定の領域以外の操作を受け付けない
    bool isGameOver = false; //ゲーム終了判定
    SDL_Texture* result; //"プレイヤーXの勝利！"
    
    std::vector<Card> fieldCards;
    std::vector<RadioButton*> cardsRadio; //場のカード
    std::vector<Token::Color> availableTokens;
    std::vector<RadioButton*> tokenRadio; //場の宝石トークン
    std::vector<Card> reservedCards;
    std::vector<RadioButton*> reservedCardsRadio; //予約カード
    
    int selectCardIndex = -1; //選択中のカード
    std::list<int> selectToken = {}; //選択中の宝石トークン
    int selectReservedCardIndex = -1; //選択中の予約カード
    std::vector<int> selectExcessToken = {0, 0, 0, 0, 0}; //選択中の宝石トークン(返却時)

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