#include <iostream>
#include <algorithm>

#include "Scene.hpp"
#include "Game.hpp"

SDL_Color RED = {255, 0, 0, 255};
SDL_Color GREEN = {0, 255, 0, 255};
SDL_Color BLUE = {0, 0, 255, 255};
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Color BLACK = {0, 0, 0, 255};
SDL_Color GOLD = {255, 255, 0, 255};
SDL_Color HIGHLIGHT = {255, 0, 200, 255};
SDL_Color TRANSPARENT = {0, 0, 0, 0};
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

TitleScene::TitleScene(SDLHelper& sdlHelper) : helper(sdlHelper), changeScene(false), selectedPlayerCount(2){
    //タイトル
    titleTexture = helper.renderText("Splendor", GOLD, 100);
    subtitleTexture = helper.renderText("宝石の煌めき", GOLD, 50);
    
    // プレイヤー選択のボタンサイズ
    const int buttonWidth = 150;
    const int buttonHeight = 40;
    const int spacing = 30;

    // プレイヤー数選択するボタン
    for(int i = 0; i < 3; i++){
        std::string text = std::to_string(i + 2) + "人プレイ";
        radioButtons.push_back(new RadioButton(helper, text, 
                                        ( SCREEN_WIDTH - 3 * buttonWidth - 2 * spacing ) / 2 + ( buttonWidth + spacing ) * i, 
                                        300, buttonWidth, buttonHeight,
                                        WHITE, GREEN, GOLD, 24));
    }
    radioButtons[0]->select();

    // ゲームスタートボタン
    startButton = new Button(helper, "ゲームスタート",
                             (SCREEN_WIDTH - 250) / 2, 400, 250, 70,
                             GREEN, WHITE, 30);
}

TitleScene::~TitleScene(){
    if(titleTexture != NULL){
        SDL_DestroyTexture(titleTexture);
        titleTexture = NULL;
    }

    if(subtitleTexture != NULL){
        SDL_DestroyTexture(subtitleTexture);
        subtitleTexture = NULL;
    }

    for(Button* button : radioButtons){
        if(button != NULL){
            delete button;
            button = NULL;
        }
    }

    if(startButton != NULL){
        delete startButton;
        startButton = NULL;
    }
}

void TitleScene::update(const SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        for(int i=0; i< radioButtons.size(); i++){
            if(radioButtons[i]->isMouseOver(mouseX, mouseY)){
                //すべてのラジオボタンの選択を解除
                for(auto& rb : radioButtons){
                    rb->deselect();
                }
                radioButtons[i]->select(); //クリックしたボタンを選択状態にする
                selectedPlayerCount = 2 + i;
            }
        }
        if(startButton->isMouseOver(mouseX, mouseY)){
            changeScene = true;
        }
    }else if(e.type == SDL_MOUSEMOTION){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        for(Button* button : radioButtons){
            button->isHover(mouseX, mouseY);
        }
        startButton->isHover(mouseX, mouseY);
    }
}

void TitleScene::render(SDL_Renderer* renderer){
    // タイトル・サブタイトルの描画
    int titleWidth, titleHeight, subtitleWidth, subtitleHeight;
    SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
    SDL_QueryTexture(subtitleTexture, NULL, NULL, &subtitleWidth, &subtitleHeight);
    helper.drawText(titleTexture, (SCREEN_WIDTH - titleWidth) / 2, 50);
    helper.drawText(subtitleTexture, (SCREEN_WIDTH - subtitleWidth) / 2, 200);

    for(Button* button : radioButtons){
        button->render();
    }
    startButton->render();
}

bool TitleScene::isSceneChange(){
    return changeScene;
}

Scene* TitleScene::getNextScene(){
    return new GameScene(helper, selectedPlayerCount);
}

//*=====================================TitleScene========================================================*//
//*=======================================================================================================*//
//*=======================================================================================================*//
//*=======================================================================================================*//
//*=======================================================================================================*//
//*=====================================GameScene=========================================================*//

