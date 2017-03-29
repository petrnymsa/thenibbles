#include "GameMap.hpp"
#include "Wall.hpp"
#include "Food.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Constants.hpp"

GameMap::GameMap(IScreen & screen, const MapConfig &mapConfig)
        : ref_screen(screen),config(mapConfig) {
    srand(time(NULL));
    floorChar = ' ';
    width = mapConfig.GetWidth();
    height = mapConfig.GetHeight();
    InitObjects();
    if(InitEmptySpace() < 20) {
        for (auto x : objects)
            delete x;
        throw "Map has too a few empty spaces";
    }
}

GameMap::~GameMap() {
    for (auto x : players)
        delete x;
    for (auto x : objects)
        delete x;
}


void GameMap::InitObjects() {
    vector<Point> walls = config.GetWalls();
    for (const auto &x : walls) {
        auto low = lower_bound(objects.begin(), objects.end(), x, [](IGameObject *obj, const Point &point) {
            return obj->GetCoord() < point;
        });
        objects.insert(low, new Wall(x));
    }
}

void GameMap::AddPlayer(IPlayerDriver *driver, const string &name, short color) {
    Point point = GetRandEmptySpace();
    // don't place player near the borders
    while (point.x < 3 || point.x > width - 4)
        point = GetRandEmptySpace();
    bool collision = true;
    while (collision) {
        collision = false;
        //new player cant collidate with any other player's parts
        for (const auto &pl : players) {
            if (pl->CheckCollision(point) || pl->CheckCollision(Point(point.x - 1, point.y))
                || pl->CheckCollision(Point(point.x - 2, point.y))) //Check collisions with parts
                collision = true;
        }
        if (!collision) { //also added player's parts cant collidate with objects
            for (const auto &obj : objects) {
                if (obj->CheckCollision(point) || obj->CheckCollision(Point(point.x - 1, point.y))
                    || obj->CheckCollision(Point(point.x - 2, point.y)) ||
                    obj->CheckCollision(Point(point.x + 1, point.y))) //+ dont place near some object
                    collision = true;
            }
        }
        if (collision)
            point = GetRandEmptySpace();
    }
    Player * player = new Player(point,driver,color,name);
    driver->SetOwner(*player);
    players.push_back(player);
    RemoveEmptySpace(point);
    RemoveEmptySpace(Point(point.x - 1, point.y));
    RemoveEmptySpace(Point(point.x - 2, point.y));


}

int GameMap::InitEmptySpace() {
    int cnt = 0;
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            Point point(x, y);
            bool exist = false;
            for(const auto & obj : objects){
                if(obj->CheckCollision(point)){
                    exist = true;
                    break;
                }
            }
            if(!exist){
                emptySpaces.push_back(point);
                cnt++;
            }
        }
    }
    return cnt;
}

void GameMap::InitDraw() {
    for (const auto &x : objects)
        ref_screen.AddToBuffer(x->GetCoord(), x->GetPrintChar(), x->GetColorValue());
}


int GameMap::Update() {
    GenerateFood();
    for (auto pl : players) {
        // update player's positions
        int status = pl->Update(ref_screen.engine, *this);
        if (status == RET_PLAYER_MOVE_BORDER || status == RET_PLAYER_MOVE_SUICIDE) {
            ref_screen.AddMessage(pl->GetName() + " committed suicide", pl->GetColor());
            return MAP_UPDATE_GAME_OVER;
        }
    }
    // now check collisions with map objects and other players
    for (auto pl : players) {
        CollisionType mapCollision = CheckPlayerCollision(pl);
        if (mapCollision == CollisionType::COLLISION_FOOD) {
            Point p = pl->AddPart(); //add part to player
            RemoveEmptySpace(p); //at given position remove empty space
        }
        else if (mapCollision == CollisionType::COLLISION_SOLID) { // collision with walls
            ref_screen.AddMessage(pl->GetName() + " committed suicide", pl->GetColor());
            return MAP_UPDATE_GAME_OVER;
        }
        for (auto &enemy : players) {
            if (enemy != pl) { //check collision with enemies
                if (enemy->CheckCollision(*pl)) {
                    ref_screen.AddMessage(enemy->GetName() + " win!", enemy->GetColor());
                    return MAP_UPDATE_GAME_OVER;
                }
            }
        }
    }

    return  MAP_UPDATE_OK;
}

