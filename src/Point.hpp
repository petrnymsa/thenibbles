#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <ostream>

/**
 * Represents position at the map
 */
class Point {
public:
    int x;
    /**< X-position */
    int y;/**< Y-position */
    /**
     * Create new instanace of Point
     * @param[in] px X-position
     * @param[in] py Y-posiion
     */
    Point(int px = 0, int py = 0);

    /**
     * Compares two Points if they are equal
     * @param[in] x Second Point
     * @return True if equal
     */
    bool operator==(const Point &x) const;

    /**
     * Compares two Points if they are not equal
     * @param[in] x Second Point
     * @return True if not equal
     */
    bool operator!=(const Point &x) const;

    /**
    * Compares two Points if first is lower than second
    * @param[in] x Second Point
    * @return True if first is lower than second
    */
    bool operator<(const Point &x) const;

    /**
    * Compares two Points if first is lower or equal than second
    * @param[in] x Second Point
    * @return True if first is lower or equal than second
   */
    bool operator<=(const Point &x) const;

    /**
   * Compares two Points if first is greater or equal than second
   * @param[in] x Second Point
   * @return True if first is  greater or equal  than second
   */
    bool operator>=(const Point &x) const;

    /**
    * Compares two Points if first is greater than second
    * @param[in] x Second Point
    * @return True if first is  greater than second
    */
    bool operator>(const Point &x) const;
};

#endif //POINT_HPP
