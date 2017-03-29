#ifndef SMARTERAIPLAYERDRIVER_HPP
#define SMARTERAIPLAYERDRIVER_HPP
#include "AiPlayerDriver.hpp"
/**
 * Smartest AI driver for player. Every steps trying to get nearest food.
 * Also checking if food is still available *
 */
class SmarterAiPlayerDriver : public AiPlayerDriver {

public:
    /**
     * Create new instance of driver
     * @param[in] ref_map Reference to GameMap. Needs to get information about empty spaces, foods etc.
     */
    SmarterAiPlayerDriver(GameMap &ref_map);
    /**
     * Destroys resources
     */
    virtual ~SmarterAiPlayerDriver() override;
    /**
     * Update player direction by BFS algorithm and SmarterAI logic.
     * Every steps checking availability of food, getting information about nearest food and so on
     * @param[in] engine Reference to GameEngine
     * @return Updated player's direction
     */
    virtual PlayerDirection Update(GameEngine &engine) override;
    /**
     * Create new clone of driver
     * @return Clonned object
     */
    virtual IPlayerDriver *Clone() const override;

protected:
    bool dummyFood;/**< Helper variable to checking of goal and nearest food */
};
#endif //SMARTERAIPLAYERDRIVER_HPP
