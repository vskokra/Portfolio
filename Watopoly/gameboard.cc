#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include "improvable.h"
#include "res.h"
#include "gameboard.h"
#include "gyms.h"
using namespace std;

GameBoard::GameBoard(vector<shared_ptr<Player>> plr)
{

    // in MIl ask for player array
    dice = new Dice();
    rimCupCount = 0;
    for (int i = 0; i < plr.size(); ++i)
    {
        player.emplace_back(plr[i]);
    }
    currPlayer = player[0];
    // board created
    gb.emplace_back(make_shared<NonOwnable>(0, this)); // OSAP
    gb.emplace_back(make_shared<Improvable>("AL", 40, nullptr, "Arts1", 50, this));
    gb.emplace_back(make_shared<NonOwnable>(2, this)); // SLC
    gb.emplace_back(make_shared<Improvable>("ML", 60, nullptr, "Arts1", 50, this));
    gb.emplace_back(make_shared<NonOwnable>(4, this)); // TUITION
    gb.emplace_back(make_shared<Res>("MKV", 200, nullptr, this));
    gb.emplace_back(make_shared<Improvable>("ECH", 100, nullptr, "Arts2", 50, this));
    gb.emplace_back(make_shared<NonOwnable>(7, this)); // NEEDLES
    gb.emplace_back(make_shared<Improvable>("PAS", 100, nullptr, "Arts2", 50, this));
    gb.emplace_back(make_shared<Improvable>("HH", 120, nullptr, "Arts2", 50, this));
    gb.emplace_back(make_shared<NonOwnable>(10, this)); // DC
    gb.emplace_back(make_shared<Improvable>("RCH", 140, nullptr, "Eng", 100, this));
    gb.emplace_back(make_shared<Gym>("PAC", 150, nullptr, this));
    gb.emplace_back(make_shared<Improvable>("DWE", 140, nullptr, "Eng", 100, this));
    gb.emplace_back(make_shared<Improvable>("CPH", 160, nullptr, "Eng", 100, this));
    gb.emplace_back(make_shared<Res>("UWP", 200, nullptr, this));
    gb.emplace_back(make_shared<Improvable>("LHI", 180, nullptr, "Health", 100, this));
    gb.emplace_back(make_shared<NonOwnable>(17, this)); // SLC
    gb.emplace_back(make_shared<Improvable>("BMH", 180, nullptr, "Health", 100, this));
    gb.emplace_back(make_shared<Improvable>("OPT", 200, nullptr, "Health", 100, this));
    gb.emplace_back(make_shared<NonOwnable>(20, this)); // Goose
    gb.emplace_back(make_shared<Improvable>("EV1", 220, nullptr, "Env", 150, this));
    gb.emplace_back(make_shared<NonOwnable>(22, this)); // NEEDLES
    gb.emplace_back(make_shared<Improvable>("EV2", 220, nullptr, "Env", 150, this));
    gb.emplace_back(make_shared<Improvable>("EV3", 240, nullptr, "Env", 150, this));
    gb.emplace_back(make_shared<Res>("V1", 200, nullptr, this));
    gb.emplace_back(make_shared<Improvable>("PHYS", 260, nullptr, "Sci1", 150, this));
    gb.emplace_back(make_shared<Improvable>("B1", 260, nullptr, "Sci1", 150, this));
    gb.emplace_back(make_shared<Gym>("CIF", 150, nullptr, this));
    gb.emplace_back(make_shared<Improvable>("B2", 280, nullptr, "Sci1", 150, this));
    gb.emplace_back(make_shared<NonOwnable>(30, this)); // GOTOTIMS
    gb.emplace_back(make_shared<Improvable>("EIT", 300, nullptr, "Sci2", 200, this));
    gb.emplace_back(make_shared<Improvable>("ESC", 300, nullptr, "Sci2", 200, this));
    gb.emplace_back(make_shared<NonOwnable>(33, this)); // SLC
    gb.emplace_back(make_shared<Improvable>("C2", 320, nullptr, "Sci2", 200, this));
    gb.emplace_back(make_shared<Res>("Rev", 200, nullptr, this));
    gb.emplace_back(make_shared<NonOwnable>(36, this)); // NEEDLES
    gb.emplace_back(make_shared<Improvable>("MC", 350, nullptr, "Math", 200, this));
    gb.emplace_back(make_shared<NonOwnable>(38, this)); // COOP
    gb.emplace_back(make_shared<Improvable>("DC", 400, nullptr, "Math", 200, this));

    // Initialize the Monopoly dictionary
    // unordered_map<string, vector<int>> tuitionChart;
    // each index consists price of the buildings
    tuitionChart["ML"] = vector<int>{4, 20, 60, 180, 320, 450};
    tuitionChart["AL"] = vector<int>{2, 10, 30, 90, 160, 250};
    tuitionChart["HH"] = vector<int>{8, 40, 100, 300, 450, 600};
    tuitionChart["ECH"] = vector<int>{6, 30, 90, 270, 400, 550};
    tuitionChart["PAS"] = vector<int>{6, 30, 90, 270, 400, 550};
    tuitionChart["RCH"] = vector<int>{10, 50, 150, 450, 625, 750};
    tuitionChart["DWE"] = vector<int>{10, 50, 150, 450, 625, 750};
    tuitionChart["CPH"] = vector<int>{12, 60, 180, 500, 700, 900};
    tuitionChart["LHI"] = vector<int>{14, 70, 200, 550, 750, 950};
    tuitionChart["BMH"] = vector<int>{14, 70, 200, 550, 750, 950};
    tuitionChart["OPT"] = vector<int>{16, 80, 220, 600, 800, 950};
    tuitionChart["EV1"] = vector<int>{18, 90, 250, 700, 875, 1050};
    tuitionChart["EV2"] = vector<int>{18, 90, 250, 700, 875, 1050};
    tuitionChart["EV3"] = vector<int>{20, 100, 300, 750, 925, 1100};
    tuitionChart["PHYS"] = vector<int>{22, 110, 330, 800, 975, 1150};
    tuitionChart["B1"] = vector<int>{22, 110, 330, 800, 975, 1150};
    tuitionChart["B2"] = vector<int>{24, 120, 360, 850, 1025, 1200};
    tuitionChart["EIT"] = vector<int>{26, 130, 390, 900, 1100, 1275};
    tuitionChart["ESC"] = vector<int>{26, 130, 390, 900, 1100, 1275};
    tuitionChart["C2"] = vector<int>{28, 150, 450, 1000, 1200, 1400};
    tuitionChart["MC"] = vector<int>{35, 175, 500, 1100, 1300, 1500};
    tuitionChart["DC"] = vector<int>{50, 200, 600, 1400, 1700, 2000};

    // gives the pos
    propDictionary["ML"] = 3;
    propDictionary["AL"] = 1;
    propDictionary["HH"] = 9;
    propDictionary["ECH"] = 6;
    propDictionary["PAS"] = 8;
    propDictionary["RCH"] = 11;
    propDictionary["DWE"] = 13;
    propDictionary["CPH"] = 14;
    propDictionary["LHI"] = 16;
    propDictionary["BMH"] = 18;
    propDictionary["OPT"] = 19;
    propDictionary["EV1"] = 21;
    propDictionary["EV2"] = 23;
    propDictionary["EV3"] = 24;
    propDictionary["PHYS"] = 26;
    propDictionary["B1"] = 27;
    propDictionary["B2"] = 29;
    propDictionary["EIT"] = 31;
    propDictionary["ESC"] = 32;
    propDictionary["C2"] = 34;
    propDictionary["MC"] = 37;
    propDictionary["DC"] = 39;
    propDictionary["PAC"] = 12;
    propDictionary["CIF"] = 28;
    propDictionary["MKV"] = 5;
}

