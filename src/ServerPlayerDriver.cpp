#include "ServerPlayerDriver.hpp"

ServerPlayerDriver::ServerPlayerDriver()
:IPlayerDriver(){
}

ServerPlayerDriver::~ServerPlayerDriver() {
}

PlayerDirection ServerPlayerDriver::Update(GameEngine &engine) {
    return direction;
}

IPlayerDriver *ServerPlayerDriver::Clone() const {
    return new ServerPlayerDriver();
}

void ServerPlayerDriver::PushDirection(PlayerDirection dir) {
        direction = dir;
}

