#include "KeyboardPlayerDriver.hpp"
#include <ncurses.h>

KeyboardPlayerDriver::KeyboardPlayerDriver(const KeyboardSet &set)
        : keyset(set) {
    direction = DIRECT_RIGHT;
}

PlayerDirection KeyboardPlayerDriver::Update(GameEngine &engine) {
    int input = engine.GetKeyInput();
    if (input == keyset.KEY_SET_UP && direction != PlayerDirection::DIRECT_DOWN)
        direction = DIRECT_UP;
    else if (input == keyset.KEY_SET_DOWN && direction != PlayerDirection::DIRECT_UP)
        direction = DIRECT_DOWN;
    else if (input == keyset.KEY_SET_LEFT && direction != PlayerDirection::DIRECT_RIGHT)
        direction = DIRECT_LEFT;
    else if (input == keyset.KEY_SET_RIGHT && direction != PlayerDirection::DIRECT_LEFT)
        direction = DIRECT_RIGHT;
    return direction;
}


IPlayerDriver *KeyboardPlayerDriver::Clone() const {
    return new KeyboardPlayerDriver(keyset);
}
