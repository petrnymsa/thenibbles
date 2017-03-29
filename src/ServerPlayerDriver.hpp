#ifndef SERVERPLAYERDRIVER_HPP
#define SERVERPLAYERDRIVER_HPP


#include "IPlayerDriver.hpp"
/**
 * Driver for player used by server
 */
class ServerPlayerDriver : public IPlayerDriver {

public:
    /**
     * Create new instance of driver
     */
    ServerPlayerDriver();
    /**
     * Destroys resources
     */
    virtual ~ServerPlayerDriver() override;
    /**
     * Update player direction by pushed direction from server
     * @param[in] engine Reference to game engine
     */
    virtual PlayerDirection Update(GameEngine &engine) override;
    /**
     * Clone current instance to new
     */
    virtual IPlayerDriver *Clone() const override;
    /**
     * Push direction from server.
     * @param[in] dir Direction of player
     */
    void PushDirection(PlayerDirection dir);
};
#endif //SERVERPLAYERDRIVER_HPP
