#ifndef AIPLAYERDRIVER_HPP
#define AIPLAYERDRIVER_HPP
#include <list>
#include "IPlayerDriver.hpp"
#include <map>
#include "GameMap.hpp"


/**
 * Represents basic AI driver for player.
 * This basic AI (called Random) choosing goal randomly and ignores nearest food.
 */
class AiPlayerDriver : public IPlayerDriver {
public:
    /**
     * Create new instance
     * @param[in] ref_map Reference to GameMap
     */
    AiPlayerDriver(GameMap & ref_map);
    /**
     * Destorys resources
     */
    virtual ~AiPlayerDriver();

    /**
     * Update drivers direction by BFS algorithm
     * @param[in] engine Reference to GameEngine
     */
    virtual PlayerDirection Update(GameEngine &engine) override;
    /**
     * Create clone instance of driver
     */
    virtual IPlayerDriver *Clone() const override;

protected:
    /**
     * Helper functor for compare position and priority
     */
    struct PriorCmpFunctor{
        bool  operator()(pair<Point, int> &lhs, pair<Point, int> &rhs) {
            return lhs.second > rhs.second;
        }
    };


    GameMap & gameMap;/**< Reference to GameMap */
    Point finish;/**< Choosen goal */
    bool foodEeated;/**< Indicates if player had reached goal or not */
    bool goalChanged;/**< Indicates if goal changed or not */
    list<Point> path;/**< Path to goal */
    priority_queue<pair<Point,int>, vector<pair<Point,int> >, PriorCmpFunctor > priorQ;/**< Frontier for A* algorithm */
    map<Point, Point> parents;/**< Remember nodes */
    map<Point,int> costs;/**< Costs of each nodes - used by A* */
    /**
     * Returns neighbors of given position (node for BFS). Checking map borders, but not empty spaces
     * @param[in] from Position
     * @param[out] neighbors Fill vector of positions with neighbors
     */
    void GetNeighbors(const Point & from, vector<Point> & neighbors) const;
    /**
     * Update direction by given next position and player's head
     * @param[in] playerHead Position of player head
     * @return True, if direction was updated, otherwise return false
     */
    bool UpdateDirection(const Point & playerHead);
    /**
     * Calculate path with BFS algorithm
     * @param[in] start Position where BFS can start
     * @return True if path exist, otherwise false
     */
    bool FillPath (const Point & start);
    /**
     * Compute Manhattan distance - needed by A* algorithm
     * @param[in] a First position
     * @param[in] b Second position
     * @return Computed distance
     */
    int Heuristic(const Point & a, const Point & b) const;
};


#endif //AIPLAYERDRIVER_HPP
