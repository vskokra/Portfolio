#ifndef _RES_H_
#define _RES_H_

#include "ownable.h"
#include "gameboard.h"
#include <memory>

//just pass in the ctor the cost as 200 ? 
class Res: public Ownable {
    int calcRent(); 
    public:
        Res(std::string name, int cost, std::shared_ptr<Player> owner, GameBoard *game);
        void doOperation(std::shared_ptr <Player> curr) override; 
        int getImprovs() override;
        void resetImprovs() override;
        int getImprovCost() override;
        void setImprovs(int amt) override;
        
} ;
#endif