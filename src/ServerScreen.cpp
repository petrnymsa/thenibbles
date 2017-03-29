#include <unistd.h>
#include <netdb.h>
#include <sstream>
#include <cstring>
#include <algorithm>
#include "ServerScreen.hpp"
#include "ServerFactory.hpp"
#include "MenuScreen.hpp"

ServerScreen::ServerScreen(GameEngine &engine)
        : IScreen(engine),state(ServerState::WAIT_CLIENTS), printedLines(0) {
    playersCnt = 0;
    clientsClosed = false;
    serverSocketClosed = false;
    extraResponse = -1;
    mapLoaded = true;
}


ServerScreen::~ServerScreen() {
    // no need to delete ServerDriver - cause it deleted at map
    if (!clientsClosed) {
        for (size_t i = 1; i < sockets.size(); i++) {
            close(sockets[i].first);
        }
    }
    if (!serverSocketClosed) {
        close(sockets[0].first);
    }
    if(mapLoaded)
    delete map;
    engine.ChangeSleepState(true);
}


void ServerScreen::Init() {
    erase();
    curs_set(0);
    engine.ChangeSleepState(false);
    ServerFactory factory;

    state = ServerState::WAIT_CLIENTS;
    try {
        map = new GameMap(*this, engine.GetMapConfig());
    }catch (...){
        AddLog("Map has too a few empty spaces\nPress q to exit.");
        state = ServerState::SERVER_ERROR;
        serverSocketClosed = true;
        clientsClosed = true;
        mapLoaded = false;
    }
    if(mapLoaded) {
        int serverSock = -1;
        if ((serverSock = factory.PrepareServerSocket(engine.serverName.c_str(), engine.serverPort.c_str(),
                                                      engine.maxClients)) == -1) {
            AddLog("Can't create server. Press 'q' to return");
            state = ServerState::SERVER_ERROR;
            serverSocketClosed = true;
            clientsClosed = true;
        } else {
            sockets.push_back(make_pair(serverSock, nullptr));
            ostringstream oss;
            oss << "Server started at: " <<  engine.serverName << ":" << engine.serverPort << endl;
            oss << "Waiting for players of " << engine.maxClients << endl;
            AddLog(oss.str());

        }
    }
}

void ServerScreen::Draw() {
    while (!log.empty()) {
        string msg = log.front();
        log.pop();
        printw("%s\n", msg.c_str());
        refresh();
        printedLines++;
        if (printedLines > LINES) {
            printedLines = 0;
            clear();
        }

    }
}

void ServerScreen::Update() {
    if (state == ServerState::WAIT_CLIENTS) {
        if (PlayersReady() && playersCnt == engine.maxClients) {
            AddLog("Starting. Waiting for player's START signal...");
            map->DrawToBuffer(); // draw it
            //wait for "START" signal
            state = ServerState::WAIT_CLIENTS_START;
            AddMessage("Press any key to get ready");
            // init map borders
            for(int x = 0; x < map->GetWidth(); x++) {
                AddToBuffer(Point(x, 0), '#', COLOR_WHITE);
                AddToBuffer(Point(x,map->GetHeight()-1),'#',COLOR_WHITE);
            }
            for(int y=1; y < map->GetHeight(); y++){
                AddToBuffer(Point(0, y), '#', COLOR_WHITE);
                AddToBuffer(Point(map->GetWidth()-1,y),'#',COLOR_WHITE);
            }
            ProceedPushUpdate(); //and send init data
        }
        else
            ProceedWaitClients();

    }
    else if (state == ServerState::WAIT_CLIENTS_START) {
        if (!WaitClientsStart()) { //wait for signal from clients
            ServerEnd("While getting data from clients, connection dropped", SERVER_TYPE_ERROR);
        } else if (ClientsUpdated()) { // signal START from all clients
            if(!SendStartSignal()){
                ServerEnd("Send start signal, connection dropped", SERVER_TYPE_ERROR);
            }else {
                AddLog("All clients ready. Game started.");
                AddMessage("Game started");
                state = ServerState::PUSH_UPDATE;
            }
        }
    }
    else if (state == ServerState::WAIT_CLIENTS_UPDATE) {
        // wait for updates from clients
        if (ProceedWaitData()) {
            if (ClientsUpdated()) {
                state = ServerState::PUSH_UPDATE;
            }
        }
        else {
            ServerEnd("While getting data from clients, connection dropped", SERVER_TYPE_ERROR);
        }

    }
    else if (state == ServerState::PUSH_UPDATE) {
        if (map->Update() == MAP_UPDATE_GAME_OVER) {
            ServerEnd("Game over", SERVER_TYPE_END);// game over - still we need send data to clients
        }
        map->DrawToBuffer();
        if (!ProceedPushUpdate())
            state = ServerState::SERVER_ERROR;
        else if (state != SERVER_ERROR && state != SERVER_END)
            state = ServerState::WAIT_CLIENTS_UPDATE;

        if (state == ServerState::SERVER_END) {
            // update send, close
            for (size_t i = 1; i < sockets.size(); i++) {
                CloseSocket(sockets[i].first);
                i--;
                clientsClosed = true;
                serverSocketClosed = false;
            }
        }
    }
    else if (state == ServerState::SERVER_ERROR || state == SERVER_END) {
        int input = engine.GetKeyInput();
        if (input == 'q')
            engine.ChangeScreen(new MenuScreen(engine));
    }
}


