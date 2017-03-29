#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <vector>
#include "SnakePart.hpp"
#include "Enums.hpp"
#include "IPlayerDriver.hpp"
#include <ncurses.h>

class GameMap;
using namespace std;

/**
 * Class holds information about body parts (snake parts), current direction of player.
 * Also have information about current IPlayerDriver
 */
class Player {
public:
    /**
     * Create new instance of Player
     * @param[in] start Start position of Player at GameMap
     * @param[in] playerDriver Driver for updating direction
     * @param[in] n_color Color of player
     * @param[in] player_name Name of player
     */
    Player(Point start, IPlayerDriver * playerDriver, short n_color, const string & player_name);
    /**
     * Destroys resources
     */
    ~Player();
    /**
     * Update player direction
     * @param[in] engine reference GameEngine. It needed by IPlayerDriver
     * @param[in] gameMap reference to GameMap. It needed by moving behavior.
     * @return[in] Player move state - OK, BORDER, SUICIDE
     */
    int Update(GameEngine & engine,GameMap & gameMap);
    /**
     * Add parts of body to draw buffer
     * @param[in] screen Reference to screen where to draw changes
     */
    void DrawToBuffer(IScreen & screen);
    /**
     * Add SnakePart to body
     * @return Position of added part
     */
    Point AddPart();
    /**
     * Check collision with head and given point
     * @param[in] point check collision with tihs point
     * @return True for commited collision
     */
    bool CheckCollisionHead(const Point & point) const;
    /**
    * Check collision with whole body and given point
    * @param[in] point check collision with tihs point
     * @return True for commited collision
    */
    bool CheckCollision(const Point & point) const;
    /**
    * Check collision with another player - head to body
    * @param[in] enemy check collision with given Player
    * @return True for commited collision
    */
    bool CheckCollision(const Player & enemy) const;
    /**
     * Check if player commited suicide -> his head collision with his body
     * @return True for commited
     */
    bool CheckSuicide() const;

    /**
     * Return player's name
     * @return player's name
     */
    string GetName()  const;
    /**
     * Return player's color
     * @return color value
     */
    short GetColor() const;
    /**
     * Return reference to player's head position
     */
    const Point & GetHeadPosition() const;

private:
    vector<SnakePart> parts; /**< Parts of body */
    IPlayerDriver * driver; /**< IPlayerDriver changing current direction */
    PlayerDirection  direction;/**< Current direction of Player */
    short color; /**< Color of Player's body */
    string name; /**< Player's name **/
};
#endif //PLAYER_HPP
