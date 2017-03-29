#include <cmath>
#include "SmarterAiPlayerDriver.hpp"

SmarterAiPlayerDriver::SmarterAiPlayerDriver(GameMap &ref_map)
        : AiPlayerDriver(ref_map),dummyFood(false) {
}

SmarterAiPlayerDriver::~SmarterAiPlayerDriver() { }

PlayerDirection SmarterAiPlayerDriver::Update(GameEngine &engine) {
    Point playerHead = owner->GetHeadPosition();
    if (finish == playerHead) {
        foodEeated = true;
        dummyFood = false;
    }
    goalChanged = false;
    try { //always find nearest food
        Point newFood = gameMap.GetNearestFood(playerHead);
        if(finish != newFood){
            finish = newFood;
            goalChanged = true;
        }
        foodEeated = false;
        dummyFood = false;

    }
    catch(...) {
        //otherwise set dummy food
        if (!dummyFood && foodEeated) {
            finish = gameMap.GetRandEmptySpace();
            foodEeated = false;
            dummyFood = true;
            goalChanged = true;
        }
    }
    bool  updated = UpdateDirection(playerHead);
    if(!updated) { // if not path updated
        foodEeated = true;
        goalChanged = true;
        //but still snake can get collision
    }
    return direction;
}

IPlayerDriver *SmarterAiPlayerDriver::Clone() const {
    return new SmarterAiPlayerDriver(gameMap);
}

