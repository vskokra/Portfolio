#ifndef _DICE_H
#define _DICE_H

class Dice
{
    bool doubles;
    int sum;

public:
    Dice();
    ~Dice();
    void setVal();
    int getVal();
    bool isDouble();
    void setVal(int a, int b);
};
#endif
