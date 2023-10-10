#ifndef _BOARDCELL_H_
#define _BOARDCELL_H_
#include <memory>
class GameBoard; 
class Player; 


//Abstract Class 
class BoardCell{
    
    public: 
        virtual void doOperation(std::shared_ptr<Player>curr) = 0;
        virtual int getImprovs(); // return 0 by default
        virtual void resetImprovs();
        virtual void setImprovs(int amt);
        virtual int getImprovCost();
    };
#endif 
