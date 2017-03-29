#include <netdb.h>
#include <unistd.h>
#include "ServerFactory.hpp"
#include "Constants.hpp"
#include <iostream>
using namespace std;
/** This code was inspired by David Bernhaurers code, which can be find at
 * https://gitlab.fit.cvut.cz/bernhdav/bi-pa2-uschovna as demo code for semester work */
int ServerFactory::PrepareServerSocket(const char *address, const char *port, int maxClients) {
    int sockServer = PrepareSocket(address,port,maxClients, [] (addrinfo * info , int sock, int max){
        if ( bind ( sock, info-> ai_addr, info-> ai_addrlen ) == -1)
            return -1;
        if ( listen (sock, max ) == -1 )
            return -1;
        return  0;
    });
    return  sockServer;

}

int ServerFactory::PrepareClientSocket(const char *address, const char *port) {
    int sockClient = PrepareSocket(address,port,0, [] (addrinfo * info , int sock, int max){
        return connect(sock,info->ai_addr,info->ai_addrlen);
    });
    return sockClient;
}

template<typename func>
int ServerFactory::PrepareSocket(const char *adress, const char *port,int maxClients, const func &f) {
    addrinfo * info;
    if(getaddrinfo(adress,port,NULL,&info) != 0)
        return -1;
    int sock = socket(info->ai_family,SOCK_STREAM,0);
    if(sock == -1){
        freeaddrinfo(info);
        return  -1;
    }

    if(f(info,sock,maxClients) == -1){
        freeaddrinfo(info);
        close(sock);
        return  -1;
    }

    freeaddrinfo(info);
    return sock;
}


