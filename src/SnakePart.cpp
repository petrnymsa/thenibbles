#include "SnakePart.hpp"
#include <ncurses.h>
SnakePart::SnakePart(const Point &point, short n_color)
        :IGameObject(point){
    printChar='o';
    color = n_color;
    objName = "SnakePart";
}

bool SnakePart::IsSolid() const {
    return true;
}

IGameObject* SnakePart::Clone() const {
    return  new SnakePart(coord,color);
}



