#ifndef WALL_HPP
#define WALL_HPP
#include "IGameObject.hpp"
/**
 * Represents Wall object in the world
 */
class Wall : public IGameObject {
public:
    /**
     * Create new instance of object
     * @param[in] point Position of object
     */
    Wall(const Point & point);

    /**
    * Check if object is solid or not
    * @return true if object is solid, otherwise false
    */
    virtual bool IsSolid() const;
    /**
    * Create clone of Wall object
    * @return Clonned object
    */
    virtual IGameObject * Clone() const override ;
};
#endif //WALL_HPP
