#include "Food.hpp"
#include <ncurses.h>

Food::Food(const Point &point)
:IGameObject(point){
    printChar = '*';
    color = COLOR_MAGENTA;
    objName="food";
}

bool Food::IsSolid() const {
    return false;
}

IGameObject *Food::Clone() const {
    return new Food(coord);
}

