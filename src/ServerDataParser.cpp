#include <sstream>
#include "ServerDataParser.hpp"
#include "Constants.hpp"


int ServerDataParser::GetDataType(const char *data) {
    string s(data);
    if(s.length() == 1)
        return atoi(s.c_str());
    else{
        size_t  pos = s.find(SERVER_DELIMITER);
        return  atoi(s.substr(0,pos).c_str());
    }
}


pair<string,short> ServerDataParser::ParseMessage(const char *data, char delimiter) {
    vector<string> parts;
    stringstream stream(data);
    string line;
    while(getline(stream,line,delimiter)){
        parts.push_back(line);
    }

    if(parts.size() != 3)
        throw "Data bad format";

    pair<string,short> msg(parts[1],atoi(parts[2].c_str()));
    return msg;
}


BufferPart ServerDataParser::ParseBufferPart(const char * data, char delimiter){
    vector<string> parts;
    stringstream stream(data);
    string line;

    while(getline(stream,line,delimiter)){
        parts.push_back(line);
    }
    if(parts.size() != 4)
        throw "Bad format";
    Point point = ParsePoint(parts[1]);
    char character = atoi(parts[2].c_str());
    short color = atoi(parts[3].c_str());
    return BufferPart(point,character,color);
}


string ServerDataParser::PrepareResponse(int type) {
    ostringstream oss;
    oss << type;
    return oss.str();
}

PlayerDirection ServerDataParser::ParseDirection(const char *data, char delimiter) {
    string s(data);
    size_t pos = 0;
    if((pos = s.find(delimiter)) != string::npos){
        s.erase(0,pos + 1);
    }
    PlayerDirection  direction = (PlayerDirection )atoi(s.c_str());
    return  direction;
}

string ServerDataParser::PrepareBufferPart(const BufferPart &part, char delimiter) {
    stringstream ss;
    int charValue = part.ch;
    ss << SERVER_TYPE_DRAW << delimiter << part.point.x <<"," << part.point.y <<delimiter << charValue << ";" << part.color;
    return  ss.str();
}

string ServerDataParser::PrepareMessage(const pair<string,short> & message, char delimiter) {
    stringstream ss;
    ss << SERVER_TYPE_MSG << delimiter << message.first << delimiter << message.second;
    return ss.str();
}

string ServerDataParser::PrepareDirectionUpdate(PlayerDirection direction, char delimiter) {
    stringstream ss;
    ss << CLIENT_TYPE_UPDATE << delimiter << direction;
    return  ss.str();
}

Point ServerDataParser::ParsePoint(string line){
    size_t pos = line.find(',');
    if(pos == string::npos)
        throw  "Bad coordinates format";
    int x = atoi(line.substr(0,pos).c_str());
    int y = atoi(line.substr(pos+1,line.size()).c_str());

    return Point(x,y);
}