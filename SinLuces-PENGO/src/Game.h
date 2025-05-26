#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, CREDITS ,YOU_WIN,YOU_LOSE};

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

    Music musicStage1, musicStage2, musicStart_Menu, musicWin, musicLose;

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_credits;
    const Texture2D* img_menu_up;
    const Texture2D* img_menu_down;
    const Texture2D* img_win;
    const Texture2D* img_lose;
    Texture2D sheet;
    Rectangle frameRec;

    int frameIndex;
    float frameTime;


    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};