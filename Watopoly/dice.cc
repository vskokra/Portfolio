
#include <random>
#include "dice.h"

Dice::Dice() : doubles(false) {}
Dice::~Dice() {}

void Dice::setVal()
{
    srand(time(NULL));
    int dice1 = rand() % 6 + 1;
    int dice2 = rand() % 6 + 1;
    if (dice1 == dice2)
    {
        doubles = true;
    }
    else
    {
        doubles = false;
    }
    sum = dice1 + dice2;
}

int Dice::getVal()
{
    return sum;
}

bool Dice::isDouble()
{
    return doubles;
}

void Dice::setVal(int a, int b)
{
    int dice1 = a;
    int dice2 = b;
    if (dice1 == dice2)
    {
        doubles = true;
    }
    else
    {
        doubles = false;
    }
    sum = dice1 + dice2;
}