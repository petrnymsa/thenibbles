#ifndef MAPCONFIG_HPP
#define MAPCONFIG_HPP


#include "Point.hpp"
#include <vector>
/**
 * Holds information about current map, like width, height or coordinates of walls
 */
class MapConfig {
public:
    /**
     * Create empty config
     */
    MapConfig();
    /**
     * Create MapConfig with given informations
     * @param[in] width Width of map
     * @param[in] height Height of map
     * @param[in] walls Vector of Point of coordinates of walls
     */
    MapConfig(int width, int height, std::vector<Point> walls);

    /**
     * Get width
     * @return width
     */
    int GetWidth() const;
    /**
     * Get Height
     * @return height
     */
    int GetHeight() const;
    /**
     * Get walls
     * @return returns vector of wall coordinates
     */
    std::vector<Point> GetWalls() const;

private:
    int width;/**< Width of map */
    int height;/**< Height of map */
    std::vector<Point> walls;/**< Walls in the map */

};


#endif //MAPCONFIG_HPP