GameScene::GameScene(SDLHelper& sdlHelper, int playerCount) : helper(sdlHelper), playerCount(playerCount), changeScene(false){
    game = new Game(this);
    currentPlayer = game->getCurrentPlayer();

    for(int i=0; i<game->getfieldCards().size(); i++){
        fieldCards.push_back(game->getfieldCards()[i]);
        cardsRadio.push_back(new RadioButton(helper, "", 92 + 70 * (i % 4), 27 + 120 * (i / 4), 66, 96, {100, 100, 100, 255}, TRANSPARENT, HIGHLIGHT, 0));
    }
    for(int i=0; i<5; i++){
        tokenRadio.push_back(new RadioButton(helper, "", 405, 50 + 60 * i, 23, 
                                             TRANSPARENT, TRANSPARENT, HIGHLIGHT, 0));
    }

    actionButton1 = std::make_pair(new Button(helper), false);
    actionButton2 = std::make_pair(new Button(helper), false);
    cancelButton = new Button(helper);
    returnButton = std::make_pair(new Button(helper), false);

    for(int i=0; i<5; i++){
        excessTokenRadio.push_back(new Button(helper, "", 405 + 60 * (i % 3),
                                                   415 + 45 * (i / 3), 20,
                                                   TRANSPARENT, TRANSPARENT, 0));
    }

    message = "プレイヤー1のターン";
    result = NULL;
}

GameScene::~GameScene(){
    // TODO:
    delete game;
}

void GameScene::update(const SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if(!actionBind && mouseX >= 95 && mouseY >=27 && mouseX <= 365 && mouseY <= 393){
            if(clickCards(mouseX, mouseY)){
                fieldCardAction();
            }
        }else if(!actionBind && mouseX >= 385 && mouseY >= 27 && mouseX <= 425 && mouseY <= 313){
            if(clickToken(mouseX, mouseY)){
                tokenAction();
            }
        }else if(!actionBind && mouseX >= 461 && mouseY >= 87 && mouseY <= 527 && mouseY <= 382){
            if(clickReservedCards(mouseX, mouseY)){
                reservedCardAction();
            }
        }else if(mouseX >= 400 && mouseY >= 410 && mouseX <= 530 && mouseY <= 520){
            (actionBind && !isGameOver) ?  clickExcessToken(mouseX, mouseY) : clickActionButtons(mouseX, mouseY);
        }else{
            return;
        }
    }
}

bool GameScene::clickCards(int mouseX, int mouseY){
    bool onMap = false;
    for(int i=0; i < cardsRadio.size(); i++){
        if(cardsRadio[i]->isMouseOver(mouseX, mouseY)){
            if(!cardsRadio[i]->isSelected()){
                for(auto& cr : cardsRadio){
                    cr->deselect();
                }
                cardsRadio[i]->select();
                selectCardIndex = i;
                onMap = true;
                break;
            }else{
                cardsRadio[i]->deselect();
                selectCardIndex = -1;
                break;
            }
        }
    }

    // トークン、予約カードをdeselect
    if(onMap){
        if(!selectToken.empty()){
            for(auto& sT : selectToken){
                tokenRadio[sT]->deselect();
            }
            selectToken.clear();
        }else if(selectReservedCardIndex != -1){
            reservedCardsRadio[selectReservedCardIndex]->deselect();
            selectReservedCardIndex = -1;
        }
    }
    return onMap;
}

bool GameScene::clickToken(int mouseX, int mouseY){
    bool onMap = false;
    for(int i=0; i< tokenRadio.size(); i++){
        if(tokenRadio[i]->isMouseOver(mouseX, mouseY) && !tokenRadio[i]->isSelected() && selectToken.size() < 3){
            tokenRadio[i]->select(); //クリックしたボタンを選択状態にする
            selectToken.push_back(i);
            onMap = true;
            break;
        }else if(tokenRadio[i]->isMouseOver(mouseX, mouseY) && tokenRadio[i]->isSelected()){
            tokenRadio[i]->deselect();
            selectToken.remove(i);
            break;
        }
    }

    // カード、予約カードをdeselect
    if(onMap){
        if(selectCardIndex != -1){
            cardsRadio[selectCardIndex]->deselect();
            selectCardIndex = -1;
        }else if(onMap && selectReservedCardIndex != -1){
            reservedCardsRadio[selectReservedCardIndex]->deselect();
            selectReservedCardIndex = -1;
        }
    }
    return onMap;
}

