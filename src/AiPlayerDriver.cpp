#include <map>
#include <algorithm>
#include "AiPlayerDriver.hpp"

AiPlayerDriver::AiPlayerDriver(GameMap &ref_map)
        : gameMap(ref_map), foodEeated(true), goalChanged(true) {
    finish = gameMap.GetRandEmptySpace(); //some dummy place where to go
}

AiPlayerDriver::~AiPlayerDriver() { }

PlayerDirection AiPlayerDriver::Update(GameEngine &engine) {
    const Point &playerHead = owner->GetHeadPosition();
    goalChanged = false;
    if (finish == playerHead) {
        foodEeated = true;
    }

    if (foodEeated) {
        finish = gameMap.GetRandEmptySpace();
        foodEeated = false;
        goalChanged = true;
    }
    bool updated = UpdateDirection(playerHead);
    if (!updated) {
        foodEeated = true;
        goalChanged = true;
    }

    return direction;
}

IPlayerDriver *AiPlayerDriver::Clone() const {
    return new AiPlayerDriver(gameMap);
}

bool AiPlayerDriver::UpdateDirection(const Point &playerHead) {
    bool pathInvalid = gameMap.PathCollision(path);
    bool canMove = true;
    if (pathInvalid || goalChanged) {// if path not valid or goal changed do BFS again
        canMove = FillPath(playerHead);
    }
    if (canMove && !path.empty()) { // if path exist get next move, otherwise move same direction
        Point move = path.front();
        path.pop_front();
        if (move.x < playerHead.x)
            direction = DIRECT_LEFT;
        else if (move.x > playerHead.x)
            direction = DIRECT_RIGHT;
        else if (move.y < playerHead.y)
            direction = DIRECT_UP;
        else if (move.y > playerHead.y)
            direction = DIRECT_DOWN;
    }
    return canMove;
}

/**
 * Using A* algorithm.
 * I was inspired by "Introduction to A*" article, which can be found at
 * http://www.redblobgames.com/pathfinding/a-star/introduction.html
 */
bool AiPlayerDriver::FillPath(const Point &start) {
    const vector<Point> &nodes = gameMap.GetEmptySpaces();
    parents.clear();
    costs.clear();
    while (!priorQ.empty())
        priorQ.pop();

    bool status = false;
    parents[start] = start;
    costs[start] = 0;
    priorQ.push(make_pair(start, 0));
    while (!priorQ.empty()) {
        pair<Point, int> current = priorQ.top();
        priorQ.pop();

        if (current.first == finish) { //goal found
            status = true;
            break;
        }

        vector<Point> neighbors;
        GetNeighbors(current.first, neighbors);
        for (auto &next : neighbors) {
            if (find(nodes.begin(), nodes.end(), next) != nodes.end()) {
                int newCost = costs[current.first] + 1;
                if (!costs.count(next) || newCost < costs[next]) {
                    costs[next] = newCost;
                    int priority = newCost + Heuristic(next, finish);
                    priorQ.push(make_pair(next, priority));
                    parents[next] = current.first;
                }
            }
        }
    }
    // reconstruct path
    if (status) {
        Point c = finish;
        path.clear();
        path.push_front(c);
        while (c != start) {
            c = parents.find(c)->second;
            if (c != start) //don't insert to path START
                path.push_front(c);
        }
    }
    return status;
}

void AiPlayerDriver::GetNeighbors(const Point &from, vector<Point> &neighbors) const {
    int width = gameMap.GetWidth();
    int height = gameMap.GetHeight();
    if (from.x > 1)
        neighbors.push_back(Point(from.x - 1, from.y));
    if (from.x < width - 1)
        neighbors.push_back(Point(from.x + 1, from.y));
    if (from.y > 1)
        neighbors.push_back(Point(from.x, from.y - 1));
    if (from.y < height - 1)
        neighbors.push_back(Point(from.x, from.y + 1));
}

int AiPlayerDriver::Heuristic(const Point & a, const Point & b) const{
    return abs(a.x - b.x) + abs(a.y - b.y);
}