void ServerScreen::ProceedWaitClients() {
    fd_set socketSet;
    int max = PrepareFdSet(&socketSet);
    int result = select(max + 1, &socketSet, NULL, NULL, NULL);
    if (result > 0) {
        if (FD_ISSET(sockets[0].first, &socketSet)) {
            struct sockaddr cliInfo;
            socklen_t cliInfoLen = sizeof(cliInfo);
            int cliSock = accept(sockets[0].first, &cliInfo, &cliInfoLen);
            // insert new connected client - but in future we need "hello" message
            sockets.push_back(make_pair(cliSock, nullptr));
            updatedSockets.insert(make_pair(cliSock, false));
            playersCnt++;
            colors.push(playersCnt);
            AddLog("Some client connected");

        }

        for (size_t i = 1; i < sockets.size(); i++) {
            if (FD_ISSET (sockets[i].first, &socketSet) && sockets[i].second == nullptr) {
                // data na nekterem z datovych soketu -> obslouzit
                string name;
                if (!ServeHelloClient(sockets[i].first, name)) {
                    AddLog("Client dropped connection");
                    updatedSockets.erase(sockets[i].first);
                    close(sockets[i].first);
                    sockets.erase(sockets.begin() + i);
                    i--;
                    playersCnt--;
                }
                else {
                    //add player to map, init his ServerDriver
                    ServerPlayerDriver *driver = new ServerPlayerDriver();
                    int color =  COLOR_GREEN;
                    if(!colors.empty()) {
                        color = colors.front();
                        colors.pop();
                    }
                    map->AddPlayer(driver, name, color);
                    sockets[i].second = driver;
                    //send back response that is everything ok
                    server.SendResponse(sockets[i].first, SERVER_TYPE_OK);
                    AddLog("Connected players: " + to_string(playersCnt));
                    SendMessageToSocket(sockets[i].first,"Your color",color);
                }
            }
        }
    }

}

bool ServerScreen::ServeHelloClient(int socket, string &name) {
    if (!server.GetData(socket, data, sizeof(data)))
        return false;
    ostringstream oss;
    oss << data;
    name = oss.str();
    AddLog(oss.str() + " confirmed");
    return true;
}

bool ServerScreen::ProceedWaitData() {
    fd_set socketSet;
    int max = PrepareFdSet(&socketSet);
    int result = select(max + 1, &socketSet, NULL, NULL, NULL);
    if (result > 0) {
        for (size_t i = 1; i < sockets.size(); i++) {
            if (FD_ISSET(sockets[i].first, &socketSet)) {
                // retrieve data from client
                if (!server.GetData(sockets[i].first, data, sizeof(data)))
                    return false;

                //get data type
                int dataType = dataParser.GetDataType(data);
                if (dataType == CLIENT_TYPE_UPDATE) {
                    PlayerDirection direction = dataParser.ParseDirection(data, SERVER_DELIMITER);
                    sockets[i].second->PushDirection(direction);
                    updatedSockets[sockets[i].first] = true;
                    server.SendResponse(sockets[i].first, SERVER_TYPE_OK);
                }
            }
        }
    }

    return true;

}


bool ServerScreen::SendStartSignal() {
    string resp = dataParser.PrepareResponse(SERVER_TYPE_START);
    for (size_t i = 1; i < sockets.size(); i++) {
        if(!server.SendAndWaitResponse(sockets[i].first,resp.c_str(),resp.length(),SERVER_TYPE_OK))
            return  false;
    }

    return true;
}


void ServerScreen::SendMessageToSocket(int socket, const string &msg, short color) {
    string data = dataParser.PrepareMessage(make_pair(msg,color),SERVER_DELIMITER);
    server.SendOnly(socket,data.c_str(),data.length());
}