bool GameScene::clickReservedCards(int mouseX, int mouseY){
    bool onMap = false;
    for(int i=0; i < reservedCardsRadio.size(); i++){
        if(reservedCardsRadio[i]->isMouseOver(mouseX, mouseY) && !reservedCardsRadio[i]->isSelected()){
            for(auto& rcr : reservedCardsRadio){
                rcr->deselect();
            }
            reservedCardsRadio[i]->select();
            selectReservedCardIndex = i;
            onMap = true;
            break;
        }else if(reservedCardsRadio[i]->isMouseOver(mouseX, mouseY) && reservedCardsRadio[i]->isSelected()){
            reservedCardsRadio[i]->deselect();
            selectReservedCardIndex = -1;
            return false;
            break;
        }
    }

    // カード、トークンをdeselect
    if(onMap){
        if(selectCardIndex != -1){
            cardsRadio[selectCardIndex]->deselect();
            selectCardIndex = -1;
        }else if(!selectToken.empty()){
            for(int& sT : selectToken){
                tokenRadio[sT]->deselect();
            }
            selectToken.clear();
        }
    }
    return onMap;
}

void GameScene::clickActionButtons(int mouseX, int mouseY){
    if(actionButton1.first->isMouseOver(mouseX, mouseY) && actionButton1.second){
        // アクションボタン１を押したときの処理
        if(selectCardIndex != -1){
            game->purchaseFieldCard(selectCardIndex);
            cardsRadio[selectCardIndex]->deselect();
            selectCardIndex = -1;
            invisibleActionButton();
            if(game->isGameOver()){
                gameOver(game->getCurrentPlayer().getNumber()-1);
            }else{
                nextTurn();
            }
        }else if(!selectToken.empty()){
            game->takeTokensOther(selectToken);
            for(int& sT : selectToken){
                tokenRadio[sT]->deselect();
            }
            selectToken.clear();
            invisibleActionButton();
            if(game->getCurrentPlayer().getTotalTokenCount() > 10){
                actionBind = true;
                message = "宝石は11枚以上持てません";
            }else{
                nextTurn();
            }
        }
    }else if(actionButton2.first->isMouseOver(mouseX, mouseY) && actionButton2.second){
        // アクションボタン２を押したときの処理
        if(selectCardIndex != -1){
            game->reserveCard(selectCardIndex);
            cardsRadio[selectCardIndex]->deselect();
            selectCardIndex = -1;
            invisibleActionButton();
            nextTurn();
        }else if(!selectToken.empty()){
            game->takeTokensSame(selectToken.back());
            tokenRadio[selectToken.back()]->deselect();
            selectToken.clear();
            invisibleActionButton();
            reservedCardsRadio.clear();
            reservedCards = game->getCurrentPlayer().getReservedCards();
            for(int i=0; i<reservedCards.size(); i++){
                reservedCardsRadio.push_back(new RadioButton(helper, "", 461, 87 + 100 * i, 66, 96,
                                                  TRANSPARENT, TRANSPARENT, HIGHLIGHT, 0));
            }

            if(game->getCurrentPlayer().getTotalTokenCount() > 10){
                actionBind = true;
                message = "宝石は11枚以上持てません";
            }else{
                nextTurn();
            }
        }else if(selectReservedCardIndex != -1){
            game->purchaseReservedCard(selectReservedCardIndex);
            reservedCardsRadio[selectReservedCardIndex]->deselect();
            selectReservedCardIndex = -1;
            invisibleActionButton();

            if(game->isGameOver()){
                gameOver(game->getCurrentPlayer().getNumber()-1);
            }else{
                nextTurn();
            }
        }
    }else if(cancelButton->isMouseOver(mouseX, mouseY)){
        // キャンセルボタンを押したときの処理
        if(selectCardIndex != -1){
            cardsRadio[selectCardIndex]->deselect();
            selectCardIndex = -1;
        }else if(!selectToken.empty()){
            for(auto sT : selectToken){
                tokenRadio[sT]->deselect();
            }
            selectToken.clear();
        }else if(selectReservedCardIndex != -1){
            reservedCardsRadio[selectReservedCardIndex]->deselect();
            selectReservedCardIndex = -1;
        }
        invisibleActionButton();
    }
}

