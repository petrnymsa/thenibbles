#include "MapConfig.hpp"


MapConfig::MapConfig()
:width(0),height(0),walls(std::vector<Point>()){
}

MapConfig::MapConfig(int n_width, int n_height, std::vector<Point> n_walls)
        :width(n_width), height(n_height),walls(n_walls){
}

int MapConfig::GetWidth() const {
    return width;
}

int MapConfig::GetHeight() const {
    return height;
}

std::vector<Point> MapConfig::GetWalls() const {
    return walls;
}


