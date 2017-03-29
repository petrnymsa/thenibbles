#include "IGameObject.hpp"
#include <ncurses.h>

IGameObject::IGameObject(const Point &point)
        :coord(point),color(COLOR_WHITE),printChar('.') {
   // objName = "IGameObject";
}

IGameObject::~IGameObject() { }

short IGameObject::GetColorValue(void) const {
    return color;
}

char IGameObject::GetPrintChar(void) const {
    return printChar;
}

string IGameObject::GetObjName() const {
    return  objName;
}


const Point & IGameObject::GetCoord(void) const {
    return coord;
}

bool IGameObject::CheckCollision(const Point &x) const {
    return coord == x;
}