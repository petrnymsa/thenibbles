#ifndef MAPCONFIGMANAGER_HPP
#define MAPCONFIGMANAGER_HPP

#include <vector>
#include <string>
#include "MapConfig.hpp"
using  namespace std;
/**
 * Parsing map configuration file
 */
class MapConfigManager {
public:
    /**
     * Initialize MapConfigManager instance. Internally set allowed key values.
     */
    MapConfigManager();

    /**
     * Parsing input file with map configuration. If parsing failed, method throws exception.
     * @param[in] fileName a string name of configuration file
     * @return MapConfig object with map configuration info
     */
    MapConfig GetConfiguration(const string & fileName);

private:
    int width; /**< Width of map */
    int height;  /**< Height of map */
    std::vector<Point> walls;/**< Coordinates of walls */
    std::vector<string> keys;/**< Allowed config keys */

    void ParseSingleWall(const string & line);

    void ParseSetOfWalls(const string & line);

    /**
     * Look-up in keys vector if given key exist
     * @param[in] key Name of key
     * @return True if key exist, otherwise false
     */
    bool KeyExist(const string & key) const;

    /**
     * Split given line by delimiter to vector of values
     * @param[in] line Line to split
     * @param[in] delimiter split by given delimiter
     * @return vector<string> splitted values by delimiter from line
     */
    vector<string> SplitLine(const string & line ,const char delimiter) const;
    /**
     * Add wall to walls container
     * @param[in] point Posiiton of wall
     */
    void AddWall(const Point & point);
};

#endif //MAPCONFIGMANAGER_HPP
