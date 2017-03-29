#include "Player.hpp"
#include "GameMap.hpp"
#include "Constants.hpp"

Player::Player(Point start, IPlayerDriver *playerDriver, short n_color, const string &player_name)
        : driver(playerDriver), color(n_color), name(player_name) {
    parts.push_back(SnakePart(Point(start.x - 2, start.y), n_color));
    parts.push_back(SnakePart(Point(start.x - 1, start.y), n_color));
    parts.push_back(SnakePart(start, n_color));

    direction = DIRECT_RIGHT;
}

Player::~Player() {
    delete driver;
}

int Player::Update(GameEngine &engine, GameMap &gameMap) {
    direction = driver->Update(engine);
    SnakePart head = parts.back();
    Point prevTail = parts.begin()->GetCoord(); //tail
    parts.erase(parts.begin());

    switch (direction) {
        case DIRECT_UP:
            head.coord.y--;
            break;
        case DIRECT_DOWN:
            head.coord.y++;
            break;
        case DIRECT_LEFT:
            head.coord.x--;
            break;
        case DIRECT_RIGHT:
            head.coord.x++;
            break;
    }
    parts.push_back(SnakePart(head.GetCoord(), head.GetColorValue()));
    if (prevTail != parts.begin()->GetCoord()) // if no part was added - last tail and actual tail aren't on same place
        gameMap.AddFloorAt(prevTail); // add floor - add empty space

    gameMap.RemoveEmptySpace(parts.back().GetCoord()); //at the head position remove empty position
    if (head.GetCoord().x <= 0 || head.GetCoord().x >= gameMap.GetWidth() - 1 || head.GetCoord().y <= 0 ||
        head.GetCoord().y >= gameMap.GetHeight() - 1)
        return RET_PLAYER_MOVE_BORDER;
    //check if player committed suicide
    if (CheckSuicide())
        return RET_PLAYER_MOVE_SUICIDE;
    return RET_PLAYER_MOVE_OK;
}

void Player::DrawToBuffer(IScreen &screen) {
    for (const auto &part : parts) {
        if (part.GetCoord() == parts.back().GetCoord()) // draw head
            screen.AddToBuffer(part.GetCoord(), part.GetPrintChar(), COLOR_YELLOW);
        else //draw tail
            screen.AddToBuffer(part.GetCoord(), part.GetPrintChar(), part.GetColorValue());
    }
}


bool Player::CheckCollisionHead(const Point &point) const {
    SnakePart head = parts.back();
    return head.CheckCollision(point);
}


bool Player::CheckCollision(const Player &enemy) const {
    for (const auto &x : parts) {
        if (enemy.CheckCollisionHead(x.GetCoord()))
            return true;
    }
    return false;
}


bool Player::CheckSuicide() const {
    Point head = parts.back().GetCoord();
    for (unsigned int i = 0; i < parts.size() - 1; i++) {
        if (parts[i].CheckCollision(head))
            return true;
    }
    return false;
}


bool Player::CheckCollision(const Point &point) const {
    for (const auto &x : parts) {
        if (x.CheckCollision(point))
            return true;
    }
    return false;
}


Point Player::AddPart() {
    SnakePart tail = parts.front();
    // insert at begin (tail)
    parts.insert(parts.begin(), SnakePart(tail.GetCoord(), tail.GetColorValue()));
    return tail.GetCoord();

}
string Player::GetName() const {
    return name;
}

short Player::GetColor() const {
    return color;
}

const Point &Player::GetHeadPosition() const {
    return parts.back().GetCoord();

}