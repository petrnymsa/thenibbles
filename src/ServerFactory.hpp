#ifndef SERVERFACTORY_HPP
#define SERVERFACTORY_HPP

/**
 * Helper class for creating sockets
 */
class ServerFactory{
public:
    /**
     * Create server socket
     * @param[in] address Address of server
     * @param[in] port Port of server
     * @param[in] maxClients Maximal count of connected clients
     * @return created socket
     */
    int PrepareServerSocket(const char * address, const char * port, int maxClients);
    /**
     * Create client socket
     * @param[in] address Address of server
     * @param[in] port Port of server
     * @return created socket
     */
    int PrepareClientSocket(const char * address, const char * port);

private:
    template <typename func>
    /**
     * Helper method to translate address and port to socket
     * @param[in] address Address of server
     * @param[in] port Port of server
     * @param[in] maxClients Maximal count of connected clients
     * @param[in] f Reference to function / method to obtain additional steps
     */
    int PrepareSocket(const char * address, const char * port, int maxClients,const func & f);

};
#endif //SERVERFACTORY_HPP