bool ServerScreen::WaitClientsStart() {
    fd_set socketSet;
    int max = PrepareFdSet(&socketSet);
    int result = select(max + 1, &socketSet, NULL, NULL, NULL);
    if (result > 0) {
        for (size_t i = 1; i < sockets.size(); i++) {
            if (FD_ISSET(sockets[i].first, &socketSet)) {
                if(!server.GetData(sockets[i].first,data,sizeof(data)))
                    return false;

                int type = dataParser.GetDataType(data);
                if(type == CLIENT_TYPE_START) {
                    server.SendResponse(sockets[i].first, SERVER_TYPE_OK);
                    updatedSockets[sockets[i].first] = true;
                }
            }
        }
    }
    return true;
}


bool ServerScreen::ProceedPushUpdate() {
    // send all buffer parts to all clients
    while (!drawBuffer.empty()) {
        BufferPart part = drawBuffer.front();
        drawBuffer.pop();
        string preData = dataParser.PrepareBufferPart(part, SERVER_DELIMITER);
        // AddLog(preData);
        for (size_t i = 1; i < sockets.size(); i++) {
            if (!server.SendAndWaitResponse(sockets[i].first, preData.c_str(), preData.length(), SERVER_TYPE_OK)) {
                ServerEnd("While sending data to client, connection dropped.", SERVER_TYPE_ERROR);
                return false;
            }
        }
    }
    //send all messages
    while (!messages.empty()) {
        pair<string, short> msg = messages.front();
        messages.pop();
        //prepare message to send
        string preData = dataParser.PrepareMessage(msg, SERVER_DELIMITER);
        for (size_t i = 1; i < sockets.size(); i++) {
            if (!server.SendAndWaitResponse(sockets[i].first, preData.c_str(), preData.length(), SERVER_TYPE_OK)) {
                ServerEnd("While sending msg to client, connection dropped.", SERVER_TYPE_ERROR);
                return false;
            }
        }
    }
    /**
     * If some extra state response is set, dont send  update
     */
    //send extra state response
    if (extraResponse == SERVER_TYPE_END || extraResponse == SERVER_TYPE_ERROR) {
        string resp = dataParser.PrepareResponse(extraResponse);
        for (size_t i = 1; i < sockets.size(); i++) {
            if (!server.SendAndWaitResponse(sockets[i].first, resp.c_str(), resp.length(), SERVER_TYPE_OK)) {
                // something happened
                ServerEnd("While sending extra response to client connection dropped.", SERVER_TYPE_ERROR);
                return false;
            }
        }
        extraResponse = -1;
    }
    else {
        // send push update to all clients
        for (size_t i = 1; i < sockets.size(); i++) {
            if (!server.SendResponse(sockets[i].first, SERVER_TYPE_UPDATE)) {
                ServerEnd("While sending update response to client connection dropped.", SERVER_TYPE_ERROR);
            }
        }
    }
    // AddLog("All changes sent");
    for (auto &x : updatedSockets)
        x.second = false;
    return true;
}

/* ---------------------------------------------------------------------------------------------------------- */

int ServerScreen::PrepareFdSet(fd_set *set) {
    int max = 0;
    FD_ZERO (set);
    for (size_t i = 0; i < sockets.size(); i++) {
        FD_SET (sockets[i].first, set);
        if (i == 0 || sockets[i].first > max) max = sockets[i].first;
    }
    return max;
}

bool ServerScreen::PlayersReady() const {
    bool ready = true;
    for (size_t i = 1; i < sockets.size(); i++) {
        if (sockets[i].second == nullptr) {
            ready = false;
            break;
        }
    }
    return ready;
}


bool ServerScreen::ClientsUpdated() const {
    bool updated = true;
    for (auto &x : updatedSockets) {
        if (!x.second) {
            updated = false;
            break;
        }
    }
    return updated;
}


void ServerScreen::ServerEnd(const string &msg, int type) {
    AddLog(msg);
    if (type == SERVER_TYPE_END)
        state = ServerState::SERVER_END;
    else {
        //close all sockets - server is in crash state
        for (size_t i = 0; i < sockets.size(); i++) {
            CloseSocket(sockets[i].first);
            i--;
            clientsClosed = true;
            serverSocketClosed = true;
        }
        state = ServerState::SERVER_ERROR;
    }
    if (type == SERVER_TYPE_END)
        AddLog("Sending end");

    extraResponse = type;
    AddLog("Press 'q' to return");

}

void ServerScreen::CloseSocket(int socket) {
    auto it = lower_bound(sockets.begin(), sockets.end(), socket, [](pair<int, ServerPlayerDriver *> &lhs, int rhs) {
        return lhs.first < rhs;
    });
    if (it != sockets.end()) {
        close(socket);
        sockets.erase(it);
        updatedSockets.erase(socket);
    }
}


void ServerScreen::AddLog(const string &msg) {
    log.push(msg);
    Draw();
}