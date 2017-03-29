#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP
#include "Constants.hpp"
#include "ServerDataParser.hpp"
/**
 * Helper class to sending and receiving data
 */
class ServerManager {
public:
    /**
     * Send data to given socket
     * @param[in] socket Socket where to send data
     * @param[in] data  Data to send
     * @param[in] length Length of data
     * @return False if some error occurred, otherwise true
     */
    bool SendOnly(int socket, const char * data, size_t length);
    /**
     * Send response of given type to given socket
     * @param[in] socket Socket where to send data
     * @param[in] type Type of response
     * @return False if some error occurred, otherwise true
     */
    bool SendResponse(int socket, int type);
    /**
     * Send data to given socket and wait for response of given type
     * @param[in] socket Socket where to send data
     * @param[in] data  Data to send
     * @param[in] length Length of data
     * @param[in] type Type of response
     * @return False if some error occurred, otherwise true
     */
    bool SendAndWaitResponse(int socket,const char * data, size_t length, int type);
    /**
     * Receive data from given socket
     * @param[in] socket Socket where to send data
     * @param[in] data  Data to send
     * @param[in] length Length of data
     */
    bool GetData(int socket, char * data, int length);

private:
    ServerDataParser parser;/**< Helper class for parsing data */
};
#endif //SERVERMANAGER_HPP
