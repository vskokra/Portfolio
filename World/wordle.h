#ifndef _WORDLE_H_
#define _WORDLE_H_
#include "termcodes.h"

struct Wordle
{
    char *code;

    Wordle(char *code);
    ~Wordle();
    void playGame();
};

#endif
