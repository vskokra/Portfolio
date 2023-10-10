#ifndef __GAMEBOARD_H_
#define __GAMEBOARD_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "boardcell.h"
#include "player.h"
#include "ownable.h"
#include "improvable.h"
#include "nonOwnable.h"
#include "dice.h"
//why can't we include std namespace in .h files 

using namespace std;

class GameBoard
{
    // Store player in order of turn 
    // char *playerChar[6]; // Store in order of turn/player
public:
    std::vector <shared_ptr<BoardCell>> gb; // Store in order of occurence on board with CollectOSAP as 0;
    vector <shared_ptr<Player>> player; 
    std::unordered_map <std::string, std::vector<int>> tuitionChart;
    GameBoard(vector<shared_ptr <Player>> plr);
    void roll();
    void next();
//    void buyProperty(Player *player, Ownable *prop);
//    void sellProperty(Player *player, Ownable *prop);
//    void mortgage(Player *player, Ownable *prop);
//    void unmortgage(Player *player, Ownable *prop);
//    void improve_buy(Player *player, Improvable *prop);
//    void improve_sell(Player *player, Improvable *prop);
//    void trade(Player *player1, Ownable *prop1, int *money1, Player *player2, Ownable *prop2, int *money2);
//    void auction(Ownable *prop);
    bool bankrupt(shared_ptr<Player>p, int amount); // autoChecker
    void declareBankrupt(shared_ptr<Player> p);
    void basicFive(shared_ptr <Player> p);
    shared_ptr <Player> currPlayer;
    Dice *dice;
    bool rollComplete;
    int rimCupCount;
    shared_ptr<Ownable> getOwnable();
    void declareBankrupt(shared_ptr<Player> pay, shared_ptr<Player> collect);
    void improveBuy(shared_ptr <Player> p);
    void improveSell(shared_ptr <Player> p);
    void trade();
    void trade(string prop1, string prop2, string name);
    void trade(int amt, string prop, string name);
    void trade(string prop, int amt,  string name);
    void unmortgage(shared_ptr <Player> p);
    void mortgage(shared_ptr <Player> p);
    std::unordered_map<std::string, int> propDictionary;
    void rolltest();
};
#endif
