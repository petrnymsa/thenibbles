#ifndef MENUSCREEN_HPP
#define MENUSCREEN_HPP
#include "IScreen.hpp"
#include <string>
#include <vector>
#include <ncurses.h>

/**
 * Represents Menu with available options and the header
 */
class MenuScreen : public IScreen {
public:
    /**
     * Create new instance of MenuScreen
     * @param[in] engine Reference to GameEngine
     */
    MenuScreen(GameEngine & engine);
    /**
     * Destroy resources. Deletes ncurses windows.
     */
    virtual ~MenuScreen() override;
    /**
     * Initialize the main screen, option scren
     */
    virtual void Init() override;
    /**
     * Draw options, highlight select option
     */
    virtual void Draw() override;
    /**
     * Update selected option
     */
    virtual void Update() override;

private:
    /**
     * Nested enum represent one of menu options
     */
    enum MenuOptions{
        pvplocal=0, /**< Player vs Player */
        pvpnet=1, /**< Player vs Player over net */
        pvsai_random=2, /**< Player vs AI_RANDOM */
        pvsai_smart=3, /**< Player vs AI_SMART */
        pvsai_smarter=4, /**< Player vs AI_SMARTER */
        startServer=5, /**< Start server */
        exit=6 /**< Exit game */
    };
    std::vector<std::string> options; /**< Texts of avaible options */
    unsigned int selected; /**< Current selected option */
    WINDOW *win; /**<  Main ncurses window - draw box around it */
    WINDOW *headerWin; /**< NCurses window for draw header */
    WINDOW *menuWin; /**< NCurses windows for draw avaible options */
    bool headerPrinted; /**< Indicates if header was printed  */
    /**
     * Prints to screen formated header about game
     */
    void PrintHeader();
    /**
     * Print main menu
     */
    void PrintMenu();

    /**
     * Perform one of avaible option from menu.
     * It can call engine ChangeScreen() method, Exit() etc
     */
    void PerformAction();
};
#endif //MENUSCREEN_HPP
