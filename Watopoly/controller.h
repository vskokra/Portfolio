#ifndef __CONTROLLER_H
#define __CONTROLLER_H
#include <string>
#include <fstream>
#include <memory>
#include <vector>

using namespace std;

class GameBoard;
class Player;
class Display;

class Controller{
    GameBoard *game;
    vector<string> validCommands;
    bool testingMode;
    bool isValidCommand(string command);
    Display *d;
public:
    Controller(bool testingMode = false);
    ~Controller();
    void init(vector<shared_ptr<Player>> players);
    void loadGame(string filename);
    void saveGame(string filename);
    void roll();
    void bigFive();
    void assets(shared_ptr <Player> player);
    void all();
    void play(bool alreadyInit);
};
 #endif
