#include <iostream>
#include <ncurses.h>
#include "GameEngine.hpp"
#include "MenuScreen.hpp"
#include "Constants.hpp"

using namespace std;


/**
 * @mainpage Nibbles
 * Variation of classic snake game with multiple games modes\n
 * Player vs Player: Two players at the one PC or over network\n
 * Player vs AI: Up to three variation of AI enemy\n
 * Usage of program\n
 * Program needs path to configuration file of map. Optionally you can pass configuration of network\n
 * ./program path/to/config.map - run program with default network settings (localhost, port: 1666, two players)\n
 * ./program path/to/config.map localhost 1856 1 - run program with given network settings (localhost, port: 1856, 1 player)\n
 */
int main(int argc, char **argv) {
    string mapConfigName = "examples/1.map";
    string address = "localhost";
    string port = "1666";
    int maxClients = 2;

    if (argc == 2)
        mapConfigName = argv[1];
    else if (argc == 5) {
        mapConfigName = argv[1];
        address = argv[2];
        port = argv[3];
        maxClients = atoi(argv[4]);
        if(maxClients == 0){
            cout << "Count of max clients can't be zero or you typed incorrect format of count." << endl;
            return RETURN_BAD_ARGUMENTS;
        }

        if(atoi(port.c_str()) <= 0){
            cout << "Incorrect format of port." << endl;
            return RETURN_BAD_ARGUMENTS;
        }
    }
    else{
        cout << "Bad arguments. <program> <config file> <servername> <port> <maxPlayers>" << endl;
        cout << "<servername> <port> <maxPlayers> are optional" << endl;
        return RETURN_BAD_ARGUMENTS;
    }
    initscr();
    if (!has_colors()) {
        cout << "Your terminal doesn't support terminal." << endl;
        endwin();
        return RETURN_COLOR_NOT_SUPPORTED;
    }

    start_color();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    noecho();
    refresh();
    GameEngine *engine = new GameEngine(mapConfigName, address, port, maxClients);
    try {
        engine->Start();
        engine->ChangeScreen(new MenuScreen(*engine));
    }
    catch (const char *msg) {
        engine->DisplayCrash(msg);
    }

    while (engine->IsRunning()) {
        try {
            engine->Update();
        }
        catch (const char *msg) {
            engine->DisplayCrash(msg);
            continue;
        }
        engine->Draw();
        engine->Sleep(150);
    }

    if (engine->IsCrashState()) {
        //wait for input
        printw("Press any key to continue...");
        nodelay(stdscr, false); //wait for input
        getch();
    }

    delete engine;
    endwin();
    return 0;
}