void GameScene::clickExcessToken(int mouseX, int mouseY){
    bool onMap = false;
    if(cancelButton->isMouseOver(mouseX, mouseY)){
        for(int& token : selectExcessToken){
            token = 0;
        }
        return;
    }else if(returnButton.first->isMouseOver(mouseX, mouseY) && returnButton.second){
        game->returnExcessTokens(selectExcessToken);
        for(int i=0; i<5; i++){
            selectExcessToken[i] = 0;
            // excessTokenRadio[i]->setInvisibleButton();
        }
        returnButton.first->setInvisibleButton();
        cancelButton->setInvisibleButton();
        actionBind = false;
        nextTurn();
        return;
    }
    for(int i=0; i<5; i++){        
        if(excessTokenRadio[i]->isMouseOver(mouseX, mouseY) &&
           game->getCurrentPlayer().getTokenCount(static_cast<Token::Color>(i)) > selectExcessToken[i]){
            selectExcessToken[i]++;
            break;
        }
    }

}

void GameScene::render(SDL_Renderer* renderer){
    currentPlayer = game->getCurrentPlayer();

    // メッセージログを表示
    renderMessageLog();

    // 場のカードを表示
    renderFieldCards();

    // 場の貴族タイルを表示
    renderFieldNobleTiles();

    // 場の宝石トークンを表示
    renderAvailableTokens();

    // プレイヤー情報を表示
    renderAllPlyerInfo(playerCount);

    // 現在のターンのプレイヤーの予約カードを表示
    rcpReservedCards(currentPlayer);

    // 行動選択ボタンを表示
    renderActionButtons();

    // 取得トークン超過時、返却ボタンを表示
    if(actionBind && !isGameOver) returnExcessToken();

    if(isGameOver) helper.drawText(result, 0, 200);
}

int GameScene::getPlayerCount() const{
    return playerCount;
}

void GameScene::nextTurn(){
    game->nextTurn();
    reservedCardsRadio.clear();
    reservedCards = game->getCurrentPlayer().getReservedCards();
    for(int i=0; i<reservedCards.size(); i++){
        reservedCardsRadio.push_back(new RadioButton(helper, "", 461, 87 + 100 * i, 66, 96,
                                                    TRANSPARENT, TRANSPARENT, HIGHLIGHT, 0));
    }
    message = "プレイヤー" + std::to_string(game->getCurrentPlayer().getNumber()) + "のターン";
}

bool GameScene::isSceneChange(){
    return false;
}

Scene* GameScene::getNextScene(){
    return new TitleScene(helper);
}

void GameScene::renderMessageLog() { // メッセージログを描画
    helper.drawText(message, WHITE, 25, 30, 485);
}

void GameScene::renderCard(const Card& card, int x, int y) {
    // カードの幅・高さ
    int cardWidth = 60;
    int cardHeight = 90;

    // カードカラー・コストの領域
    SDL_Rect colorRect = {x, y, cardWidth, cardHeight * 3 / 10};
    SDL_Rect costRect = {x, y + cardHeight * 3 / 10, cardWidth, cardHeight * 7 / 10};

    
    // カードカラー・威信点の描画
    std::string pointsStr = std::to_string(card.getPrestige());
    SDL_Color cardColor = Token::colorToSDLColor(card.getCardColor());
    SDL_Color pointColor = (card.getCardColor() != Token::WHITE) ? WHITE : BLACK;
    SDL_Texture* pointTexture = helper.renderText(pointsStr, pointColor, 25);
    helper.drawRect(x, y, cardWidth, cardHeight * 3 / 10, cardColor, pointTexture);
    helper.drawRect(x, y + cardHeight * 3 / 10, cardWidth, cardHeight * 7 / 10, {200, 200, 200, 255}, NULL);

    // トークンのコストの描画
    int index = 0; // 描画するトークンが何色目か
    int tokenRadius =  12;
    int tokenMargin = 15;
    int space = 5;
    
    for(const auto& [color, count] : card.getCost()){
        if(count > 0){
            SDL_Color costColor = Token::colorToSDLColor(color);
            std::string costCountStr = std::to_string(card.getCost().at(color));
            SDL_Color costCountColor = (color != Token::WHITE) ? WHITE : BLACK;
            SDL_Texture* costCountTexture = helper.renderText(costCountStr, costCountColor, 20);
            int startX = x + tokenMargin + ( space + 2 * tokenRadius) * ( index / 2 );
            int startY = y + cardHeight - tokenMargin - ( space + 2 * tokenRadius ) * ( index % 2 );
            helper.drawCircle(startX, startY, tokenRadius, costColor, costCountTexture);
            index++;
        }
    }
}