void GameMap::DrawToBuffer() {
    if (!ref_screen.BufferInitialized()) {
        InitDraw();
        ref_screen.ChangeBufferState(true);
    }

    for (auto &pl : players)
        pl->DrawToBuffer(ref_screen);
}


CollisionType GameMap::CheckPlayerCollision(Player *player) {
    for (unsigned int i = 0; i < objects.size(); i++) {
        bool collision = player->CheckCollisionHead(objects[i]->GetCoord());
        if (collision && objects[i]->IsSolid())
            return CollisionType::COLLISION_SOLID;
        else if (collision && objects[i]->GetObjName() == "food") {
            delete objects[i];
            objects.erase(objects.begin() + i);
            return CollisionType::COLLISION_FOOD;
        }
    }
    return CollisionType::COLLISION_NONE;
}

void GameMap::AddFloorAt(const Point &point) {
    ref_screen.AddToBuffer(point, floorChar, COLOR_WHITE);
    AddEmptySpace(point);
}

void GameMap::GenerateFood() {
    try {
        int generate = rand() % 100;
        if (generate % 10 == 0) {
            Point foodCoord = GetRandEmptySpace();
            IGameObject *food = new Food(foodCoord);
            objects.push_back(food);
            ref_screen.AddToBuffer(foodCoord, food->GetPrintChar(), food->GetColorValue());
            //delete food;
        }
    }
    catch (...) {
        // here we dont need do anything, only we cant put some food
    }
}
void GameMap::RemoveEmptySpace(const Point &point) {
    auto it = find(emptySpaces.begin(), emptySpaces.end(), point);
    if (it != emptySpaces.end())
        emptySpaces.erase(it);
}

void GameMap::AddEmptySpace(const Point &point) {
    emptySpaces.push_back(point);
}

Point GameMap::GetRandEmptySpace() const {
    if (emptySpaces.size() == 0)
        throw "No empty space available";
    unsigned long index = rand() % emptySpaces.size();
    return emptySpaces[index];
}

const vector<Point> & GameMap::GetEmptySpaces() const {
    return emptySpaces;
}

bool GameMap::PathCollision(const list<Point> &path) const {
    for(auto it = path.begin(); it != path.end(); ++it){
        for(const auto & pl : players){
            if(pl->CheckCollision(*it))
                return true;
        }
    }
    return false;
}

Point GameMap::GetNearestFood(const Point &start) const {
    pair<Point, int> dist(Point(-1, -1), width * height); //dummy place if there is no food
    bool first = true;
    for (const auto &x : objects) {
        if (x->GetObjName() == "food") {
            Point food = x->GetCoord();
            int diffX = abs(start.x - food.x);
            int diffY = abs(start.y - food.y);
            int distance = (int) sqrt(diffX * diffX + diffY * diffY);

            if(first){
                dist = make_pair(food,distance);
                first = false;
            }
            else if (distance < dist.second) {
                dist = make_pair(food, distance);
            }
        }
    }
    if (dist.first == Point(-1, -1))
        throw "No food avaible";

    return dist.first;
}

bool GameMap::FoodStillExist(const Food &food) const {
    for (const auto &x : objects) {
        if (x->GetObjName() == "food" && x->GetCoord() == food.GetCoord()) {
            return true;
        }
    }
    return false;
}

int GameMap::GetWidth() const {
    return width;
}

int GameMap::GetHeight() const {
    return height;
}