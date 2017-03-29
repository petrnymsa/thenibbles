#include "Wall.hpp"
#include <ncurses.h>

Wall::Wall(const Point &point)
        :IGameObject(point){
    printChar = '#';
    color=COLOR_WHITE;
    objName = "Wall";
}

bool Wall::IsSolid() const {
    return true;
}

IGameObject* Wall::Clone() const {
    return  new Wall(coord);
}

