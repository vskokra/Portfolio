#include "controller.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "gameboard.h"
#include "player.h"
#include "display.h"
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;

// COMPLETE
Controller::Controller(bool testingMode) :  testingMode{testingMode}, d{nullptr}{
    game = nullptr;
    validCommands = {"roll", "assets", "all", "save"};
}

// COMPLETE
Controller::~Controller() {
    delete game;
    delete d;
}

// COMPLETE
void Controller::init(vector<shared_ptr<Player>> players) {
    game = new GameBoard(players);
    d = new Display(game);
}
// COMPLETE
void Controller::roll() {
    if (testingMode) {
        game -> rolltest();
    } else {
        game -> roll();
    }
    d->printDisplay(); 
}

// COMPLETED
void Controller::bigFive() {
    game -> basicFive(game -> currPlayer);
}

// COMPLETED
void Controller::assets(shared_ptr<Player> player) {
    cout << "Player Name: " << player -> getName() << endl;
    cout << "Player Character: " << player -> playerChar << endl;
    cout << "The Player has: " << player -> getMoney() << "$" << endl;
    cout << "The Player's networth - money is: " << player -> getWorth() << endl;
    cout << "The Player owns: " << player -> playerProps.size() << " number of properties\n";
    cout << "Now displaying Player's properties:\n";
    for (int x = 0; x < player -> playerProps.size(); ++x) {
        shared_ptr <Ownable> property = player -> playerProps[x];
        cout << "Property Name: " << property -> propName << endl;
        cout << "Property Base Cost: " << property -> cost << endl;
        if (property -> isMortgage) {
            cout << "Is Mortgaged\n";
        } else {
            cout << "Is not Mortgaged\n";
        }
        cout << "Number of Improvements: " << property -> getImprovs() << endl;
        cout << "Improvement Cost: " << property -> getImprovCost() << endl;
        cout << "TOTAL Value of property after improvements: " << ((property -> getImprovs()) * property -> getImprovCost()) + property -> cost << "$\n";

    }
    cout << "*************************************************************\n";
} 

// COMPLETED
void Controller::all() {
    cout << "Displaying the assets of all the players:\n";
    for (int x = 0; x < game -> player.size(); ++x) {
        assets(game -> player[x]);
    }
}

// HELPER FUNCTION COMPLETED
bool Controller::isValidCommand(string command) {
    for (int x = 0; x < validCommands.size(); ++x) {
        if (validCommands[x] == command) return true;   
    }
    return false;
}


// COMPLETED????
void Controller::play(bool alreadyInit) {
    if (!alreadyInit) {
        int numberofPlayers;
        string playerName;
        char playerChar;
        unordered_map<char, string> playerDetails;
        vector<shared_ptr<Player>> playerVector;
        cout << "Welcome to Watopoly!\n";
        cout << "Please Enter the Number of Players playing today: ";
        cin >> numberofPlayers;
        if (numberofPlayers < 2 || numberofPlayers > 8) {
            cout << "Invalid number of players entered! Exiting!";
            return;
        }
        for (int x = 1; x <= numberofPlayers; ++x) {
            cout << "You will now be entering player details.\n";
            cout << "If there are no more players, please enter done\n";
            cout << "Please enter player " << x << " name: ";
            cin >> playerName;
            cout << "Please enter player " << x << " character: ";
            cin >> playerChar;
            shared_ptr <Player> vectorPlayer = make_shared <Player> (playerName, playerChar);
            playerVector.emplace_back(vectorPlayer);
            playerDetails[playerChar] = playerName;
        }
        cout << "Thank you for entering player details!\n";
        cout << "The game is now ready to be played!! Yay!!\n";
        init(playerVector);
        }
        d->printDisplay();
        string command;
        while (true) {
        cout << "You can enter the following commands: \n";  
        cout << "roll: to start the game\n";
        cout << "assets: to display your assets\n";
        cout << "all: to display the assets of all the players\n";
        cout << "save: to save the current game\n";
        /*cout << "trade: to start trading\n";
        cout << "improve: to improve a property\n";
        cout << "mortgage: to mortgage a property\n";
        cout << "unmortgage: to unmortgage a property\n";
        cout << "bankruptcy: to declare bankruptcy\n";*/
        cout << "quit: to exit the game\n";
        cin >> command;
//      "roll", "assets", "all", "save"
        if (isValidCommand(command)) {
            if (command == "roll") {
                roll();
                bigFive();
            } else if (command == "assets") {
                assets(game -> currPlayer);
            } else if (command == "all") {
                all();
            } else {
                string filename = "savedGame.txt";
                saveGame(filename);
            }
        } /*else if (command == "trade" || command == "mortgage" ||  command == "unmortgage"
                    || command == "improve" ||  command == "bankruptcy") {
                bigFive();
        } */else if (command == "quit") {
            cout << "Thank you for playing Watopoly!\n";
            cout << "Quiting the game now!\n";
            return;
        } else {
            cout << "Invalid command entered! Please enter one of the valid commands!\n";
        }
    }
}



