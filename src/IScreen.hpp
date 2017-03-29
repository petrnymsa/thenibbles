#ifndef IScreen_HPP
#define IScreen_HPP
#include <ncurses.h>
#include <queue>
#include "GameEngine.hpp"
#include "BufferPart.hpp"

/**
 * Abstract class that represents an game screen which allow update and draw screen.
 */
class IScreen {
public:
    GameEngine & engine; /**< Reference to main game engine class */
    /**
     * Create new instance of abstract IScreen class
     * @param[in] gameEngine reference to GameEngine class
     */
    IScreen(GameEngine & gameEngine);
    /**
     * Destroy resources
     */
    virtual ~IScreen();

    /**
     * Initialize game screen
     */
    virtual void Init() = 0;
    /**
     * Draw changes to screen
     */
    virtual void Draw() = 0;
    /**
     * Update state of screen
     */
    virtual void Update() =0;
    /**
     * Add message to display
     * @param[in] msg Message to display
     * @param[in] color Color of message
     */
    virtual void AddMessage(const string & msg, short color = COLOR_WHITE);
    /**
     * Add change to screen buffer
     * @param[in] point Coordinates at screen
     * @param[in] printChar Character to print
     * @param[in] color Color of character
     */
    virtual void AddToBuffer(const Point & point, char printChar, short color);

    /**
    * Add change to screen buffer
    * @param[in] part BufferPart to add
    */
    virtual void AddToBuffer(const BufferPart & part);
    /**
     * @return If buffer is initialized or not
     */
    bool BufferInitialized() const;
    /**
     * Change state of buffer
     * @param[in] state State of buffer
     */
    void ChangeBufferState(bool state);

protected:
    /**
     * Clear message window and messages
     * @param[in] win NCurses window
     */
    void ClearMsgWin(WINDOW * win);
    /**
     * Draw messages to given window
     * @param[in] win NCurses window where to draw
     */
    virtual void DrawMessages(WINDOW * win);
    /**
     * Update messagess and message time
     * @param[in] win NCurses window
     */
    virtual void UpdateMessages(WINDOW * win);
    /**
     * Draw buffer changes to given window
     * @param[in] win NCurses window where to draw
     */
    virtual void DrawBuffer(WINDOW * win);
    int msgDelay;/**< Delay of every message */
    int msgTime;/**< Helper variable to hold information about dealy of message */
    queue<pair<string, short> > messages;/**< Messages to display */
    pair<string, short> currentMsg;/** Current schoosen message */
    queue<BufferPart> drawBuffer;/**< Buffer changes to draw */
    bool bufferActive;/**< Indicates if initialization of buffer was performed */
};

#endif //IScreen_HPP
