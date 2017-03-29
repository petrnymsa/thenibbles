#include <sys/socket.h>
#include <cstring>
#include "ServerManager.hpp"


bool ServerManager::SendOnly(int socket, const char *data, size_t length) {
    return send(socket, data, length, 0) != -1;
}

bool ServerManager::SendResponse(int socket, int type) {
    string response = parser.PrepareResponse(type);
  //  cerr << "Manager response: " << response << endl;
    return  send(socket,response.c_str(),response.length(),0) != -1;
}

bool ServerManager::SendAndWaitResponse(int socket, const char *data, size_t length, int type) {
    if(!SendOnly(socket,data,length))
         return false;
    char bufferData[1];
    ssize_t r = recv(socket,bufferData,sizeof(bufferData),0);
    if(r <= 0 )
        return  false;
    bufferData[r] = 0;
   // cerr << "Manager get bytes: " << r << endl;
 //   cerr << "Manager get: " << bufferData << endl;
    int retType = parser.GetDataType(bufferData);
  //  cerr << "Manager type: " << retType << endl;
    return type == retType;
}

bool ServerManager::GetData(int socket,char * data, int length) {
    ssize_t r = recv(socket,data,length,0);
    if(r <= 0) return false;

    data[r] = 0;
    return true;
}

