#include <stdio.h>
#include <raylib.h>
#include "Globals.h"
#include "Game.h"
#include "ResourceManager.h"
#include "LogMessages.h"
#include "Scene.h"

Game::Game()
{
    state = GameState::MAIN_MENU;
    scene = nullptr;
    img_route = nullptr;
    img_above = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man");
    InitAudioDevice();

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };	

    //Load textures
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::BeginPlay(int startPuntuation = 0, int startLives = 3, int level = 0)
{
    scene = new Scene(this, startPuntuation, startLives, level);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
    case GameState::MAIN_MENU:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::PLAYING;
        }

        frameCounter++;
        if (frameCounter % 4 == 0) // Incrementa animPhrame cada 4 frames
        {
            animPhrame++;
            if (animPhrame >= 4) animPhrame = 0;
            
        }

        position += PLAYER_SPEED * direction;
        if (position >= WINDOW_HEIGHT || position <= 0- MENU_ANIM_SIZE*5) // Cambia de dirección al alcanzar maxHeight o 0
        {
            direction *= -1;
            eating = !eating;
        }

        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE)) //PROVISIONAL RETURN MAIN MENU
        {
            ReturnToMainMenu();
        }
        else if (IsKeyPressed(KEY_F2)) //PROVISIONAL RETURN MAIN MENU
        {
            scene->GoNextLevel();
        }
        else
        {
            //Game logic
            scene->Update();
        }
        break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::MAIN_MENU:
    {
        int n = MENU_ANIM_SIZE;
        DrawTexture(*img_route, (WINDOW_WIDTH - img_route->width) / 2, WINDOW_HEIGHT / 2 - img_route->height / 2, WHITE);
        if (eating)
        {
            Rectangle sourceRect = { 0, (float)(4+animPhrame) * n,(float)5 * n,(float)n };
            DrawTextureRec(*img_animations, sourceRect, { (float)position, WINDOW_HEIGHT / 2 - sourceRect.height / 2 }, WHITE);
        }
        else
        {
            Rectangle sourceRect = { 0, (float)(animPhrame) * n, (float)5 * n,(float)n };
            DrawTextureRec(*img_animations, sourceRect, { (float)position, WINDOW_HEIGHT / 2 - sourceRect.height / 2 }, WHITE);
        }
        DrawTexture(*img_above, (WINDOW_WIDTH - img_above->width) / 2, WINDOW_HEIGHT / 2 - img_above->height / 2, WHITE);
        DrawTexture(*img_title, (WINDOW_WIDTH - img_title->width) / 2, WINDOW_HEIGHT / 4 - img_title->height / 2, WHITE);
        const char* text = "PRESS SPACE TO START";
        int fontSize = 20;
        DrawText(TextFormat(text), WINDOW_WIDTH /2 - MeasureText(text, fontSize)/2, WINDOW_HEIGHT/16 * 11, fontSize, YELLOW);
        text = "PRESS ESC TO EXIT";
        fontSize = 20;
        DrawText(TextFormat(text), WINDOW_WIDTH / 2 - MeasureText(text, fontSize) / 2, WINDOW_HEIGHT / 16 * 13, fontSize, RED);
        text = "MADE BY ALBERT & MARC";
        fontSize = 10;
        DrawText(TextFormat(text), WINDOW_WIDTH / 2 - MeasureText(text, fontSize) / 2, WINDOW_HEIGHT / 16 * 16, fontSize, WHITE);

        break;
    }

    case GameState::PLAYING:
    {
        scene->Render();
        break;
    }

    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
 }
void Game::ReturnToMainMenu()
{
    state = GameState::MAIN_MENU;
    FinishPlay();
}
void Game::GoNextLevel(int startPuntuation, int startLives, int level)
{
    FinishPlay();
    level++;
    BeginPlay(startPuntuation, startLives, level);
}
void Game::Cleanup()
{
    CloseAudioDevice();
    UnloadResources();
    CloseWindow();
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(ResourceType::IMG_MENU, "resources/sprites/menu_pacman.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_route = data.GetTexture(ResourceType::IMG_MENU);
    if (data.LoadTexture(ResourceType::IMG_MENU_ABOVE, "resources/sprites/menu_pacman_above.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_above = data.GetTexture(ResourceType::IMG_MENU_ABOVE);
    if (data.LoadTexture(ResourceType::IMG_MENU_ANIMATIONS, "resources/sprites/menu_animations.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_animations = data.GetTexture(ResourceType::IMG_MENU_ANIMATIONS);
    if (data.LoadTexture(ResourceType::IMG_MENU_TITLE, "resources/sprites/menu_titile.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_title = data.GetTexture(ResourceType::IMG_MENU_TITLE);

    return AppStatus::OK;
}
void Game::UnloadResources()
{

    ResourceManager& data = ResourceManager::Instance();
    data.Release();

    UnloadRenderTexture(target);
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}