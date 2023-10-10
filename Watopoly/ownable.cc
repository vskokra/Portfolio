#include <string.h>
#include "player.h"
#include "ownable.h"
#include "gameboard.h"
#include <algorithm>
using namespace std; 


//ctor 
Ownable::Ownable(string name, int cost,  shared_ptr<Player>owner, string dept, GameBoard *game): propName(name), dept{dept}, cost(cost), owner(owner), game{game}{}

void Ownable::buyProperty(shared_ptr <Player> p)
{
    p->money_sub(cost);
    owner = p;
    // int idx = game->currPlayer->getPosition();
    // shared_ptr <Ownable> newProp = dynamic_pointer_cast <Ownable> (game->gb[idx]);
    p->addProp(game->getOwnable());
    p->updateWorth(cost);
}


void Ownable::setMortgage(bool x){
    isMortgage = x;
    if(x){
    owner->updateWorth(-cost);
    owner->money_add(cost/2);
    cout<< "Mortgaged " << this->propName << endl;
    }
    else{
        owner->updateWorth(cost);
        owner->money_sub(cost*0.6);
        cout << "UnMortgaged " << this->propName << endl;
    }
    
}

void Ownable::auction(){
    vector<char> plr;
    for (int i = 0; i < game->player.size(); i++){
        plr.emplace_back(game->player[i]->playerChar);
    }
    int x = 0;
    char highest ='v';
    int currBid = 0;
    while (plr.size() != 1){
        char curr = plr[x];
        cout<<"Player char:" << curr << "\nThe Current Bid is $" << currBid <<". Press 0 to Withdraw, else enter amount to raise the bid by." << endl;
        while(true){
            int i;
            if(cin >> i){
                if ( i == 0 ){
                    plr.erase(remove(plr.begin(), plr.end(), curr), plr.end());
                    break;
                }
                else{
                    currBid += i;
                    highest = curr;
                    if(x == plr.size() - 1)
                    x = 0;
                    else
                    x++;
                    break;
                }
            }
        }
    }
    if(highest == 'v'){
         cout<<"Player char:" << plr[0] << "\nThe Current Bid is $" << currBid <<". Press 0 to Withdraw, else enter amount to raise the bid by." << endl;
          while(true){
            int i;
            if(cin >> i){
                if ( i == 0 ){
                    cout << "No one bought the property" << endl;
                    return;
                }
                else{
                    currBid = i;
                    highest = plr[0];
                }
            }
          }
    }
            shared_ptr <Player> p;
            for (int i = 0; i<game->player.size(); i ++){
            if(plr[0] == game->player[i]->playerChar){
                p = game->player[i];
            }
          }
          while (p->getMoney() < currBid){
                    if (game->bankrupt(p, currBid)){
                    game->declareBankrupt(p);
                    }
                    else{
                        cout<< "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell"<<endl;  
                    }
                }
                game->currPlayer->money_sub(currBid - cost);
                buyProperty(p);
    }