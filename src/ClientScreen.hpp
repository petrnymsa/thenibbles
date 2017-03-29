#ifndef SEMESTRALKA_CLIENTSCREEN_HPP
#define SEMESTRALKA_CLIENTSCREEN_HPP

#include "IScreen.hpp"
#include "Enums.hpp"
#include "ServerDataParser.hpp"
#include "Constants.hpp"
#include "KeyboardPlayerDriver.hpp"
#include "ServerManager.hpp"

/**
 * Represents client's screen for connecting to server
 */
class ClientScreen : public IScreen {

public:
    /**
     * Create new instance of ClientScreen
     * @param[in] gameEngine reference to GameEngine
     */
    ClientScreen(GameEngine &gameEngine);
    /**
     * Destroys resources and close socket
     */
    virtual ~ClientScreen() override;
    /**
     * Create client's socket, prepare game
     */
    virtual void Init() override;
    /**
     * Draw changes from server to screen
     */
    virtual void Draw() override;
    /**
     * Update client by current ClientState
     */
    virtual void Update() override;

private:
    int sock;/**< Client's socket */
    ClientState state;/**< State of Client */
    ServerDataParser dataParser;/**< Helper class for parse data */
    KeyboardPlayerDriver playerDriver;/**< Driver for changing direction of player */
    ServerManager server;/**< Helper class for communication with server */
    WINDOW * msgWindow;/**< Window for messages */
    WINDOW * mapWindow;/**< Window for map */
    PlayerDirection  direction;/**< Current player direction */
    bool initServerData;/**< Indicates if server send initializing data */
    char data[SERVER_MAX_BUFFER];/**< Buffer for sending and receiving data */

    /**
     * Send client's name to server
     * @return False if some error occurred, otherwise true
     */
    bool SendName();
    /**
     * Send start signal to server
     * @return False if some error occurred, otherwise true
     */
    bool SendStartSignal();
    /**
     * Wait for start signal from server
     * @return False if some error occurred, otherwise true
     */
    bool WaitStartSignal();
    /**
     * Parse incoming data from server
     * @return False if some error occurred, otherwise true
     */
    bool ProceedServerData();
    /**
     * Update direction and send back to server
     * @return False if some error occurred, otherwise true
     */
    bool ProceedClientUpdate();
    /**
     * Set client to CLIENT_ERROR state and display message
     */
    void ClientError();

};
#endif //CLIENTSCREEN_HPP