void GameBoard::roll()
{
    int roll_count = 0;
    while ((roll_count < 3 && dice->isDouble()) || roll_count == 0)
    { // checks if number of rolls exceed 3
        if(currPlayer->isTims()){
            if(currPlayer->getRimCups() == 0){
            cout << "Press 0 to roll, 1 to pay $50 to get out of jail" << endl;
            int j;
            cin >> j;
            if(j == 1){
                while (currPlayer->getMoney() < 50)
                {
                    if (bankrupt(currPlayer, 50))
                    {
                        declareBankrupt(currPlayer);
                    }
                    else
                    {
                        cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                    }
                }
                currPlayer->money_sub(50);
                currPlayer->goToTims(false);
                currPlayer->resetRollsTims();
            }
            }
            else{
                cout << "Press 0 to roll, Press 1 to pay $50 or Press 2 to use Roll Up the RIm Cup to get out of Tims Line" << endl;
                int i;
                while (true)
                {
                    cin >> i;
                    if (cin)
                    {
                        if (i == 1)
                        {
                            while (currPlayer->getMoney() < 50)
                            {
                                if (bankrupt(currPlayer, 50))
                                {
                                    declareBankrupt(currPlayer);
                                }
                                else
                                {
                                    cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                                }
                            }
                            currPlayer->money_sub(50);
                            currPlayer->resetRollsTims();
                            break;
                        }
                        if (i == 2)
                        {
                            currPlayer->setRimCups(currPlayer->getRimCups() - 1); // Should update the card set (HOW)
                            currPlayer->goToTims(false);
                            currPlayer->resetRollsTims();
                            break;
                        }
                    }
                }
            }
        }
                    dice->setVal();
                    if (currPlayer->isTims() && !(dice->isDouble()) && currPlayer->getRollsTims() == 2)
                    { // checks if in jail and is 3rd turn
                        if (currPlayer->getRimCups() == 0)
                        { // checks if has rimCups
                            while (currPlayer->getMoney() < 50)
                            {
                                if (bankrupt(currPlayer, 50))
                                {
                                    declareBankrupt(currPlayer);
                                }
                                else
                                {
                                    cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                                }
                            }
                            currPlayer->money_sub(50);
                            currPlayer->goToTims(false);
                            currPlayer->resetRollsTims();
                        }
                        else
                        {
                            cout << "Press 1 to pay $50 or Press 2 to use Roll Up the RIm Cup to get out of Tims Line" << endl;
                            int i;
                            while (true)
                            {
                                cin >> i;
                                if (cin)
                                {
                                    if (i == 1)
                                    {
                                        while (currPlayer->getMoney() < 50)
                                        {
                                            if (bankrupt(currPlayer, 50))
                                            {
                                                declareBankrupt(currPlayer);
                                            }
                                            else
                                            {
                                                cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                                            }
                                        }
                                        currPlayer->money_sub(50);
                                        currPlayer->resetRollsTims();
                                        break;
                                    }
                                    if (i == 2)
                                    {
                                        currPlayer->setRimCups(currPlayer->getRimCups() - 1); // Should update the card set (HOW)
                                        currPlayer->goToTims(false);
                                        currPlayer->resetRollsTims();
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if (currPlayer->isTims() && !(dice->isDouble()))
                    {
                        currPlayer->addRollsTims();
                        break;
                    }
                    else if (currPlayer->isTims() && dice->isDouble())
                    {
                        currPlayer->resetRollsTims();
                        currPlayer->goToTims(false);
                    }
                    ++roll_count;
                    if (roll_count == 3 && dice->isDouble())
                    {
                        currPlayer->setPosition(10);
                        currPlayer->goToTims(true);
                        break;
                    }
                    int oldPosition = currPlayer->getPosition();
                    int sum = (dice->getVal() + oldPosition) % 40;
                    currPlayer->setPosition(sum);
                    int newPosition = currPlayer->getPosition();
                    if (newPosition < oldPosition)
                    { // Implementation for crossing OSAP
                        currPlayer->money_add(200);
                    }
                    gb[currPlayer->getPosition()]->doOperation(currPlayer);
                }
}

bool isNumber(const string &str)
{
    for (char const &c : str)
    {
        if (isdigit(c) == 0)
        {
            return false;
        }
    }
    return true;
}

void GameBoard::basicFive(shared_ptr<Player> p)
{
    // trade
    // mortgage
    // unmortgage
    // improve
    // next
    int n;
    cout << "Enter the number based on the Options\n1: trade\n2: mortgage\n3: unmortgage\n4: buy improvement\n5: sell improvement\n6: next\n";
    while (cin >> n)
    {
        if (n == 1)
        {
            this->trade();
        }
        else if (n == 2)
        {
            this->mortgage(p);
        }
        else if (n == 3)
        {
            this->unmortgage(p);
        }
        else if (n == 4)
        {
            this->improveBuy(p);
        }
        else if (n == 5)
        {
            this->improveSell(p);
        }
        else if (n == 6)
        {
            next();
            break;
        }
        else
        {
            cout << "Invalid option entered\n";
        }
        cout << "Options\n1: trade\n2: mortgage\n3: unmortgage\n4: buy improvement\n5: sell improvement\n6: next\n";
    }
}

void GameBoard::next()
{
    int flag = false;
    for (int x = 0; x < player.size() - 1; ++x)
    {
        if (player[x] == currPlayer)
        {
            flag = true;
            currPlayer = player[x + 1];
            break;
        }
    }
    if (!flag)
        currPlayer = player[0];
}

bool GameBoard::bankrupt(shared_ptr<Player> p, int amount)
{
    if (p->getMoney() + p->getWorth() / 2 < amount)
    {
        return true;
    }
    return false;
}

void GameBoard::declareBankrupt(shared_ptr<Player> p)
{
    p->money_sub(p->getMoney());
    for (int i = 0; i < p->playerProps.size(); i++)
    {
        p->playerProps[i]->owner = nullptr;
        p->playerProps[i]->resetImprovs();
        p->playerProps[i]->setMortgage(false);
    }
    player.erase(remove(player.begin(), player.end(), p), player.end());
}

void GameBoard::declareBankrupt(shared_ptr<Player> pay, shared_ptr<Player> collect)
{
    pay->money_sub(pay->getMoney());
    collect->money_add(pay->getMoney());
    for (int i = 0; i < pay->playerProps.size(); i++)
    {
        pay->playerProps[i]->owner = collect;
        pay->playerProps[i]->resetImprovs();
        collect->playerProps.emplace_back(pay->playerProps[i]);
        if (pay->playerProps[i]->isMortgage)
        {
            int tax = 0.1 * pay->playerProps[i]->cost;
            while (collect->getMoney() < tax)
            {
                if (bankrupt(collect, tax))
                {
                    declareBankrupt(collect);
                }
                else
                {
                    cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                }
            }
            collect->money_sub(tax);
            cout << "Press 1 if you would like to unmortgage " << pay->playerProps[i]->propName << endl;
            int i;
            if (cin >> i)
            {
                if (i == 1)
                {
                    tax = 0.5 * pay->playerProps[i]->cost;
                    while (collect->getMoney() < tax)
                    {
                        if (bankrupt(collect, tax))
                        {
                            declareBankrupt(collect);
                        }
                        else
                        {
                            cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                        }
                    }
                    collect->money_sub(tax);
                    pay->playerProps[i]->setMortgage(false);
                }
            }
        }
    }
    // player.erase(remove(player.begin(), player.end(), pay),player.end());
}

shared_ptr<Ownable> GameBoard::getOwnable()
{
    return dynamic_pointer_cast<Ownable>(gb[currPlayer->getPosition()]);
}

void GameBoard::improveBuy(shared_ptr<Player> p)
{
    string propName;
    cout << "Enter property that you want to improve\n";
    cin >> propName;
    shared_ptr <Ownable> improvable = nullptr;
    for(int x = 0; x < p->playerProps.size(); x ++){
        if(p->playerProps[x]->propName == propName){
            improvable = dynamic_pointer_cast<Ownable>(p->playerProps[x]);
            cout << improvable->dept << " HERE\n";
            break;
        }
    }
    //int index = propDictionary[propName];
    // shared_ptr <Ownable> ownable = dynamic_pointer_cast<Ownable>(gb[index]);
    //shared_ptr<Improvable> improvable = dynamic_pointer_cast<Improvable>(gb[index]);
    if (improvable->owner != p)
    {
        cout << "You cannot improve, since you are not the owner\n";
        return;
    }
    // int x;
    /*cout << "Enter 1: improve buy\nEnter 2: improve sell\n";
    cin >> x;*/
    string deptName = improvable->dept;
    cout<<deptName<<endl;
    int ownedCount = p->ownedProps[deptName];
    //  if (x == 1) {
    if (((deptName == "Math" || deptName == "Arts1") && ownedCount == 2) || ownedCount == 3)
    {
        /*for (int i = 0; i < gb.size(); i++)
        {
            shared_ptr<Ownable> prop = dynamic_pointer_cast<Ownable>(gb[i]);
            cout<< prop->dept << "HERE \n";
            if (prop->dept == improvable->dept)
            {
                if (prop->isMortgage == true)
                {
                    cout << "There are mortgaged properties in the monopoly" << endl;
                    return;
                }
            }
        }*/

         for(int x = 0; x < p->playerProps.size(); x ++){
            if(p->playerProps[x]->dept == deptName){
                if(p->playerProps[x]->isMortgage){
                    cout << "There are mortgaged properties in the monopoly" << endl;
                    return;
                }
            }
        }
        if (improvable->getImprovs() < 5 && p->getMoney() > improvable->getImprovCost())
        {
            improvable->setImprovs((improvable->getImprovs()) + 1);
            p->money_sub(improvable->getImprovCost());
            p->updateWorth(improvable->getImprovCost());
        }
        else
        {
            cout << "CANNOT BUY IMPROVEMENTS RIGHT NOW" << endl;
        }
        // improv_buy(currPlayer, improvable);
    }
    else
    {
        cout << "You cannot buy improvements because it is not part of a monopoly\n";
    }
}

void GameBoard::improveSell(shared_ptr<Player> p)
{
    string propName;
    cout << "Enter property that you want to improve\n";
    cin >> propName;
    int index = propDictionary[propName];
    // shared_ptr <Ownable> ownable = dynamic_pointer_cast<Ownable>(gb[index]);
    shared_ptr<Improvable> improvable = dynamic_pointer_cast<Improvable>(gb[index]);
    if (improvable->owner != p)
    {
        cout << "You cannot improve, since you are not the owner\n";
        return;
    }
    // int x;
    /*cout << "Enter 1: improve buy\nEnter 2: improve sell\n";
    cin >> x;*/
    string deptName = improvable->dept;
    int ownedCount = p->ownedProps[deptName];

    if (improvable->getImprovs() == 0)
    {
        cout << "This building does not have any improvements to sell\n";
        return;
    }
    else
    {
        improvable->setImprovs(improvable->getImprovs() - 1);
        p->money_add(improvable->getImprovCost() / 2);
        p->updateWorth(-(improvable->getImprovCost()));
    }
}

void GameBoard::mortgage(shared_ptr<Player> p)
{
    string propName;
    cout << "Enter property that you want to mortgage\n";
    cin >> propName;
    int index = propDictionary[propName];
    shared_ptr<Ownable> ownable = dynamic_pointer_cast<Ownable>(gb[index]);
    // Improvable *improvable = dynamic_cast<Improvable *>(gb[index]);
    if (ownable->getImprovs() != 0)
    {
        cout << "You cannot mortgage due to existing improvements\n";
        return;
    }
    // check neighbours still have improvs
    for (int i = 0; i < gb.size(); i++)
    {
        shared_ptr<Ownable> prop = dynamic_pointer_cast<Ownable>(gb[index]);
        if (prop->dept == ownable->dept)
        {
            if (prop->getImprovs() > 0)
            {
                cout << "There are improvements in the monopoly" << endl;
                return;
            }
        }
    }

    if (ownable->owner != currPlayer)
    {
        cout << "You cannot mortgage this property since you are not the owner\n";
        return;
    }
    ownable->setMortgage(true);
}

void GameBoard::unmortgage(shared_ptr<Player> p)
{
    string propName;
    cout << "Enter property that you want to unmortgage\n";
    cin >> propName;
    int index = propDictionary[propName];
    shared_ptr<Ownable> ownable = dynamic_pointer_cast<Ownable>(gb[index]);
    // Improvable *improvable = dynamic_cast<Improvable *>(gb[index]);
    if (p->getMoney() < (ownable->cost) * 0.6)
    {
        cout << "You cannot unmortgage due to less money\n";
        return;
    }
    if (ownable->owner != currPlayer)
    {
        cout << "You cannot unmortgage this property since you are not the owner\n";
        return;
    }
    ownable->setMortgage(false);
}

void GameBoard::trade()
{

    cout << "Enter the name of the person you want to trade with, what you want to give and what you want to recieve\n";

    string nameOther, give, receive;
    cin >> nameOther;
    cin >> give;
    cin >> receive;

    // check if give and receive are properites or money
    bool giveMoney;
    bool receiveMoney;
    giveMoney = isNumber(give);
    receiveMoney = isNumber(receive);

    // check if the prop names entered are valid

    if (!giveMoney)
    {
        if (propDictionary.count(give) == 0)
        {
            cout << "Inavlid property name" << endl;
            return;
        }
    }

    if (!receiveMoney)
    {
        if (propDictionary.count(receive) == 0)
        {
            cout << "Inavlid property name" << endl;
            return;
        }
    }

    // reject money for money trade
    if (giveMoney && receiveMoney)
    {
        cout << "Invalid trade. You can't exchange money for money." << endl;
        // not sure if this is right to do;
        return;
    }

    // convert the give and receive into ints if they represent money
    int amtGive, amtReceive;
    if (giveMoney)
    {
        amtGive = stoi(give);
    }
    if (receiveMoney)
    {
        amtReceive = stoi(receive);
    }

    // cur wants to give prop for prop
    if (!giveMoney && !receiveMoney)
    {

        trade(give, receive, nameOther);
    }
    else if ((giveMoney && !receiveMoney))
    {
        trade(amtGive, receive, nameOther);
    }
    else
    {
        trade(give, amtReceive, nameOther);
    }
}

void GameBoard::trade(string give, string receive, string nameOther)
{

    int idx1 = 0; // index of prop cur player wants to give
    int idx2 = 0; // index of prop cur players wants to receive

    idx1 = propDictionary[give];
    idx2 = propDictionary[receive];

    shared_ptr<Ownable> propGive = dynamic_pointer_cast<Ownable>(gb[idx1]);
    shared_ptr<Ownable> propReceive = dynamic_pointer_cast<Ownable>(gb[idx2]);

    auto vec = currPlayer->playerProps;
    if (find(vec.begin(), vec.end(), propGive) == vec.end())
    {
        cout << "You dont own " << give << endl;
        return;
    }

    shared_ptr<Player> p2;
    for (int i = 0; i < player.size(); i++)
    {
        if (nameOther == player[i]->getName())
        {
            p2 = player[i];
            break;
        }
    }

    vec = p2->playerProps;
    // check if Im giving right prop
    if (find(vec.begin(), vec.end(), propGive) == vec.end())
    {
        cout << nameOther << "doesn't own " << receive << endl;
        return;
    }

    // check if properties involved in the trade are improv free
    // reject if not

    int num1 = propGive->getImprovs();    // improvs on Propgive
    int num2 = propReceive->getImprovs(); // improvs on propReceive

    if (num1 != 0 || num2 != 0)
    {
        cout << "Invalid trade. You can't give properties with improvements" << endl;
        return;
    }

    // valid trade - transaction occurs
    shared_ptr<Player> p1 = currPlayer; // how to access cur players name ??

    // step1: add the propGive to other player

    p2->addProp(propGive);
    cout << p2->playerProps[p2->playerProps.size() - 1];
    // step2: sub propGive from cur
    p1->subProp(propGive);

    // step3: add the propReceive to cur
    p1->addProp(propReceive);

    // step4: sub the propReceive from other
    p2->subProp(propReceive);
}

void GameBoard::trade(int amtGive, string receive, string nameOther)
{

    // cur wants to give money for prop

    int idx2 = propDictionary[receive];

    shared_ptr<Ownable> propReceive = dynamic_pointer_cast<Ownable>(gb[idx2]);

    // check if properties involved in the trade are improv free
    // reject if not

    int num2 = propReceive->getImprovs(); // improvs on propReceive

    if (num2 != 0)
    {
        cout << "Invalid trade. You can't give properties with improvements" << endl;
        return;
    }

    // check if cur has enough money to make the trade,
    // reject if not
    if (currPlayer->getMoney() < amtGive)
    {
        cout << "Invalid trade. You don't have sufficient funds" << endl;
    }

    // valid trade - transaction occurs
    shared_ptr<Player> p1 = currPlayer; // how to access cur players name ??
    shared_ptr<Player> p2;
    for (int i = 0; i < player.size(); i++)
    {
        if (nameOther == player[i]->getName())
        {
            p2 = player[i];
            break;
        }
    }
    // step1: add the propReceive to cur player

    p1->addProp(propReceive);

    // step2: sub propReceive from other
    p2->subProp(propReceive);

    // step3: add amt to other
    p2->money_add(amtGive);

    // step4: sub amt from cur
    p1->money_sub(amtGive);
}

void GameBoard::trade(string give, int amtReceive, string nameOther)
{
    // cur wants to give prop for money
    int idx1 = propDictionary[give];
    shared_ptr<Ownable> propGive = dynamic_pointer_cast<Ownable>(gb[idx1]);

    // check if properties involved in the trade are improv free
    // reject if not

    int num1 = propGive->getImprovs(); // improvs on propReceive

    if (num1 != 0)
    {
        cout << "Invalid trade. You can't give properties with improvements" << endl;
        return;
    }
    shared_ptr<Player> p1 = currPlayer; // how to access cur players name ??
    shared_ptr<Player> p2;
    for (int i = 0; i < player.size(); i++)
    {
        if (nameOther == player[i]->getName())
        {
            p2 = player[i];
            break;
        }
    }
    // check if cur has enough money to make the trade,
    // reject if not
    if (p2->getMoney() < amtReceive)
    {
        cout << "Invalid trade. You don't have sufficient funds" << endl;
    }

    // valid trade - transaction occurs

    // step1: add the propGive to other player

    p2->addProp(propGive);

    // step2: sub propGive from other
    p1->subProp(propGive);

    // step3: add amt to Cur
    p1->money_add(amtReceive);

    // step4: sub amt from other
    p2->money_sub(amtReceive);
}

void GameBoard::rolltest()
{
    int roll_count = 0;
    while ((roll_count < 3 && dice->isDouble()) || roll_count == 0)
    {
        if (currPlayer->isTims())
        {
            if (currPlayer->getRimCups() == 0)
            {
                cout << "Press 0 to roll, 1 to pay $50 to get out of jail" << endl;
                int j;
                cin >> j;
                if (j == 1)
                {
                    while (currPlayer->getMoney() < 50)
                    {
                        if (bankrupt(currPlayer, 50))
                        {
                            declareBankrupt(currPlayer);
                        }
                        else
                        {
                            cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                        }
                    }
                    currPlayer->money_sub(50);
                    currPlayer->goToTims(false);
                    currPlayer->resetRollsTims();
                }
            }
            else
            {
                cout << "Press 0 to roll, Press 1 to pay $50 or Press 2 to use Roll Up the RIm Cup to get out of Tims Line" << endl;
                int i;
                while (true)
                {
                    cin >> i;
                    if (cin)
                    {
                        if (i == 1)
                        {
                            while (currPlayer->getMoney() < 50)
                            {
                                if (bankrupt(currPlayer, 50))
                                {
                                    declareBankrupt(currPlayer);
                                }
                                else
                                {
                                    cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                                }
                            }
                            currPlayer->money_sub(50);
                            currPlayer->resetRollsTims();
                            break;
                        }
                        if (i == 2)
                        {
                            currPlayer->setRimCups(currPlayer->getRimCups() - 1); // Should update the card set (HOW)
                            currPlayer->goToTims(false);
                            currPlayer->resetRollsTims();
                            break;
                        }
                    }
                }
            }
        }
        int d1;
        int d2;
        cout << "Enter dice 1 number: ";
        cin >> d1;
        cout << "Enter dice 2 number: ";
        cin >> d2;
        dice->setVal(d1, d2);
        if (currPlayer->isTims() && !(dice->isDouble()) && currPlayer->getRollsTims() == 2)
        {
            if (currPlayer->getRimCups() == 0)
            {
                while (currPlayer->getMoney() < 50)
                {
                    if (bankrupt(currPlayer, 50))
                    {
                        declareBankrupt(currPlayer);
                    }
                    else
                    {
                        cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                    }
                }
                currPlayer->money_sub(50);
                currPlayer->goToTims(false);
            }
            else
            {
                cout << "Press 1 to pay $50 or Press 2 to use Roll Up the RIm Cup to get out of Tims Line" << endl;
                int i;
                while (true)
                {
                    cin >> i;
                    if (cin)
                    {
                        if (i == 1)
                        {
                            while (currPlayer->getMoney() < 50)
                            {
                                if (bankrupt(currPlayer, 50))
                                {
                                    declareBankrupt(currPlayer);
                                }
                                else
                                {
                                    cout << "You don't have enough money! You have the following choices: bankrupt, trade, mortgage, improve sell" << endl;
                                }
                            }
                            currPlayer->money_sub(50);
                            break;
                        }
                        if (i == 2)
                        {
                            currPlayer->setRimCups(currPlayer->getRimCups() - 1); // Should update the card set (HOW)
                            currPlayer->goToTims(false);
                            break;
                        }
                    }
                }
            }
        }
        else if (currPlayer->isTims() && !(dice->isDouble()))
        {
            currPlayer->addRollsTims();
            break;
        }
        else if (currPlayer->isTims() && dice->isDouble())
        {
            currPlayer->resetRollsTims();
            currPlayer->goToTims(false);
        }
        ++roll_count;
        if (roll_count == 3 && dice->isDouble())
        {
            currPlayer->setPosition(10);
            currPlayer->goToTims(true);
            break;
        }
        int oldPosition = currPlayer->getPosition();
        int sum = (dice->getVal() + oldPosition) % 40;
        currPlayer->setPosition(sum);
        int newPosition = currPlayer->getPosition();
        if (newPosition < oldPosition)
        { // Implementation for crossing OSAP
            currPlayer->money_add(200);
        }
        gb[currPlayer->getPosition()]->doOperation(currPlayer);
    }
}
