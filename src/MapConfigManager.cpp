#include "MapConfigManager.hpp"
#include "Constants.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;


MapConfigManager::MapConfigManager()
        :width(MAP_MIN_WIDTH),height(MAP_MIN_HEIGHT) {
    keys.push_back("width");
    keys.push_back("height");
    keys.push_back("wall");
}


MapConfig MapConfigManager::GetConfiguration(const string & fileName) {
    ifstream file(fileName, ios::in);
    string line;
    if(file.is_open()){
        if(file.peek() == std::ifstream::traits_type::eof()){
            file.close();
            throw "Your configuration is empty";
        }

        while(getline(file,line)){
            if(line.front() == '#') //skip comment
                continue;

            vector<string> values = SplitLine(line,'=');
            if(values.size() != 2) {
                file.close();
                throw "Incorrect format at configurtion";
            }
            if(!KeyExist(values.front())) {
                file.close();
                throw "Bad key in configuration";
            }

            if(values.front() == "width") {
                width = atoi(values[1].c_str());
                if(width < MAP_MIN_WIDTH || width > MAP_MAX_WIDTH) {
                    file.close();
                    throw "Bad size of map width. Minimum width is 20 and maximum 60";
                }
            }
            else if(values.front() == "height") {
                height = atoi(values[1].c_str());
                if (height < MAP_MIN_HEIGHT || height > MAP_MAX_HEIGHT) {
                    file.close();
                    throw "Bad size of map height. Minimum height is 20 and maximum 30";
                }
            }
            else{ // otherwise get wall coordinates
                string coord = values[1];
                size_t p = coord.find('-');
                if(p != string::npos){
                    ParseSetOfWalls(coord);
                }
                else{
                    ParseSingleWall(coord);
                }

            }
        }
    }
    else {
        throw "Filename is not exist or is not readable";
    }

    file.close();

    return MapConfig(width,height,walls);
}


void MapConfigManager::ParseSingleWall(const string &line) {
    size_t pos = line.find(',');
    if(pos == string::npos)
        throw "Bad coordinates format";

    int x = atoi(line.substr(0,pos).c_str());
    int y = atoi(line.substr(pos+1,line.size()).c_str());
    AddWall(Point(x,y));
}

void MapConfigManager::ParseSetOfWalls(const string &line) {
    vector<string> limits = SplitLine(line,'-');
    if(limits.size() != 2)
        throw "Bad wall limits configuration";

    string limit = limits[0];
    size_t pos = limit.find(',');
    if(pos == string::npos)
        throw  "Bad coordinates format at limit";

    int startX = atoi(limit.substr(0,pos).c_str());
    int startY = atoi(limit.substr(pos+1,limit.size()).c_str());

    limit = limits[1];
    pos = limit.find(',');
    if(pos == string::npos) {
        throw "Bad coordinates format at limit";
    }

    int endX = atoi(limit.substr(0,pos).c_str());
    int endY = atoi(limit.substr(pos+1,limit.size()).c_str());

    if(startX > endX || startY > endY)
        throw  "Start limit must be less than end limit";

    for(int x = startX; x <= endX; x++){
        for(int y = startY; y <= endY; y++){
            AddWall(Point(x,y));
        }
    }
}


bool MapConfigManager::KeyExist(const string &key) const {
    for(const auto & x : keys){
        if(x == key)
            return  true;
    }
    return false;
}


vector<string> MapConfigManager::SplitLine(const string &line, const char delimiter) const {
    vector<string> values;
    string value;
    stringstream stream(line);
    while(getline(stream,value,delimiter)){
        values.push_back(value);
    }

    return values;
}


void MapConfigManager::AddWall(const Point &point) {
    auto it = find(walls.begin(),walls.end(),point);
    if(it == walls.end())
        walls.push_back(point);
}



