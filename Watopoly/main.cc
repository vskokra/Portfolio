#include "controller.h"
#include <iostream>
using namespace std;

int main(int argc, char *argvp[]){
    if (argc == 1) {
        Controller c;
        c.play(false);
    } else if (argc == 2) {
        Controller c{true};
        c.play(false);
    } else if (argc == 3) {
        Controller c{};
        //
    } else if (argc == 4) {
        //
    } else {
        cout << "Invalid Command Line Arguments Entered!\n";
    }   
}
