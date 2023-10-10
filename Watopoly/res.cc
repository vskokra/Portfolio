#include "res.h"
#include "player.h"
#include <iostream>
#include <string> 
using namespace std;


Res::Res(string name, int cost, shared_ptr<Player>owner, GameBoard *game) : Ownable{name, cost, owner, "Res", game} {}

int Res::calcRent(){

    shared_ptr<Player> curOwner = this->owner;
    int numOwned = curOwner->ownedProps.at("Res"); // gives the number of properties owned by the curOwner in the same dept
    return 25 * numOwned; 
    
}

void Res::doOperation(shared_ptr<Player>curr)

{

    cout << " You are on " << propName << endl;
    // check 1 - if property has no owner
    if (this->owner == nullptr)
    {
        cout << "No property Owner" << endl;
        if (curr->getMoney() < this->cost)
        {
            auction();
            return;
        }
        cout << "Options\n1: buy\n2: auction\n";
        int n;
        cin >> n;
        if (n == 1)
        {
            buyProperty(curr);
        }
        else if (n == 2)
        {
            auction();
        }
    }
    else if (this->owner == curr)
    {
        return;
    }
    else
    {

    //step 1 : calculate the rent 
    int amt = (*this).calcRent(); 

    //step 2:

    //check for less money 
    if (amt > curr->getMoney()){
    
        cout << "Insufficient funds, raise money" << endl;
        
    } else {
        //add money to owner 
       shared_ptr<Player>own = this->owner;
        own->money_add(amt);
        //sub money from curr 
        curr->money_sub(amt);
    }
    cout << "You paid " << amt << " as rent on" << propName;
    }
}

int Res::getImprovs() { return 0; }
void Res::resetImprovs() {}
int Res::getImprovCost() {return 0;}
void Res::setImprovs(int amt) {}
