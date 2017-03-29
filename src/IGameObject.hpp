#ifndef IGameObject_HPP
#define IGameObject_HPP
#include "Point.hpp"
#include <string>
using namespace std;
/**
 * Represents abstract game object
 * Holds information about position, character and color
 */
class IGameObject {
public:
    /**
   * Position of object.
   * It is set as public, because other objects can change coordinates and also get informations about it.
   */
    Point coord;

    /**
     * Create new instace of IGameObject
     * @param[in] point Position of object
     */
    IGameObject(const Point & point);
    /**
     * Destroys resources
     */
    virtual ~IGameObject();
    /**
     * Get Color
     * @return Color value
     */
    short GetColorValue(void) const;
    /**
     * Get character
     * @return Printable character
     */
    char GetPrintChar(void) const;
    /**
     * Get internal name of object
     */
    string GetObjName(void) const;
    /**
     * Return position of object
     * @return Position of object
     */
    const Point & GetCoord(void) const;

//    void SetX(int x);
  //  void SetY(int y);
    /**
     * Check collision with object and given coordinates of another object
     * @param[in] x Coordinates of another object
     * @return True if objects colidates
     */
    virtual bool CheckCollision(const Point & x) const;
    /**
     * Check if object is solid or not
     * @return true if object is solid, otherwise false
     */
    virtual bool IsSolid() const = 0;
    /**
     * Create clone of IGameObject object
     * @return Clonned object
     */
    virtual IGameObject * Clone() const = 0;
protected:
    short color; /**< Color value of object */
    char  printChar; /**< Printable character */
    string objName;/**< Internal object name */
};


#endif //IGameObject_HPP
