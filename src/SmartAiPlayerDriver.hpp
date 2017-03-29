#ifndef SMARTAIPLAYERDRIVER_HPP
#define SMARTAIPLAYERDRIVER_HPP
#include "AiPlayerDriver.hpp"
/**
 * Smarter variation of AI. As a goal choosing nearest food, but at the path ignores some foods.
 * Also can choose some random position, if no foods at the map exist
 */
class SmartAiPlayerDriver : public AiPlayerDriver {
public:
    /**
    * Create new instance of driver
    * @param[in] ref_map Reference to GameMap. Needs to get information about empty spaces, foods etc.
    */
    SmartAiPlayerDriver(GameMap &ref_map);
    /**
    * Destroys resources
    */
    virtual ~SmartAiPlayerDriver() override;
    /**
    * Update player direction by BFS algorithm and SmartAI logic.
    * Get nearest food as goal and goes to it, also can check availability of food.
     * If no fodd exist, choose some random position at the map.
    * @param[in] engine Reference to GameEngine
    * @return Updated player's direction
    */
    virtual PlayerDirection Update(GameEngine &engine) override;
    /**
   * Create new clone of driver
   * @return Clonned object
   */
    virtual IPlayerDriver *Clone() const override ;

protected:
    bool dummyFood;/**< Helper variable to checking of goal and nearest food */
};
#endif //SMARTAIPLAYERDRIVER_HPP
