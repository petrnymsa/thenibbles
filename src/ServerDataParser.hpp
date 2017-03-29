#ifndef SERVERDATAPARSER_HPP
#define SERVERDATAPARSER_HPP
#include "Enums.hpp"
#include "BufferPart.hpp"
#include <string>
#include <vector>

using namespace std;
/**
 * Helper class for parsing data from server
 */
class ServerDataParser {
public:
    /**
     * Parse incoming data and return type of response
     * @param[in] data Data to parse
     * @return Type of data
     */
    int GetDataType(const char * data);
    /**
     * Parse incoming data as message
     * @param[in] data Data to parse
     * @param[in] delimiter Delimiter of data
     * @return Parsed message with color
     */
    pair<string,short> ParseMessage(const char * data, char delimiter);
    /**
     * Parse incoming data as BufferPart
     * @param[in] data Data to parse
     * @param[in] delimiter Delimiter of data
     * @return Parsed buffer part
     */
    BufferPart ParseBufferPart(const char * data, char delimiter);
    /**
     * Parse incoming data as PlayerDirection
     * @param[in] data Data to parse
     * @param[in] delimiter Delimiter of data
     * @return Parsed player direction
     */
    PlayerDirection ParseDirection(const char * data, char delimiter);

    /**
     * Prepare type of response to send
     * @param[in] type Type of response
     * @return prepared data
     */
    string PrepareResponse(int type);
    /**
     * Prepare BufferPart to send
     * @param[in] part Buffer part
     * @param[in] delimiter Delimiter to split data
     * @return prepared data
     */
    string PrepareBufferPart(const BufferPart & part, char delimiter);
    /**
    * Prepare PlayerDirection to send
    * @param[in] direction PlayerDirection to prepare
    * @param[in] delimiter Delimiter to split data
    * @return prepared data
    */
    string PrepareDirectionUpdate(PlayerDirection direction, char delimiter);
    /**
    * Prepare message to send
    * @param[in] message Message with color to prepare
    * @param[in] delimiter Delimiter to split data
    * @return prepared data
    */
    string PrepareMessage(const pair<string,short> & message, char delimiter);

private:
    /**
     * Helper method to parse coordinates of point
     * @param[in] line Data to parse
     * @return parsed Point
     */
    Point ParsePoint(string line);
};
#endif //SERVERDATAPARSER_HPP
