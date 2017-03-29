#include <ncurses.h>
#include "GameEngine.hpp"
#include "MenuScreen.hpp"
#include "MapConfigManager.hpp"

GameEngine::GameEngine(const string &mapConfigFile, string serverAddr, string port, int max)
        : mapConfigFileName(mapConfigFile), serverName(serverAddr),serverPort(port),maxClients(max),screen(nullptr), isRunnig(false), isCrashState(false), keyInput(ERR),
          isSleepAllowed(true), isAutoInput(true) {
}

GameEngine::~GameEngine() {
    delete screen;
}

void GameEngine::Update() {
    if (isAutoInput)
        keyInput = getch();
    if (screen && !isCrashState) {
        screen->Update();
    }
}

void GameEngine::Draw() {
    if (screen)
        screen->Draw();
}

void GameEngine::ChangeScreen(IScreen *scr) {
    if (screen)
        delete screen;

    clear();
    refresh();
    screen = scr;
    screen->Init();
}


void GameEngine::Sleep(clock_t sec) {
    if (isSleepAllowed) {
        clock_t start = clock();
        clock_t end = sec * 1000 + start;
        while (clock() < end) { }
    }
}

void GameEngine::Start() {
    MapConfigManager manager;
    mapConfig = manager.GetConfiguration(mapConfigFileName);

    int maxWidth = getmaxx(stdscr);
    int maxHeight = getmaxy(stdscr);

    if((maxWidth+3) < mapConfig.GetWidth() || (maxHeight+3) < mapConfig.GetHeight())
        throw "Your window size is too small. Map is larger than your screen";
    isRunnig = true;
}

void GameEngine::Exit() {
    isRunnig = false;
}

void GameEngine::ChangeSleepState(bool state) {
    isSleepAllowed = state;
}

void GameEngine::AutoInput(bool state) {
    isAutoInput = state;
}


void GameEngine::DisplayCrash(const char *msg) {
    isRunnig = false;
    isCrashState = true;
    clear();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1) | A_BOLD);
    printw("%s\n", msg);
    refresh();
    attroff(COLOR_PAIR(1) | A_BOLD);
}


bool GameEngine::IsRunning() const {
    return isRunnig;
}

bool GameEngine::IsCrashState() const {
    return isCrashState;
}

int GameEngine::GetKeyInput() const {
    if(isAutoInput)
        return  keyInput;
    else return getch();
}

MapConfig & GameEngine::GetMapConfig() {
    return mapConfig;
}






