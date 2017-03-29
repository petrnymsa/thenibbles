#include <cmath>
#include "SmartAiPlayerDriver.hpp"
SmartAiPlayerDriver::SmartAiPlayerDriver(GameMap &ref_map)
:AiPlayerDriver(ref_map), dummyFood(false){
}

SmartAiPlayerDriver::~SmartAiPlayerDriver() { }

PlayerDirection SmartAiPlayerDriver::Update(GameEngine &engine) {
    Point  playerHead = owner->GetHeadPosition();
    if (finish == playerHead) { // if we reached the goal
        foodEeated = true;
        dummyFood = false;
    }
    goalChanged = false;
    // if food was eated or real food doesnt exist anymore
    if(foodEeated || (!dummyFood && !gameMap.FoodStillExist(finish))){
        try {
            //try get new food
            Point newFood = gameMap.GetNearestFood(playerHead);
            finish = newFood;
            foodEeated = false;
            dummyFood = false;
            goalChanged = true;
        }
        catch (...) {
            // otherwise set dummy food
            if (!dummyFood && foodEeated) {
                finish = gameMap.GetRandEmptySpace();
                foodEeated = false;
                dummyFood = true;
                goalChanged = true;
            }
        }
    }

    bool  updated = UpdateDirection(playerHead);
    if(!updated) {
        foodEeated = true;
        goalChanged = true;
    }

    return direction;
}

IPlayerDriver *SmartAiPlayerDriver::Clone() const {
    return new SmartAiPlayerDriver(gameMap);
}

