#ifndef SNAKEPART_HPP
#define SNAKEPART_HPP
#include "IGameObject.hpp"
#include <ncurses.h>
/**
 * Represents part of snake's body
 */
class SnakePart : public IGameObject {

public:
    /**
     * Create new instance
     * @param[in] point Position of part
     * @param[in] n_color Color of part. Default is COLOR_GREEN
     */
    SnakePart(const Point & point, short n_color=COLOR_GREEN);
    /**
    * Check if object is solid or not
    * @return true if object is solid, otherwise false
    */
    virtual bool IsSolid() const;
    /**
    * Create clone of SnakePart object
    * @return Clonned object
    */
    virtual IGameObject * Clone() const override ;
};
#endif //SNAKEPART_HPP
