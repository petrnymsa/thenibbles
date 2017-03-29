#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

/**
 * Return value, if no arguments was given to program
 */
const int RETURN_BAD_ARGUMENTS = 1;
/**
 * Return if terminal not support colors
 */
const int RETURN_COLOR_NOT_SUPPORTED = 2;

/**
 * Return if Player's move was "ok" - player did not have any collision
 */
const int RET_PLAYER_MOVE_OK = 0;
/**
 * Player had collision with map border
 */
const int RET_PLAYER_MOVE_BORDER = 1;

/**
 * Player had collision with yourself
 */
const int RET_PLAYER_MOVE_SUICIDE = 2;

const int MAP_MIN_WIDTH=20;/**< Min width of map */
const int MAP_MIN_HEIGHT=20;/** Min height of map */
const int MAP_MAX_HEIGHT=40;/** Max height of map */
const int MAP_MAX_WIDTH=60;/** Max width of map */

/**
 * Return status from map's update. Return if everythings is ok
 */
const int MAP_UPDATE_OK = 1;
/**
 * Return status from map's update. Return if game is over.
 */
const int MAP_UPDATE_GAME_OVER = -1;

//const char * const SERVER_ADDRESS = "localhost";
//const char * const SERVER_PORT = "1666";
/**
 * Delimiter for network based data
 */
const char SERVER_DELIMITER = ';';
/**
 * Max amount of players at network hosted game
 */
//const int SERVER_MAX_CLIENTS = 1;
/**
 * Max amount of bytes to send and recieve over network
 */
const int SERVER_MAX_BUFFER = 100;
/**
 * Response from server to client -> signal for clients, that they must update yourself and send repsonse back
 */
const int SERVER_TYPE_UPDATE = 1;
/**
 * Response from server to client -> message to display
 */
const int SERVER_TYPE_MSG = 2;
/**
 * Response from server to client -> change buffer screen
 */
const int SERVER_TYPE_DRAW = 3;
/**
 * Response from server to client - or client to server
 */
const int SERVER_TYPE_OK = 4;
/**
 * Response from server to client - player's ready. Game can start.
 */
const int SERVER_TYPE_START = 5;
/**
 * Response from server to client - or client to server.
 * Send when something gone wrong
 */
const int SERVER_TYPE_ERROR = 6;
/**
 * Response from server to client - game is over
 */
const int SERVER_TYPE_END = 7;

/**
 * Response from client to server - client sent updated direction
 */
const int CLIENT_TYPE_UPDATE = 1;
/**
 * Response from client to server - client is ready to play
 */
const int CLIENT_TYPE_START = 2;

#endif //CONSTANTS_HPP
