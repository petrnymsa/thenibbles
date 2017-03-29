#include "Point.hpp"

Point::Point(int px, int py)
    :x(px),y(py){
}

bool Point::operator==(const Point & obj) const {
    return x == obj.x && y == obj.y;
}

bool Point::operator!=(const Point &obj) const {
    return x != obj.x || y != obj.y;
}

bool Point::operator<(const Point &obj) const {
    return ((x < obj.x) || ((x == obj.x) && (y < obj.y)));
}

bool Point::operator<=(const Point &obj) const {
    return ((x <= obj.x) || ((x == obj.x) && (y <= obj.y)));
}

bool Point::operator>(const Point &obj) const {
   return ((x > obj.x) || ((x == obj.x) && (y > obj.y)));
}

bool Point::operator>=(const Point &obj) const {
    return ((x >= obj.x) || ((x == obj.x) && (y >= obj.y)));
}