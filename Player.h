#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;

class Player {
protected:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol);

    virtual ~Player() {}
    virtual void getMove(int& row, int& col) = 0;

    string getName() const;
    char getSymbol() const;
    void setName(const string& name);
};

#endif
