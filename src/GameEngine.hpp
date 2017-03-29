#ifndef GameEngine_HPP
#define GameEngine_HPP
#include <ctime>
#include <string>
#include "MapConfig.hpp"
class IScreen;
using namespace std;
/**
 * Represents main object where is stored main logic of game, like Update or Draw methods
 */
class GameEngine {
public:
    /**
     * Create new instance of GameEngine
     * @param[in] mapConfigFile Name of configuration file
     * @param[in] serverAddr Address of server
     * @param[in] port Port of server
     * @param[in] maxPlayers Max of clients at server
     */
    GameEngine(const string & mapConfigFile, string serverAddr, string port, int maxPlayers);
    /**
     * Destroys object's resources
     */
    ~GameEngine();
    /**
     * Update current game screen. Also update keyboard input.
     */
    void Update();
    /**
     * Draw current game screen.
     */
    void Draw();
    /**
     * Sleep engine for given seconds
     * @param[in] sec Sleep time
     */
    void Sleep(clock_t sec);
    /**
     * CHange current screen to another.
     * Current screen is properly destroyed.
     * @param[in] screen Screen to change
     */
    void ChangeScreen(IScreen * screen);
    /**
     * Change engine state IsRunning to false
     */
    void Exit();
    /**
     * Change engine state IsRunning to true
     */
    void Start();
    /**
     * Check if engine is running or not
     * @return True if engine running, otherwise false
     */
    bool IsRunning() const;
    /**
     * Check if engine is set as crashed
     * @return True if engine crashed.
     */
    bool IsCrashState() const;
    /**
     * Return currenty pressed key
     * @return int key code
     */
    int GetKeyInput() const;

    /**
     * If something was totally wrong, display information to user
     * Also end engine and set crash status
     * @param[in] msg Message to display.
     */
    void DisplayCrash(const char * msg);
    /**
     * Change if engine can sleep or not. Server need to turn off this feature
     * @param[in] state True - can sleep, False - can't sleep
     */
    void ChangeSleepState(bool state);

    /**
     * Change if engine read curses input every update frame or not
     * @param[in] state True - read every update, False - read only when GetKeyInput is called
     */
    void AutoInput(bool state);

    /**
     * Return configuration of map
     * @return MapConfig instance
     */
    MapConfig & GetMapConfig();
    const string mapConfigFileName; /**< Path to map configuration file */
    const string serverName;/**< Server address - default "localhost" */
    const string serverPort;/**< Server port - default 1666 */
    const int    maxClients;/**< Server max clients (players) - default 2 */
private:
    /**
     * Current active screen
     */
    IScreen * screen;
    bool isRunnig; /**< Running state of engine */
    bool isCrashState; /**< Crash state of engine */
    int keyInput; /**< Currently pressed key */
    bool isSleepAllowed; /**< Engine can turn off sleeping */
    bool isAutoInput;/**< Engine state of auto-check input */
    MapConfig mapConfig; /**< Configuration of map */
};
#endif //GameEngine_HPP