void GameScene::renderFieldCards() { // 場に出ているカードの情報を描画
    helper.drawRect(14, 27, 66, 96, {0, 0, 200, 200}, NULL);
    helper.drawRect(14, 147, 66, 96, {200, 200, 0, 200}, NULL);
    helper.drawRect(14, 267, 66, 96, {0, 200, 0, 200}, NULL);

    for(int i=3; i>=1; i--){
        std::string levelText = "レベル" + std::to_string(i);
        helper.drawText(levelText, WHITE, 17, 15, 305 - 120 * (i-1));

        fieldCards = game->getfieldCards();
        for(int j=0; j<4; j++){
            int index = (3 - i) * 4 + j;
            cardsRadio[index]->render();
            renderCard(fieldCards[index], 95 + 70 * j, 390 - 120 * i);
        }
    }
}

void GameScene::rcpReservedCards(const Player& player) { // 予約しているカードの情報を描画
    reservedCards = player.getReservedCards();

    // 枠の描画
    helper.drawRect(444, 30, 100, 360, {150, 150, 150, 255}, NULL);

    // 現在のターンのプレイヤー、文字を描画
    helper.drawText("予約カード", WHITE, 18, 449, 30);
    std::string currentPlayerText = "プレイヤー" + std::to_string(currentPlayer.getNumber());
    helper.drawText(currentPlayerText, WHITE, 16, 449, 55);

    // 予約カードの描画
    for(int i=0; i<reservedCards.size(); i++){
        reservedCardsRadio[i]->render();
        renderCard(reservedCards[i], 464, 90 + 100 * i);
    }
}

void GameScene::renderNobleTile(const NobleTile& tile, int x, int y) {
    // タイルの幅・高さ
    int tileWidth = 60;
    int tileHeight = 90;
    SDL_Color tileColor = {200, 200, 200, 255};
    SDL_Rect tileRect = {x, y + tileHeight, tileWidth, tileHeight};

    // タイルの描画
    helper.drawRect(x, y, tileWidth, tileHeight, tileColor, NULL);
    
    // 威信点の描画
    std::string pointsStr = "3";
    helper.drawText(pointsStr, BLACK, 25, x + 5, y);

    // トークンのコストの描画
    int index = 0; // 描画するトークンが何色目か
    int cardSide =  20;
    int cardMargin = 5;
    int space = 5;
    
    for(const auto& [color, count] : tile.getRequiredCards()){
        if(count > 0){
            SDL_Color costColor = Token::colorToSDLColor(color);
            std::string costCountStr = std::to_string(tile.getRequiredCards().at(color));
            SDL_Color costCountColor = (color != Token::WHITE) ? WHITE : BLACK;
            SDL_Texture* costCountTexture = helper.renderText(costCountStr, costCountColor, 20);
            int startX = x + cardMargin + ( space + cardSide) * ( index / 2 );
            int startY = y + tileHeight - cardSide - cardMargin - ( space + cardSide ) * ( index % 2 );
            helper.drawRect(startX, startY, cardSide, cardSide, costColor, costCountTexture);
            index++;
        }
    }
}

void GameScene::renderFieldNobleTiles() { // 場に出ている貴族タイルの情報を描画
    std::vector<NobleTile> tiles = game->getfieldTiles();
    for(int i=0; i<tiles.size(); i++){
        renderNobleTile(tiles[i], 305 - 70 * i, 380);
    }
}

