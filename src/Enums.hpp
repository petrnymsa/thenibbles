#ifndef ENUMS_HPP
#define ENUMS_HPP

/**
 * Information about current direction of player
 */
enum PlayerDirection{
    DIRECT_LEFT = 0, /**< Left direction */
    DIRECT_RIGHT,/**< Right direction */
    DIRECT_UP,/**< Up direction */
    DIRECT_DOWN,/**< Down direction */
};

/**
 * Information about object's collision state
 */
enum CollisionType{
    COLLISION_NONE,/**< No collision */
    COLLISION_SOLID,/**< Collision with solid object */
    COLLISION_FOOD/**< Collision with food type */
};

/**
 * Information about Game mode
 * It was chosen in menu, which game will be played
 */
enum GameMode{
    PLAYER_VS_PLAYER, /**< Player vs Player - local */
    PLAYER_VS_AI_RANDOM,  /**< Player vs AI_RANDOM */
    PLAYER_VS_AI_SMART, /**< Player vs AI_SMART */
    PLAYER_VS_AI_SMARTER /**< Player vs AI_SMARTER */
};

/**
 * States of server
 */
enum ServerState{
    WAIT_CLIENTS,/**< Wait for clients to connect */
    WAIT_CLIENTS_UPDATE,/**< Wait for client's updates */
    WAIT_CLIENTS_START,/**< Wait for client's start signals */
    PUSH_UPDATE,/**< Send all data changes to clients */
    SERVER_ERROR,/**< Something gone wrong, at this state server ended connection with clients */
    SERVER_END/**< Game ended */
};

/**
 * State of client
 */
enum ClientState{
    SEND_NAME,/**< Initialization - client sending name to server */
    SEND_START,/**< Sending start signal to server */
    WAIT_START,/**< Waiting for start signal from server */
    WAIT_FOR_UPDATE,/**< Client waits fro PUSH_UPDATE signal, also collecting data, like messages and changes of buffer */
    UPDATE_GAME,/**< Client updating his direction and re-draw screen. Sending update to server */
    CLIENT_ERROR,/** Something gone wrong. Client ended connection with server */
    CLIENT_END /** Game ended  */
};
#endif //ENUMS_HPP
