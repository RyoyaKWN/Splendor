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
        }
        
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