void GameScene::renderPlayerInfo(const Player& player, int startY){ // プレイヤーのトークン数・威信点・予約カード数・所持タイル数を描画
    int startX = SCREEN_WIDTH - 400;
    SDL_Rect rect = {startX, startY, 400, 100};

    // ターン時ハイライト
    if(currentPlayer.getNumber() == player.getNumber()){
        helper.drawRect(startX - 2, startY - 2, 404, 104, HIGHLIGHT, NULL);
    }

    // 枠線
    SDL_Color rectColor = {100, 100, 100, 255};
    helper.drawRect(rect.x, rect.y, rect.w, rect.h, rectColor, NULL);


    // プレイヤーインデックス
    std::string playerText = "プレイヤー" + std::to_string(player.getNumber());
    helper.drawText(playerText, WHITE, 20, startX + 5, startY);

    // 威信点
    std::string pointText = std::to_string(player.getPrestigePoints());
    helper.drawText("威信点", GOLD, 15, startX + 345, startY + 10);
    // helper.drawText(pointText, GOLD, 60, startX + 345, startY + 20);
    helper.drawRect(startX + 335, startY + 30, 65, 60, TRANSPARENT, helper.renderText(pointText, GOLD, 50));

    int tokenSpace = 10;
    SDL_Color textColor;
    // カードカラー
    for(int i=static_cast<int>(Token::Color::RED); i<=static_cast<int>(Token::Color::BLACK); i++){
        Token::Color cardColor = static_cast<Token::Color>(i);
        SDL_Color cardColorSDL = Token::colorToSDLColor(cardColor);

        std::string cardCountText = std::to_string(player.getCardTokenCount(cardColor));
        textColor = (cardColor == Token::Color::WHITE || cardColor == Token::Color::GOLD) ? BLACK : WHITE;
        SDL_Texture* textTexture = helper.renderText(cardCountText, textColor, 20);
        helper.drawRect(startX + 15 + (tokenSpace + 30) * i , startY + 30, 30, 30, cardColorSDL, textTexture);
    }

    // 宝石トークン
    for(int i=static_cast<int>(Token::Color::RED); i<=static_cast<int>(Token::Color::GOLD); i++){
        Token::Color tokenColor = static_cast<Token::Color>(i);
        SDL_Color tokenColorSDL = Token::colorToSDLColor(tokenColor);

        std::string tokenCountText = std::to_string(player.getTokenCount(tokenColor));
        textColor = (tokenColor == Token::Color::WHITE || tokenColor == Token::Color::GOLD) ? BLACK : WHITE;
        SDL_Texture* textTexture = helper.renderText(tokenCountText, textColor, 20);
        helper.drawCircle(startX + 30 + (tokenSpace + 30) * i , startY + 80, 15, tokenColorSDL, textTexture);
    }

    // 予約カード枚数
    std::string reservedCardCount = std::to_string(player.getReservedCards().size());
    helper.drawText("予約", WHITE, 15, startX + 260, startY + 20);
    helper.drawText(reservedCardCount, WHITE, 40, startX + 262, startY + 40);


    // 取得貴族タイル枚数
    std::string tileCount = std::to_string(player.getOwnedNobleTiles().size());
    helper.drawText("貴族", WHITE, 15, startX + 300, startY + 20);
    helper.drawText(tileCount, WHITE, 40, startX + 304, startY + 40);

}

void GameScene::renderAllPlyerInfo(int numPlayers){ // 全プレイヤーの情報を描画
    std::vector<Player> players = game->getPlayers();

    for(int i=0; i<numPlayers; i++){
        renderPlayerInfo(players[i], 35 + 120 * i);
    }
}

void GameScene::renderAvailableTokens() { // 利用可能なトークンの数を描画
    std::map fieldTokens = game->getAvailableTokens();
    for(int i = 0; i < 6; i++){
        Token::Color tokenColor = static_cast<Token::Color>(i);
        SDL_Color tokenColorSDL = Token::colorToSDLColor(tokenColor);

        std::string countText = std::to_string(fieldTokens[tokenColor]);
        SDL_Color textColor = (i == 3 || i == 5) ? BLACK : WHITE; //白と金のトークンの文字だけ黒
        SDL_Texture* textTexture = helper.renderText(countText, textColor, 30);

        // 黒のみ枠線を描画
        if(tokenColor == Token::Color::BLACK){
            helper.drawCircle(405, 50 + 60 * i, 22, WHITE, NULL);
        }

        if(i != 5){
            tokenRadio[i]->render();
        }
        helper.drawCircle(405, 50 + 60 * i, 20, tokenColorSDL, textTexture);
    }
}

void GameScene::renderActionButtons() {
    cancelButton->setButton("キャンセル", 400, 490, 130, 30, {150, 150, 150, 255}, WHITE, 20);
    bool isSelect = false;

    if(selectCardIndex != -1 || !selectToken.empty()){
        actionButton1.first->render();
        actionButton2.first->render();
        isSelect = true;
    }else if(selectReservedCardIndex != -1){
        actionButton2.first->render();
        isSelect = true;
    }

    if(isSelect){
        cancelButton->render();
    }
}

