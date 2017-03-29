#ifndef FOOD_HPP
#define FOOD_HPP


#include "IGameObject.hpp"

/**
 * Represents food entity
 */
class Food : public IGameObject {


public:
    /**
     * Create new instace of Fodd
     * @param[in] point Position of food
     */
    Food(const Point & point);
    /**
     * Check if object is solid or not
     * @return true if object is solid, otherwise false
     */
    virtual bool IsSolid() const override;

    /**
     * Create clone of Food object
     * @return Clonned object
     */
    virtual IGameObject *Clone() const override;
};


#endif //FOOD_HPP
