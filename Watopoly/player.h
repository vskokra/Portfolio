#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "ownable.h"
using namespace std;
class Player{
    std::string playerName;
    int position;
    int money;
    int propVal;
    bool Tims;
    int rollsTims;
    int rimCups;
public:
    char playerChar;
    std::unordered_map <string, int> ownedProps;
    std::vector <shared_ptr<Ownable>> playerProps;
    Player(std::string playerName, char playerChar);
    void addProp(shared_ptr<Ownable> newProp);
    void subProp(shared_ptr<Ownable> newProp);
    int getPosition(); // gives players position
    void setPosition(int pos); //sets new position
    int getMoney(); // gives info of money
    void money_add(unsigned int val);
    void money_sub(unsigned int val);
    void goToTims(bool x);
    int getRimCups();
    void setRimCups(int n);
    bool isTims();
    void addRollsTims();
    void resetRollsTims();
    int getRollsTims();
    int getWorth();
    void updateWorth(int amount);
    string getName();

};

#endif
