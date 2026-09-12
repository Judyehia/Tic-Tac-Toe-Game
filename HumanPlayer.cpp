#include "HumanPlayer.h"
#include <iostream>
using namespace std;

HumanPlayer::HumanPlayer(const string& name, char symbol)
    : Player(name, symbol) {}

void HumanPlayer::getMove(int& row, int& col) {
    cout << name << " (" << symbol << "), enter your move (row and column): ";
    cin >> row >> col;
}
