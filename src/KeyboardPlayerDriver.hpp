#ifndef KeyboardPlayerDriver_HPP
#define KeyboardPlayerDriver_HPP
#include "IPlayerDriver.hpp"
#include "KeyboardSet.hpp"
/**
 * Serve as keyboard input and updater of player's direction
 */
class KeyboardPlayerDriver: public IPlayerDriver {
public:
    /**
     * Create new instance
     * @param[in] set Reference KeyboardSet which holds informations about keys.
     */
    KeyboardPlayerDriver(const KeyboardSet & set);
    /**
       * Update direction
       * @param[in] engine Reference to GameEngine.
       * @return PlayerDirection enum
       */
    virtual PlayerDirection Update(GameEngine & engine) override;
    /**
      * Create new clone of instance
      * @return Clonned object
      */
    virtual  IPlayerDriver * Clone() const override ;
private:
    KeyboardSet keyset; /**< Holds information about keys to update direction */
};
#endif //KeyboardPlayerDriver_HPP
