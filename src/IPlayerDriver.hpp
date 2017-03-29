#ifndef IPlayerDriver_HPP
#define IPlayerDriver_HPP
#include "Enums.hpp"
#include "GameEngine.hpp"

class Player;
/**
 * Abstract class of player driver.
 * It serve as updater of player's direction
 */
class IPlayerDriver {
public:
    /**
     * Create new instance of IPlayerDriver
     */
    IPlayerDriver() { direction = PlayerDirection ::DIRECT_RIGHT; owner = nullptr;};
    /**
     * Destroys resources
     */
    virtual ~IPlayerDriver() { };
    /**
     * Update direction
     * @param[in] engine Reference to GameEngine.
     * @return PlayerDirection enum
     */
    virtual PlayerDirection Update(GameEngine & engine) = 0;
    /**
     * Create new clone of instance
     * @return Clonned object
     */
    virtual  IPlayerDriver * Clone() const = 0;
    /**
     * Set owner of this driver
     * @param[in] playerOwner Owner of driver
     */
    void SetOwner(Player & playerOwner){
        owner = &playerOwner;
    }
protected:
    PlayerDirection direction; /**< Currently set direction */
    Player * owner;/**< Reference to owner of driver */
};
#endif //IPLAYERDRIVER_HPP
