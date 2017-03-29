CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic -Wno-long-long -O0 -ggdb -g
LIBS=-lncurses
EXEC=nymsapet
#find all source files, included sub-directories
src=$(wildcard src/*.cpp)
#generate all objects file from .cpp
obj=$(src:.cpp=.o)
#generate all dependency files
#dep=$(obj:.o=.d) 
all: compile doc

compile: $(EXEC)

run: $(EXEC)
	./snake examples/1.map
run2: $(EXEC)
	./snake examples/2.map

#include dependecies
#-include $(dep) 

$(EXEC): $(obj)	
	$(CXX) $^ $(LIBS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#deplist depends on .d file
#deplist: $(dep)

doc:
	doxygen Doxyfile

clean:
	rm -rf src/*.o src/*.d nymsapet doc src/objs
lines:
	wc -l src/*.cpp src/*.hpp
#generate dependency file for each .cpp file,not include system header
#%.d: %.cpp
#	$(CXX) -MM $< >$@

AiPlayerDriver.o: src/AiPlayerDriver.cpp src/AiPlayerDriver.hpp \
 src/IPlayerDriver.hpp src/Enums.hpp src/GameEngine.hpp src/MapConfig.hpp \
 src/Point.hpp src/GameMap.hpp src/IGameObject.hpp src/BufferPart.hpp \
 src/Player.hpp src/SnakePart.hpp src/GameScreen.hpp src/IScreen.hpp \
 src/Food.hpp
ClientScreen.o: src/ClientScreen.cpp src/ClientScreen.hpp src/IScreen.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp src/BufferPart.hpp \
 src/Enums.hpp src/ServerDataParser.hpp src/Constants.hpp \
 src/KeyboardPlayerDriver.hpp src/IPlayerDriver.hpp src/KeyboardSet.hpp \
 src/ServerManager.hpp src/ServerFactory.hpp src/MenuScreen.hpp
Food.o: src/Food.cpp src/Food.hpp src/IGameObject.hpp src/Point.hpp
GameEngine.o: src/GameEngine.cpp src/GameEngine.hpp src/MapConfig.hpp \
 src/Point.hpp src/MenuScreen.hpp src/IScreen.hpp src/BufferPart.hpp \
 src/MapConfigManager.hpp
GameMap.o: src/GameMap.cpp src/GameMap.hpp src/IGameObject.hpp \
 src/Point.hpp src/MapConfig.hpp src/BufferPart.hpp src/IPlayerDriver.hpp \
 src/Enums.hpp src/GameEngine.hpp src/Player.hpp src/SnakePart.hpp \
 src/GameScreen.hpp src/IScreen.hpp src/Food.hpp src/Wall.hpp \
 src/Constants.hpp
GameScreen.o: src/GameScreen.cpp src/GameScreen.hpp src/IScreen.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp src/BufferPart.hpp \
 src/IPlayerDriver.hpp src/Enums.hpp src/KeyboardPlayerDriver.hpp \
 src/KeyboardSet.hpp src/MenuScreen.hpp src/GameMap.hpp \
 src/IGameObject.hpp src/Player.hpp src/SnakePart.hpp src/Food.hpp \
 src/AiPlayerDriver.hpp src/SmartAiPlayerDriver.hpp \
 src/SmarterAiPlayerDriver.hpp src/Constants.hpp
IGameObject.o: src/IGameObject.cpp src/IGameObject.hpp src/Point.hpp
IScreen.o: src/IScreen.cpp src/IScreen.hpp src/GameEngine.hpp \
 src/MapConfig.hpp src/Point.hpp src/BufferPart.hpp
KeyboardPlayerDriver.o: src/KeyboardPlayerDriver.cpp \
 src/KeyboardPlayerDriver.hpp src/IPlayerDriver.hpp src/Enums.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp src/KeyboardSet.hpp
main.o: src/main.cpp src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp \
 src/MenuScreen.hpp src/IScreen.hpp src/BufferPart.hpp src/Constants.hpp
MapConfig.o: src/MapConfig.cpp src/MapConfig.hpp src/Point.hpp
MapConfigManager.o: src/MapConfigManager.cpp src/MapConfigManager.hpp \
 src/MapConfig.hpp src/Point.hpp src/Constants.hpp
MenuScreen.o: src/MenuScreen.cpp src/MenuScreen.hpp src/IScreen.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp src/BufferPart.hpp \
 src/GameScreen.hpp src/IPlayerDriver.hpp src/Enums.hpp \
 src/ServerScreen.hpp src/ServerPlayerDriver.hpp src/ServerDataParser.hpp \
 src/ServerManager.hpp src/Constants.hpp src/GameMap.hpp \
 src/IGameObject.hpp src/Player.hpp src/SnakePart.hpp src/Food.hpp \
 src/ClientScreen.hpp src/KeyboardPlayerDriver.hpp src/KeyboardSet.hpp
Player.o: src/Player.cpp src/Player.hpp src/SnakePart.hpp \
 src/IGameObject.hpp src/Point.hpp src/Enums.hpp src/IPlayerDriver.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/GameMap.hpp src/BufferPart.hpp \
 src/GameScreen.hpp src/IScreen.hpp src/Food.hpp src/Constants.hpp
Point.o: src/Point.cpp src/Point.hpp
ServerDataParser.o: src/ServerDataParser.cpp src/ServerDataParser.hpp \
 src/Enums.hpp src/BufferPart.hpp src/Point.hpp src/Constants.hpp
ServerFactory.o: src/ServerFactory.cpp src/ServerFactory.hpp \
 src/Constants.hpp
ServerManager.o: src/ServerManager.cpp src/ServerManager.hpp \
 src/Constants.hpp src/ServerDataParser.hpp src/Enums.hpp \
 src/BufferPart.hpp src/Point.hpp
ServerPlayerDriver.o: src/ServerPlayerDriver.cpp \
 src/ServerPlayerDriver.hpp src/IPlayerDriver.hpp src/Enums.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp
ServerScreen.o: src/ServerScreen.cpp src/ServerScreen.hpp src/IScreen.hpp \
 src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp src/BufferPart.hpp \
 src/Enums.hpp src/ServerPlayerDriver.hpp src/IPlayerDriver.hpp \
 src/ServerDataParser.hpp src/ServerManager.hpp src/Constants.hpp \
 src/GameMap.hpp src/IGameObject.hpp src/Player.hpp src/SnakePart.hpp \
 src/GameScreen.hpp src/Food.hpp src/ServerFactory.hpp src/MenuScreen.hpp
SmartAiPlayerDriver.o: src/SmartAiPlayerDriver.cpp \
 src/SmartAiPlayerDriver.hpp src/AiPlayerDriver.hpp src/IPlayerDriver.hpp \
 src/Enums.hpp src/GameEngine.hpp src/MapConfig.hpp src/Point.hpp \
 src/GameMap.hpp src/IGameObject.hpp src/BufferPart.hpp src/Player.hpp \
 src/SnakePart.hpp src/GameScreen.hpp src/IScreen.hpp src/Food.hpp
SmarterAiPlayerDriver.o: src/SmarterAiPlayerDriver.cpp \
 src/SmarterAiPlayerDriver.hpp src/AiPlayerDriver.hpp \
 src/IPlayerDriver.hpp src/Enums.hpp src/GameEngine.hpp src/MapConfig.hpp \
 src/Point.hpp src/GameMap.hpp src/IGameObject.hpp src/BufferPart.hpp \
 src/Player.hpp src/SnakePart.hpp src/GameScreen.hpp src/IScreen.hpp \
 src/Food.hpp
SnakePart.o: src/SnakePart.cpp src/SnakePart.hpp src/IGameObject.hpp \
 src/Point.hpp
Wall.o: src/Wall.cpp src/Wall.hpp src/IGameObject.hpp src/Point.hpp
