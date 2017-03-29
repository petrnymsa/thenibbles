#include "GameScreen.hpp"
#include "KeyboardPlayerDriver.hpp"
#include "MenuScreen.hpp"
#include "GameMap.hpp"
#include "AiPlayerDriver.hpp"
#include "SmartAiPlayerDriver.hpp"
#include "SmarterAiPlayerDriver.hpp"
#include "Constants.hpp"

GameScreen::GameScreen(GameEngine &engine, GameMode mode)
        : IScreen(engine), gameMode(mode) {
    map = nullptr;
    isGameOver = false;
    isPause = true;
    mapWindow = msgWindow = nullptr;
    mapLoaded = true;
}

GameScreen::~GameScreen() {
    if (mapLoaded)
        delete map;
    delwin(mapWindow);
    delwin(msgWindow);

    clear();
    refresh();

    cbreak();
}

void GameScreen::Init() {
    erase();
    curs_set(0);
    msgWindow = newwin(3, 60, 0, 0);
    mapWindow = newwin(engine.GetMapConfig().GetHeight(), engine.GetMapConfig().GetWidth(), 3, 0);
    touchwin(mapWindow);
    box(mapWindow, '#', '#');
    wrefresh(mapWindow);

    mapLoaded = true;
    try {
        map = new GameMap(*this, engine.GetMapConfig());
    }
    catch (...) {
        AddMessage("Map has too a few empty spaces");
        mapLoaded = false;
    }
    if (mapLoaded) {
        IPlayerDriver *driver = nullptr;

        driver = new KeyboardPlayerDriver(KeyboardSet('w', 'a', 's', 'd'));
        map->AddPlayer(driver->Clone(), "Player1", COLOR_GREEN);
        delete driver;

        switch (gameMode) {
            case GameMode::PLAYER_VS_PLAYER:
                driver = new KeyboardPlayerDriver(KeyboardSet(KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT));
                map->AddPlayer(driver->Clone(), "Player2", COLOR_BLUE);
                delete driver;
                break;
            case GameMode::PLAYER_VS_AI_RANDOM:
                driver = new AiPlayerDriver(*map);
                map->AddPlayer(driver->Clone(), "AI", COLOR_BLUE);
                delete driver;
                break;
            case GameMode::PLAYER_VS_AI_SMART:
                driver = new SmartAiPlayerDriver(*map);
                map->AddPlayer(driver->Clone(), "AI", COLOR_BLUE);
                delete driver;
                break;
            case GameMode::PLAYER_VS_AI_SMARTER:
                driver = new SmarterAiPlayerDriver(*map);
                map->AddPlayer(driver->Clone(), "AI", COLOR_BLUE);
                delete driver;
                break;

        }

        AddMessage("Press any key (except 'q') to start. Q for return to menu");

        isGameOver = false;
        isPause = true;
    }
    else {
        isGameOver = true;
    }
}

void GameScreen::Draw() {
    touchwin(msgWindow);
    box(msgWindow, 0, 0);
    wrefresh(msgWindow);
    if (mapLoaded)
        map->DrawToBuffer();
    DrawBuffer(mapWindow);
    DrawMessages(msgWindow);
}


void GameScreen::Update() {
    int key = engine.GetKeyInput();
    if (key == 'q') {
        engine.ChangeScreen(new MenuScreen(engine));
        return;
    }
    if (key == 'r')
        GameReset();

    if (isPause) {
        if (key != 'r' && key != ERR) {
            isPause = false;
            ClearMsgWin(msgWindow);
        }
    }
    else {
        if (!isGameOver && (key == 'p' || key == KEY_EXIT)) {
            AddMessage("Paused...");
            isPause = true;
        }
        if (!isGameOver) {
            if (mapLoaded && map->Update() == MAP_UPDATE_GAME_OVER)
                GameOver();
        }
    }

    UpdateMessages(msgWindow);
}


void GameScreen::GameOver() {
    isGameOver = true;
    AddMessage("Press 'r' for reset, 'q' for return to menu.");
}

void GameScreen::GameReset() {
    ClearMsgWin(msgWindow);
    delete map;
    delwin(mapWindow);
    delwin(msgWindow);
    clear();
    refresh();
    Init();
    bufferActive = false;
}