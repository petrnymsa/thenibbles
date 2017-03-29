#include "MenuScreen.hpp"
#include "GameScreen.hpp"
#include "ServerScreen.hpp"
#include "ClientScreen.hpp"
#include <ncurses.h>
using namespace std;

MenuScreen::MenuScreen(GameEngine & engine):IScreen(engine),selected(0) {
    options.push_back("1.Player vs Player (local)");
    options.push_back("2.Player vs Player (net)");
    options.push_back("3.Player vs AI (random)");
    options.push_back("4.Player vs AI (smart)");
    options.push_back("5.Player vs AI (smarter)");
    options.push_back("6.Start server");
    options.push_back("7.Exit");
    headerPrinted = false;
}

MenuScreen::~MenuScreen() {
    delwin(menuWin);
    delwin(headerWin);
    delwin(win);

    clear();
    refresh();
}

void MenuScreen::Init() {
    erase();
    clear();
    curs_set(0);
    win = newwin(15,60,0,0);
    headerWin = newwin(6,40,1,10);
    menuWin = newwin(7,40,7,10);
    headerPrinted = false;
}

void MenuScreen::Draw() {
    if(!headerPrinted){
        refresh();
        touchwin(win);
        box(win,0,0);
        wrefresh(win);
        touchwin(headerWin);
        PrintHeader();
    }
    PrintMenu();
    wclear(menuWin);
    wclear(win);

}

void MenuScreen::Update() {
    int option = engine.GetKeyInput();
    switch(option){
        case KEY_UP:
            if(selected == MenuOptions::pvplocal)
                selected = MenuOptions::exit;
            else selected--;
            break;
        case KEY_DOWN:
            if(selected == MenuOptions::exit)
                selected = MenuOptions::pvplocal;
            else selected++;
            break;
        case 10: // aka KEY_ENTER
            PerformAction();
            break;
    }
}

void MenuScreen::PerformAction() {
    switch(selected){
        case MenuOptions::pvplocal: engine.ChangeScreen(new GameScreen(engine, GameMode::PLAYER_VS_PLAYER));
            break;
        case MenuOptions::pvpnet: engine.ChangeScreen(new ClientScreen(engine));
            break;
        case MenuOptions ::pvsai_random: engine.ChangeScreen(new GameScreen(engine, GameMode::PLAYER_VS_AI_RANDOM));
            break;
        case MenuOptions ::pvsai_smart: engine.ChangeScreen(new GameScreen(engine, GameMode::PLAYER_VS_AI_SMART));
            break;
        case MenuOptions ::pvsai_smarter: engine.ChangeScreen(new GameScreen(engine, GameMode::PLAYER_VS_AI_SMARTER));
            break;
        case MenuOptions::startServer: engine.ChangeScreen(new ServerScreen(engine));
            break;
        case MenuOptions::exit: engine.Exit();
            break;
    }
}

void MenuScreen::PrintHeader() {
    headerPrinted = true;
    touchwin(headerWin);
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    wattron(headerWin,COLOR_PAIR(1));
    wprintw(headerWin,"o   o o-O-o o--o  o--o  o    o--o  o-o\n");
    wprintw(headerWin,"|\\  |   |   |   | |   | |    |    |   \n");
    wprintw(headerWin,"| \\ |   |   O--o  O--o  |    O-o   o-o \n");
    wprintw(headerWin,"|  \\|   |   |   | |   | |    |        |\n");
    wprintw(headerWin,"o   o o-O-o o--o  o--o  O---oo--o o--o \n");
    wattroff(headerWin,COLOR_PAIR(1));
    wattron(headerWin,A_UNDERLINE);
    wprintw(headerWin,"Developed by Petr Nymsa, FIT CVUT\n");
    wattroff(headerWin,A_UNDERLINE);
    wrefresh(headerWin);
}

void MenuScreen::PrintMenu() {
    for(unsigned int i = 0; i < options.size(); i++){
        if(i == selected){
            wattron(menuWin,A_REVERSE | COLOR_PAIR(1));
            wprintw(menuWin,"%s\n",options[i].c_str());
            wattroff(menuWin,A_REVERSE | COLOR_PAIR(1));
        }
        else wprintw(menuWin,"%s\n",options[i].c_str());
    }
   wrefresh(menuWin);
}

