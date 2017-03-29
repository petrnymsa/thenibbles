#ifndef MAP_HPP
#define MAP_HPP


#include <vector>
#include <queue>
#include <ncurses.h>
#include <list>
#include "IGameObject.hpp"
#include "MapConfig.hpp"
#include "BufferPart.hpp"
#include "IPlayerDriver.hpp"
#include "Player.hpp"
#include "GameScreen.hpp"
#include "Food.hpp"

using  namespace std;
/**
 * Represents game map, holds information about objects like walls or foods. Also holds informations about players.
 */
class GameMap {
public:
    /**
     * Create new instance of GameMap
     * @param[in] screen Reference to screen
     * @param[in] mapConfig Configuration of map
     */
    GameMap(IScreen & screen, const MapConfig & mapConfig);
    /**
     * Destroys resources like delete ncurses windows.
     */
    ~GameMap();
    /**
     * Add new Player to map
     * @param[in] driver Player's driver
     * @param[in] name Player's name
     * @param[in] color Player's color. Default value is GREEN
     */
    void AddPlayer(IPlayerDriver * driver, const string & name, short color =  COLOR_GREEN);
    /**
     * At given position add to map floor
     * @param[in] point Position of florr
     */
    void AddFloorAt(const Point & point);
    /**
     * Update world. Generates food, update player's states
     * @return int Map's state constant. See Constants.hpp
     */
    int Update();
    /**
     * Draw things to referenced screen buffer
     */
    void DrawToBuffer();
    /**
     *  Return container with empty spaces
     *  @return vector of positions
     */
    const vector<Point> & GetEmptySpaces() const;
    /**
     * Find nearest food to given position
     * @param[in] start Given position
     * @throws const char * If no food was found
     * @return Position of nearest food     *
     */
    Point GetNearestFood(const Point & start) const;
    /**
     * Check if given position of food is still available
     * @param[in] food Food to check
     * @return True if still exist, otherwise false
     */
    bool FoodStillExist(const Food & food) const;
    /**
      * Return randomly selected position of empty space
      * @return Position of empty space
      */
    Point GetRandEmptySpace() const;
    /**
     * Add empty space at map
     * @param[in] point Position of empty space
     */
    void AddEmptySpace(const Point & point);

     /**
     * Remove empty space point from given position
      * @param[in] point Position to remove
     */
    void RemoveEmptySpace(const Point & point);
    /**
     * Check if given path of positions collide with some player
     * @param[in] path List of positions
     * @return True if there is collision, otherwise false
     */
    bool PathCollision(const list<Point> & path) const;

    /**
     * @return width of map
     */
    int GetWidth() const;
    /**
     * @return height of map
     */
    int GetHeight() const;
private:
    /**
     * Generate empty spaces in map
     * @return Count of empty spaces
     */
    int InitEmptySpace();
    /**
     * Initializes game objects like walls
     */
    void InitObjects();
    /**
     * Initializes map buffer, draw static objects like walls
     */
    void InitDraw();
    /**
     * Check collision with given player and objects
     * @param[in] player Player to check
     * @return CollisionType enum
     */
    CollisionType CheckPlayerCollision(Player * player);
    /**
     * Generate food at random position
     */
    void GenerateFood();
    IScreen & ref_screen;/**< Reference to screen */
    int width;/**< Width of map */
    int height;/**< Height of map */
    char floorChar;/**< Character of floor */
    MapConfig config;/**< Configuration of map */
    vector<IGameObject *> objects;/**< Game objects like walls or foods */
    vector<Point> emptySpaces;/** Empty spaces. It used for example to generate food */
    vector<Player *> players;/** Player's at map */
};
#endif //MAP_HPP