void GameScene::fieldCardAction() {
    Card& selectCard = fieldCards[selectCardIndex];

    if(currentPlayer.isPurchasable(selectCard)){
        actionButton1.first->setButton("購入", 400, 410, 130, 30, GREEN, WHITE, 20);
        actionButton1.second = true;
    }else{
        actionButton1.first->setButton("購入", 400, 410, 130, 30, {100, 100, 100, 255}, WHITE, 20);
        actionButton1.second = false;
    }

    if(currentPlayer.getReservedCards().size() < 3){
        actionButton2.first->setButton("予約", 400, 450, 130, 30, GREEN, WHITE, 20);
        actionButton2.second = true;
    }else{
        actionButton2.first->setButton("予約", 400, 450, 130, 30, {100, 100, 100, 255}, WHITE, 20);
        actionButton2.second = false;
    }
}

void GameScene::tokenAction(){
    bool isTakable = true;
    // 3色までを1枚ずつ取得
    for(int& token : selectToken){
        if(game->getAvailableTokens().at(static_cast<Token::Color>(token)) == 0){
            isTakable = false;
        }
    }
    if(isTakable){
        actionButton1.first->setButton("1枚ずつ取得", 400, 410, 130, 30, GREEN, WHITE, 20);
        actionButton1.second = true;
    }else{
        actionButton1.first->setButton("1枚ずつ取得", 400, 410, 130, 30, {100, 100, 100, 255},
                                        WHITE, 20);
        actionButton1.second = false;
    }

    // 1色を2枚取得
    if(selectToken.size() == 1 &&  
        game->getAvailableTokens().at(static_cast<Token::Color>(selectToken.back())) >= 4){
        actionButton2.first->setButton("2枚取得", 400, 450, 130, 30, GREEN, WHITE, 20);
        actionButton2.second = true;
    }else{
        actionButton2.first->setButton("2枚取得", 400, 450, 130, 30, {100, 100, 100, 255}, 
                                        WHITE, 20);
        actionButton2.second = false;
    }
}

void GameScene::reservedCardAction(){
    Card& selectCard = reservedCards[selectReservedCardIndex];

    if(currentPlayer.isPurchasable(selectCard)){
        actionButton2.first->setButton("購入", 400, 450, 130, 30, GREEN, WHITE, 20);
        actionButton2.second = true;
    }else{
        actionButton2.first->setButton("購入", 400, 450, 130, 30, {100, 100, 100, 255}, 
                                        WHITE, 20);
        actionButton2.second = false;
    }
}

void GameScene::invisibleActionButton() { // アクションボタンを非表示
    actionButton1.first->setInvisibleButton();
    actionButton2.first->setInvisibleButton();
    cancelButton->setInvisibleButton();
}

void GameScene::returnExcessToken() {
    std::vector<int> tokenCount(5, 0);
    for(int i=0; i<5; i++){
        tokenCount[i] += selectExcessToken[i];
    }

    for(int i=0; i<5; i++){
        SDL_Color tokenColorSDL = Token::colorToSDLColor(static_cast<Token::Color>(i));

        std::string countText = std::to_string(tokenCount[i]);
        SDL_Color textColor = (i == 3) ? BLACK : WHITE;
        SDL_Texture* textTexture = helper.renderText(countText, textColor, 25);

        if(i == 4){
            helper.drawCircle(465, 460, 20, WHITE, NULL);
        }

        excessTokenRadio[i]->render();
        helper.drawCircle(405 + 60 * (i % 3), 415 + 45 * (i / 3), 18, tokenColorSDL, textTexture);
    }

    int selectCount = 0;
    for(int& token : selectExcessToken){
        selectCount += token;
    }

    if(game->getCurrentPlayer().getTotalTokenCount() - selectCount == 10){
        returnButton.first->setButton("返却", 390, 490, 70, 30, GREEN, WHITE, 15);
        returnButton.second = true;
    }else{
        returnButton.first->setButton("返却", 390, 490, 70, 30, {100, 100, 100, 255}, WHITE, 15);
    }
    returnButton.first->render();
    
    cancelButton->setButton("リセット", 470, 490, 70, 30, {150, 150, 150, 255}, WHITE, 13);
    cancelButton->render();
}

void GameScene::gameOver(int winner) {
    actionBind = true;
    isGameOver = true;
    message = "ゲーム終了！";
    std::string winnerText = "プレイヤー" + std::to_string(winner + 1) + "の勝利！";
    result = helper.renderText(winnerText, GOLD, 100);
}

//////// トークンを複数選択→ひとつ選択にしたとき,条件を満たしていてもアクションボタン２が有効にならない