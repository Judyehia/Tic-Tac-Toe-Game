#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include "AIPlayer.h"

enum class GameMode
{
    PVP,
    PVC_EASY,
    PVC_HARD
};

class Game
{
private:
    Board board;

    Player* player1;
    Player* player2;
    Player* currentPlayer;

    GameMode mode;
    bool roundStopped = false;

public:
    // Member 6
    Game();
    void showMenu();
    void setupPvP();
    void setupPvC(Difficulty difficulty);
    void reset();

    // Member 7: your part
    void start();
    void switchPlayer();
    void handleHumanMove(Player* player);
    void handleAIMove(AIPlayer* aiPlayer);
    bool checkGameEnd();
    void displayResult() const;

    // Memory management
    ~Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

#endif
