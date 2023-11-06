// #include "Game.hpp"
// #include <iostream>

// int main() {
//     std::cout << "Splendorへようこそ!" << std::endl;

//     int numPlayers;
//     do {
//         std::cout << "ゲームをプレイする人数を選択（2~4人）: ";
//         std::cin >> numPlayers;
//     } while (numPlayers < 2 || numPlayers > 4); // 無効な入力を弾くためのループ

//     Game game(numPlayers); // ゲームの初期化。必要に応じてプレイヤー数をコンストラクタに渡す。

//     game.Play(); // ゲームの開始。このメソッド内でゲームのメインループや勝敗判定などが行われると仮定。

//     std::cout << "Game Over!" << std::endl;
//     // 必要に応じてさらに結果表示や後処理を追加。

//     return 0;
// }

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "scripts/SDLHelper.hpp"
#include "scripts/Scene.hpp"
#include "scripts/Button.hpp"

int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 540;

int main(int argc, char* argv[]){
    //SDLの初期化・ウィンドウ、レンダラの作成
    SDLHelper sdlHelper("Splendor", SCREEN_WIDTH, SCREEN_HEIGHT); //タイトル・幅・高さ
    SDL_Event e;
    bool quit = false;

    // タイトルシーン（起動時）
    TitleScene* titleScene = new TitleScene(sdlHelper);
    // GameScene gameScene(sdlHelper, 3);
    Scene* currentScene = titleScene;

    while(!quit){
        bool eventFound = false;
        while(SDL_PollEvent(&e) != 0){
           if(e.type == SDL_QUIT){
                quit = true;
                eventFound = true;
                break;
            }
            currentScene->update(e);
        //     if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION){
        //         eventFound = true;
        //         break;
        //     }
        // }
        // if(!eventFound){
        //     SDL_WaitEventTimeout(&e, 10); // 少し待ってから次のイベントをポーリング
        }
        // std::cout << currentScene << std::endl;

        // 描画
        sdlHelper.clear();
        currentScene->render(sdlHelper.getRenderer());
        sdlHelper.present();

        // シーンの切り替え判定
        if(currentScene->isSceneChange()){
            Scene* nextScene = currentScene->getNextScene();
            if(nextScene != currentScene){
                delete currentScene; // この行はコメントアウトしたままで良い
                currentScene = nextScene;
            }
        }
    }
    return 0;
}