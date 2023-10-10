#include "player.h"
#include "gyms.h"
#include <iostream>

using namespace std;

Gym::Gym(string name, int cost, shared_ptr<Player> owner, GameBoard *gamelulu) : Ownable{name, cost, owner, "Gym", gamelulu} {}

int Gym::calcRent()
{
    // step1: player has to roll die
    int sum = game->dice->getVal();
    // step2: access all properties ownd by Cur Owner to check for monopoly
    shared_ptr<Player> curOwner = this->owner;
    int num = curOwner->ownedProps.at("Gyms"); // gives the number of properties owned by the curOwner in the same dept
    if (num == 2)
    {
        return sum * 10;
    }
    return sum * 4;
}

void Gym::doOperation(shared_ptr<Player> curr)
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
        // step1: Calculate the rent
        int amt = (*this).calcRent();
        // step 2:

        // check for less money
        if (amt > curr->getMoney())
        {

            cout << "Insufficient funds, raise money" << endl;
        }
        else
        {
            // add money to owner
            shared_ptr<Player> own = this->owner;
            own->money_add(amt);
            // sub money from curr
            curr->money_sub(amt);
        }
    }
}

int Gym::getImprovs() { return 0; }
void Gym::resetImprovs() {}
int Gym::getImprovCost() { return 0; }
void Gym::setImprovs(int amt) {}
