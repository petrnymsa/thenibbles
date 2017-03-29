#ifndef SERVERSCREEN_HPP
#define SERVERSCREEN_HPP
#include "IScreen.hpp"
#include "Enums.hpp"
#include "ServerPlayerDriver.hpp"
#include "ServerDataParser.hpp"
#include "ServerManager.hpp"
#include "GameMap.hpp"
#include <map>

using namespace std;
/**
 * Screen that represents server side
 */
class ServerScreen : public IScreen {
public:
    /**
     * Create new instance of ServerScreen
     * @param[in] engine Reference to GameEngine
     */
    ServerScreen(GameEngine &engine);
    /**
     * Desotroys resources
     */
    virtual ~ServerScreen();
    /**
     * Prepare game map. Server socket
     */
    virtual void Init() override;
    /**
     * Draw log messages
     */
    virtual void Draw() override;
    /**
     * Update server by ServerState
     */
    virtual void Update() override;

private:
    ServerState state; /**< Current state of server */
    GameMap *map;/**< Game map */
    vector<pair<int, ServerPlayerDriver *> > sockets;/**< Container with client's sockets (plus server) and associated ServerPlayerDrivers */
    ServerDataParser dataParser;/**< Helper class for parsing data*/
    ServerManager server;/**< Helper class for sending and receiving data */
    int printedLines;/**< Helper variable to printing logs. Count of printed lines */
    char data[SERVER_MAX_BUFFER]; /**< Buffer data for server */
    queue<string> log;/**< Log messages */
    queue<int> colors;/**< Colors for players */
    std::map<int, bool> updatedSockets;/**< Used by Push update, determine if all clients are updated */
    int playersCnt;/**< Number of connected and confirmed players */
    bool clientsClosed;/**< Indicates if clients sockets are closed or not */
    bool serverSocketClosed;/**< Indicates if server's socket is closed or not */
    int extraResponse;/**< Extra response message to clients, like SERVER_END or SERVER_ERROR */
    bool mapLoaded;/**< Indicates if map correctly lodead */
    /**
     * Processes incoming request from clients and initializing players at the map
     */
    void ProceedWaitClients();
    /**
     *  Processes incoming updated direction of players
     *  @return False if some error occurred, otherwise true
     */
    bool ProceedWaitData();
    /**
     * Send updated map and player's data to all clients
     * @return False if some error occurred, otherwise true
     */
    bool ProceedPushUpdate();
    /**
     * Waiting for client's start signals
     * @return False if some error occurred, otherwise true
     */
    bool WaitClientsStart();
    /**
     * Sends start signal to all clients
     * @return False if some error occurred, otherwise true
     */
    bool SendStartSignal();
    /**
     * Send message to specific socket
     * @param[in] socket Socket where to send message
     * @param[in] msg Message to send
     * @param[in] color Color of message
     */
    void SendMessageToSocket(int socket, const string &msg, short color = COLOR_WHITE);
    /**
     * Check if all clients are initialized
     * @return True - all players are ready, otherwise false
     */
    bool PlayersReady() const;
    /**
     * Check if all clients sent their updated data
     * @return True - all players sent data, otherwise false
     */
    bool ClientsUpdated() const;
    /**
     * Change state of Server if some error happened or game is over
     * @param[in] msg Message to log
     * @param[in] type Type of end
     */
    void ServerEnd(const string &msg, int type);

    void CloseSocket(int socket);
    /**
     * Preapre FD_SET for listening incoming data from clients
     * @param[in] set Pointer to fd_set
     * @return Count of sockets
     */
    int PrepareFdSet(fd_set *set);
    /**
     * Get name from client
     * @param[in] socket Incoming data from this socket
     * @param[in,out] name Name of player
     * @return False if some error occurred, otherwise true
     */
    bool ServeHelloClient(int socket, string &name);
    /**
     * Add message to log
     * @param[in] msg Message to display
     */
    void AddLog(const string &msg);
};


#endif //SERVERSCREEN_HPP
