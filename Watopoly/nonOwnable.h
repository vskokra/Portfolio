#ifndef __NONOWNABLE_H_
#define __NONOWNABLE_H_

#include <iostream>
#include "boardcell.h"
#include "gameboard.h"

class NonOwnable: public BoardCell {
    int pos;
    GameBoard *game;
public:
    NonOwnable(int pos, GameBoard *game);
    void doOperation(shared_ptr <Player> curr) override;
};
/*
void function(Player *curr);
};

class GooseNesting: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class GoToTims: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class CoopFee: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class DCTimsline: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class SLC: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class Tuition: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class NeedlesHall: public NonOwnable {
    public:
        void function(Player *curr) override;
};

class CollectOSAP: public NonOwnable {
    public:
        void function(Player *curr) override;
};
*/
#endif
