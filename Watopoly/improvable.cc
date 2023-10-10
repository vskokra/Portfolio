#include "improvable.h"
#include <string>
#include "player.h"
#include <iostream>

using namespace std;

//ctor for improvable
Improvable::Improvable(string name, int cost, shared_ptr<Player>owner, string dept, int improvCost, GameBoard *game): Ownable{name, cost,owner, dept,  game}, improvCost{improvCost}{}


int Improvable::calcTuition(){

    // step 1: check if prop is in monopoly
    shared_ptr<Player>curOwner = this->owner;
    string deptOfProp = this->dept;
    cout << deptOfProp << endl;
    int numProps = curOwner->ownedProps[deptOfProp];

    // in monopoly
    // step2:
    if (((deptOfProp == "Math" || deptOfProp == "Arts1") && numProps == 2) || numProps == 3)
        {
            
            if (this->improvs == 0)
            {
                return 2 * game->tuitionChart.at(this->propName)[0];
            }

            return game->tuitionChart.at(this->propName)[this->improvs];
        }
    return game->tuitionChart.at(this->propName)[0];
}


void Improvable::doOperation(shared_ptr<Player> curr)
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
    // step1- get the amount
    int amt = (*this).calcTuition();

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
    cout << "You paid " << amt << " as rent on" << propName;
    }
}

int Improvable::getImprovs(){
    return improvs; 
}

void Improvable::resetImprovs(){
    improvs = 0;
}

int Improvable::getImprovCost(){
    return improvCost;
}

void Improvable::setImprovs(int amt){
    improvs = amt;
    cout << "Improvs set to " << amt << " For Property" << this->propName << endl;
}
