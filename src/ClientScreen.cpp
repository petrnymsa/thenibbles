#include <unistd.h>
#include <sys/socket.h>
#include <cstring>
#include "ClientScreen.hpp"
#include "ServerFactory.hpp"
#include "Constants.hpp"
#include "MenuScreen.hpp"

ClientScreen::ClientScreen(GameEngine &gameEngine)
        : IScreen(gameEngine), state(ClientState::SEND_NAME), playerDriver(KeyboardSet('w', 'a', 's', 'd')),
          initServerData(false) {
    direction = DIRECT_RIGHT;
}

ClientScreen::~ClientScreen() {
    delwin(msgWindow);
    delwin(mapWindow);
    if (sock != -1)
        close(sock);
    clear();
    refresh();
    engine.AutoInput(true);
    engine.ChangeSleepState(true);
}

void ClientScreen::Init() {
    engine.AutoInput(false);
    msgWindow = newwin(3, 60, 0, 0);
    mapWindow = newwin(MAP_MAX_HEIGHT, MAP_MAX_WIDTH, 3, 0);

    ServerFactory factory;
    if ((sock = factory.PrepareClientSocket(engine.serverName.c_str(), engine.serverPort.c_str())) == -1) {
        AddMessage("Can't connect to server. Check if server is running.");
        state = ClientState::CLIENT_ERROR;
    }
    else {
        AddMessage("Please type your name and confirm with ENTER");
        state = ClientState::SEND_NAME;
    }
    UpdateMessages(msgWindow);
    DrawMessages(msgWindow);
}

void ClientScreen::Draw() {
    DrawMessages(msgWindow);
    DrawBuffer(mapWindow);
}

void ClientScreen::Update() {
    UpdateMessages(msgWindow);
    if (state == ClientState::SEND_NAME) {
        if (SendName()) {
            AddMessage("Waiting for other players...");
            state = ClientState::SEND_START;
            engine.ChangeSleepState(false);
        }
        else ClientError();
    }
    else if (state == ClientState::SEND_START) {
        // get init data from server and draw it
        if (!initServerData) {
            if (!ProceedServerData()) {
                //cerr <<"SEND_START: proceed server data error" << endl;
                ClientError();
            }
        }
        // .. and send start signal to server
        else if (state != CLIENT_ERROR) {
            if (!SendStartSignal()) {
              //  cerr <<"SEND_START: send start signal error" << endl;
                ClientError();
            }
        }
    }
    else if (state == ClientState::WAIT_START) {
        if (!WaitStartSignal()) {
          //  cerr << "WAIT_START: error" << endl;
            ClientError();
        }
    }
    else if (state == ClientState::WAIT_FOR_UPDATE) {
        if (!ProceedServerData()) {
            ClientError();
        }
    }
    else if (state == ClientState::UPDATE_GAME) {
        if (ProceedClientUpdate() && state != ClientState::CLIENT_ERROR) {
            state = ClientState::WAIT_FOR_UPDATE;
        }
        else ClientError();
    }

    if (state == ClientState::CLIENT_ERROR || state == ClientState::CLIENT_END) {
        if (engine.GetKeyInput() == 'q') {
            engine.ChangeScreen(new MenuScreen(engine));
        }
    }

}

/* ----------------------------------------------------------------------------------------- */

bool ClientScreen::ProceedClientUpdate() {
    timeout(80);
    direction = playerDriver.Update(engine);
    string directUpdate = dataParser.PrepareDirectionUpdate(direction, SERVER_DELIMITER);
    if (!server.SendAndWaitResponse(sock, directUpdate.c_str(), directUpdate.length(), SERVER_TYPE_OK)) {
       // cerr << "Server not responded at UPDATE_GAME" << endl;
        return false;
    }
    return true;
}


bool ClientScreen::WaitStartSignal() {
    if (!server.GetData(sock, data, sizeof(data))) {
       // cerr << "wait start signal: no data" << endl;
        return false;
    }

    if (!server.SendResponse(sock, SERVER_TYPE_OK)) {
       // cerr << "wait start signal: response not send" << endl;
        return false;
    }

    int type = dataParser.GetDataType(data);
    if (type == SERVER_TYPE_START) {
        state = ClientState::WAIT_FOR_UPDATE;
    }

    return true;
}


bool ClientScreen::ProceedServerData() {
    // char buffData[SERVER_MAX_BUFFER];
    if (!server.GetData(sock, data, sizeof(data))) {
       // cerr << "WAIT_FOR_UPDATE: recv not response" << endl;
        return false;
    }
    else {
        server.SendResponse(sock, SERVER_TYPE_OK);
        int type = dataParser.GetDataType(data);
        // server give signal to respond with updated direction and re-draw screen
        if (type == SERVER_TYPE_UPDATE) {
            if(state == ClientState::WAIT_FOR_UPDATE)
                state = ClientState::UPDATE_GAME;
            else if(state == ClientState::SEND_START) // we getting init data
                initServerData = true;
        }
        else if (type == SERVER_TYPE_MSG) {
            try {
                pair<string, short> msg = dataParser.ParseMessage(data, SERVER_DELIMITER);
                AddMessage(msg.first, msg.second);
            }
            catch (...) {
                return false;
            }
        }
        else if (type == SERVER_TYPE_DRAW) {
            try {
                BufferPart part = dataParser.ParseBufferPart(data, SERVER_DELIMITER);
                AddToBuffer(part);
            }
            catch (...) {
                return false;
            }
        }
        else if (type == SERVER_TYPE_ERROR) {
            AddMessage("Connection lost. Press q to exit");
            return false;
        }
        else if (type == SERVER_TYPE_END) {
            AddMessage("Game is over. Press 'q' to return", COLOR_WHITE);
            state = ClientState::CLIENT_END;
            return true;
        }
    }

    return true;
}


bool ClientScreen::SendName() {
    echo();
    curs_set(1);
    nodelay(msgWindow, false);
    keypad(msgWindow, true);
    string name; //("Player");
    int ch;
    bool firstChar = true;
    while (name.empty()) {
        firstChar = true;
        while ((ch = wgetch(msgWindow)) != '\n') {
            if (name.length() > 1 && ch == KEY_BACKSPACE) {
                int x = getcurx(msgWindow);
                nocbreak();
                wmove(msgWindow, getcury(msgWindow), x);
                waddch(msgWindow, ' ');
                wmove(msgWindow, getcury(msgWindow), x);
                cbreak();
                name.pop_back();
            }
            else name.push_back(ch);
            if (firstChar) {
                wclear(msgWindow);
                box(msgWindow, 0, 0);
                wrefresh(msgWindow);
                wmove(msgWindow, 1, 1);
                waddch(msgWindow, ch);
                firstChar = false;
            }
        }
    }
    noecho();
    curs_set(0);
    nodelay(msgWindow, true);


    if (!server.SendAndWaitResponse(sock, name.c_str(), name.length(), SERVER_TYPE_OK)) {
        state = ClientState::CLIENT_ERROR;
       // cerr << "recv not server ok" << endl;
        return false;
    }

    return true;
}

bool ClientScreen::SendStartSignal() {
    if (engine.GetKeyInput() != ERR) {
        string status = dataParser.PrepareResponse(CLIENT_TYPE_START);
        if (!server.SendAndWaitResponse(sock, status.c_str(), status.length(), SERVER_TYPE_OK))
            return false;

        state = ClientState::WAIT_START;
    }
    return true;
}

void ClientScreen::ClientError() {
    state = ClientState::CLIENT_ERROR;
    AddMessage("Connection lost. Press 'q' to return.");
}