bool validChar(char c) {
    if (c == '0' || c == '1' || c == '2'|| c == '3' || c == '4' || c == '5' || c == '6'
        || c == '7' || c == '8' || c == '9') return true;

    return false;
}

bool isInt(string s) {
    for (int x = 0; x < s.length(); ++x) {
        if (!validChar(s[x])) return false;
    }
    return true;
}

// TO BE COMPLETED
void Controller::loadGame(string filename) {
    /*
        ifstream infile{filename};
        while (!infile.eof()) { 
            stringstream line;
            string text;
            getline(infile, text);
            line << text;
            int numberOfPlayers;
            if (isInt(text)) {
                numberOfPlayers = text;
            } else {
                vector<string> words;
                while (line >> text) {
                  words.push_back(text);
                }
                if (words.size() == 3) {

                } else {
                    for (int x = 0; x < 5; ++x) {
                    }
                }
            }
        }
    return;
    */
}

// TO BE COMPLETED
void Controller::saveGame(string filename) {
        // OPEN File
    ofstream gameState;
    gameState.open(filename);
    // Save Number of Players
    gameState << game -> player.size() << endl;
    // Save Players' Details
    for (int x = 0; x < game -> player.size(); ++x) {
        shared_ptr<Player> myPlayer = game -> player[x];
        gameState << myPlayer -> getName() << " ";
        gameState << myPlayer -> playerChar << " ";
        gameState << myPlayer -> getRimCups() << " ";
        gameState << myPlayer -> getMoney() << " ";
        gameState << myPlayer -> getPosition();
        if (myPlayer -> getPosition() == 10) {
            if (myPlayer -> isTims()) {
                gameState << " 1 " << myPlayer -> getRollsTims();
            } else {
                gameState << " 0";
            }
        }
        gameState << endl;
    }
    //  SAVE BUILDING DETAILS
    vector<int> building{1, 3, 5, 6, 8, 9, 11, 12, 13, 14, 15, 16, 
                        18, 19, 21, 23, 24, 25, 26, 27, 28, 29, 31, 32, 
                        34, 35, 37, 39};
    for (int x = 0; x < 28; ++x) {
        int index = building[x];
        shared_ptr<Ownable> myBuilding = dynamic_pointer_cast<Ownable>(game -> gb[index]);
        string buildingName = myBuilding -> propName;
        string ownerName = "BANK";
        int improvements = 0;
        bool isMortgage = myBuilding -> isMortgage;
        if (myBuilding -> owner != nullptr) ownerName = myBuilding -> owner -> getName();
        if (isMortgage) improvements = -1;
        else improvements = myBuilding -> getImprovs();
        gameState << buildingName << " ";
        gameState << ownerName << " ";
        gameState << improvements << endl;
    }
    // Close File
    gameState.close();
}
