#include "wordle.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// check_length(code, guess) checks if the guess and the sectret word have the same length.
bool check_length(string secret, string guess)
{
    if (secret.length() == guess.length())
    {
        return true;
    }
    else
    {
        return false;
    }
}

// compare(code, guess) returns whether or not secret is equal to guess, and provides desired output.
bool compare(string code, string guess)
{
    int len = code.length();
    int check_var = 0;
    for (int i = 0; i < len; i++)
    {
        if (guess[i] == code[i])
        {
            cout << GREEN << guess[i] << RESET;
            check_var++;
        }
        else
        {
            for (int j = 0; j < len; j++)
            {
                if (guess[i] == code[j] && guess[j] != code[j])
                {
                    cout << YELLOW << guess[i] << RESET;
                    break;
                }
                else if (j == len - 1)
                {
                    cout << guess[i];
                    break;
                }
            }
        }
        if (i == len - 1)
        {
            cout << endl;
        }
    }
    if (check_var == len)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Wordle::playGame()
{
    string secret = code;
    int try_count = 0;
    string input;
    string guess;
    bool checklength = true;
    bool equal = false;
    while (true)
    {
        if (try_count < 6 && !equal)
        {
            if (checklength)
            {
                cout << "Enter " << secret.length() << " length word as guess: ";
            }
            if (getline(cin, input))
            {
                istringstream temp{input};
                if ((try_count < 6) && !input.empty() && temp >> guess)
                {
                    if (!check_length(secret, guess))
                    {
                        cout << "Incorrect guess size, please enter guess of length " << secret.length() << ": ";
                        checklength = false;
                    }
                    else
                    {
                        checklength = true;
                        equal = compare(secret, guess);
                        if (equal != true)
                        {
                            try_count++;
                        }
                    }
                }
            }
            else
            {
                cout << "Out of guesses! The word was: " << secret << endl;
                break;
            }
        }
        else
        {
            break;
        }
    }
    if (equal == true)
    {
        cout << "Congrats you found the word!" << endl;
    }
    else if (try_count >= 6)
    {
        cout << "Out of guesses! The word was: " << secret << endl;
    }
}

Wordle::Wordle(char *code)
{
    this->code = code;
}

Wordle::~Wordle()
{
}
