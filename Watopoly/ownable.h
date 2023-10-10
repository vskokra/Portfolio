#ifndef _OWNABLE_H_
#define _OWNABLE_H_

#include <string>
#include "boardcell.h"
#include <memory>
#include <iostream>
class Player;
class GameBoard; 

class Ownable: public BoardCell{
    public:
        std::string dept;
        int cost;
        GameBoard *game;
        Ownable(std::string name, int cost,  std::shared_ptr<Player>owner , std::string dept, GameBoard *game);
        void buyProperty(std::shared_ptr <Player> p);
        void auction();
        virtual void doOperation(std::shared_ptr<Player> curr) override = 0;
        virtual int getImprovs() override = 0;
        virtual void resetImprovs() override = 0;
        virtual int getImprovCost() override = 0;
        virtual void setImprovs(int amt) override = 0;
        std::string propName;
        std::shared_ptr<Player>owner; // set up owner field
        bool isMortgage = false; //default val
        void setMortgage(bool x);
}; 
#endif
