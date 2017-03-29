#ifndef KEYBOARDSET_HPP
#define KEYBOARDSET_HPP

/**
 * Represents settings of player's keyboard's input.
 */
struct KeyboardSet{
public:
    const int KEY_SET_UP; /**< Key to update direction UP */
    const int KEY_SET_DOWN;/**< Key to update direction DOWN */
    const int KEY_SET_LEFT;/**< Key to update direction LEFT */
    const int KEY_SET_RIGHT;/**< Key to update direction RIGHT */
    /**
     * Create new instance of KeyBoard set
     * @param[in] up Key to update direction UP
     * @param[in] left Key to update direction LEFT
     * @param[in] down Key to update direction DOWN
     * @param[in] right Key to update direction RIGHT
     */
    KeyboardSet(int up, int left, int down, int right)
            :KEY_SET_UP(up),KEY_SET_DOWN(down),KEY_SET_LEFT(left),KEY_SET_RIGHT(right){

    }
};

#endif //KEYBOARDSET_HPP
