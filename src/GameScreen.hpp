#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP


#include "IScreen.hpp"
#include "MapConfig.hpp"
#include "IPlayerDriver.hpp"
#include <ncurses.h>
#include <queue>
class GameMap;

/**
 * Represents main game screen, where is stored game world, players and so on
 */
class GameScreen : public IScreen {
public:
    /**
     * Create new instance of GameScreen
     * @param[in] engine Reference to GameEngine
     * @param[in] mode Currently set GameMode
     */
    GameScreen(GameEngine & engine, GameMode mode);
    /**
     * Destroys resources
     */
    virtual ~GameScreen() override;
    /**
     * Initializes ncurses Window, map configuraion, player's drivers
     */
    virtual void Init() override;
    /**
     * Draw map on screen, messages
     */
    virtual void Draw() override;
    /**
     * Update map, players, messages
     */
    virtual void Update() override;
    /**
     * Set state to GameOver
     */
    void GameOver();

protected:
    GameMode gameMode; /**<  Currently set GameMode */
    GameMap * map;/**< Currently played map */
    WINDOW * mapWindow; /**< Main map window */
    WINDOW * msgWindow;/**< Message window */
    bool isGameOver; /**< Indicates if GameOver state is set */
    bool isPause; /**< Indicates if game is paused or not */
    bool mapLoaded;/**< Indicates if map correctly lodead */

    void GameReset();
};


#endif //GAMESCREEN_HPP
