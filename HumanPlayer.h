#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H
#include "Player.h"

class HumanPlayer : public Player {
public:

    HumanPlayer(const string& name, char symbol);

    void getMove(int& row, int& col) override;
};

#endif
