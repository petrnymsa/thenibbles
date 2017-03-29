#ifndef BUFFERPART_HPP
#define BUFFERPART_HPP

#include "Point.hpp"

/**
 * Helper class for buffer.
 * It contains screen position, character to print and color of text.
 */
struct BufferPart{
    const Point point; /**< Screen coordinates */
    const char ch; /**< Character to print */
    const short color; /**< Color of character */

    /**
     * Create new instance of Buffer part
     * @param[in] n_point Screen coordinates
     * @param[in] w Character to print
     * @param[in] n_color Color of character
     */
    BufferPart(const Point & n_point, char w, short n_color)
            :point(n_point),ch(w),color(n_color){

    }
};

#endif //BUFFERPART_HPP
