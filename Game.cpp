#include "Game.h"
#include "HumanPlayer.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>

using namespace std;


// Initializes the game with an empty board and null players.
Game::Game() : board(3), player1(nullptr), player2(nullptr),
currentPlayer(nullptr), mode(GameMode::PVP) {
}

// Displays mode selection menu.
void Game::showMenu()
{
    cout << "\nTIC-TAC-TOE GAME\n";
    cout << "================\n";
    cout << "1. Player vs Player\n";
    cout << "2. Player vs Computer (Easy)\n";
    cout << "3. Player vs Computer (Hard)\n";
    cout << "4. Exit\n\n";
    cout << "Select game mode: ";
}

// Configures Player vs Player mode with user input.
void Game::setupPvP()
{
    delete player1;
    delete player2;
    // Shared safety: keep ownership valid if creating a player throws.
    player1 = nullptr;
    player2 = nullptr;
    currentPlayer = nullptr;
    string name1, name2;

    cout << "Enter name for Player 1 (X): ";
    cin >> name1;
    cout << "Enter name for Player 2 (O): ";
    cin >> name2;

    player1 = new HumanPlayer(name1, 'X');
    player2 = new HumanPlayer(name2, 'O');
    currentPlayer = player1;
    mode = GameMode::PVP;
}

// Configures Player vs Computer mode.
void Game::setupPvC(Difficulty difficulty)
{
    delete player1;
    delete player2;
    player1 = nullptr;
    player2 = nullptr;
    currentPlayer = nullptr;
    string name1;

    cout << "Enter your name: ";
    cin >> name1;

    player1 = new HumanPlayer(name1, 'X');
    player2 = new AIPlayer("Computer", 'O', difficulty);
    currentPlayer = player1;
    mode = (difficulty == Difficulty::EASY)
        ? GameMode::PVC_EASY : GameMode::PVC_HARD;
}

void Game::reset()
{
    board.reset();
    currentPlayer = player1;
    roundStopped = false; // Member 7: clear a previous interrupted round.
}

Game::~Game()
{
    delete player1;
    delete player2;
}

// ===================== MEMBER 7: GAME FLOW =====================

// Selects a mode, sets up the players, and runs one complete round.
// Calling start() again shows the menu and starts a fresh round.
void Game::start()
{
    roundStopped = false;
    int choice = 0;

    while (true)
    {
        showMenu();

        if (!(cin >> choice))
        {
            if (cin.eof() || cin.bad())
            {
                roundStopped = true;
                return;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number from 1 to 4.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice >= 1 && choice <= 4)
            break;

        cout << "Invalid choice. Select 1, 2, 3, or 4.\n";
    }

    if (choice == 4)
    {
        cout << "Goodbye!\n";
        return;
    }

    if (choice == 1)
        setupPvP();
    else if (choice == 2)
        setupPvC(Difficulty::EASY);
    else
        setupPvC(Difficulty::HARD);

    reset();
    if (!cin)
    {
        roundStopped = true;
        cout << "Input closed during player setup.\n";
        return;
    }

    // The existing Easy AI uses rand(). Seed it before the round.
    srand(static_cast<unsigned int>(time(nullptr)));
    cout << "\nEnter moves using row and column numbers from 1 to 3.\n";
    board.display();

    while (!checkGameEnd())
    {
        AIPlayer* aiPlayer = dynamic_cast<AIPlayer*>(currentPlayer);
        if (aiPlayer != nullptr)
            handleAIMove(aiPlayer);
        else
            handleHumanMove(currentPlayer);

        if (roundStopped)
            break;

        board.display();
        // Check the final move before giving the other player a turn.
        if (checkGameEnd())
            break;

        switchPlayer();
    }

    displayResult();
}

// Switches the turn to the other player.
void Game::switchPlayer()
{
    if (currentPlayer == player1)
        currentPlayer = player2;
    else
        currentPlayer = player1;
}

// Repeats until the human supplies a legal move or input closes.
void Game::handleHumanMove(Player* player)
{
    if (player == nullptr || checkGameEnd())
        return;

    while (true)
    {
        int row = -1;
        int col = -1;
        player->getMove(row, col);

        if (!cin)
        {
            if (cin.eof() || cin.bad())
            {
                roundStopped = true;
                cout << "\nInput closed.\n";
                return;
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter two whole numbers, for example 1 2.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Validate before conversion, so even extreme integers are safe.
        if (row < 1 || row > board.getSize() ||
            col < 1 || col > board.getSize())
        {
            cout << "Out of range. Row and column must be from 1 to 3.\n";
            continue;
        }

        // Human coordinates follow display(): 1..3. Board indices: 0..2.
        if (board.makeMove(row - 1, col - 1, player->getSymbol()))
            return;

        cout << "That cell is occupied. Try another cell.\n";
    }
}

// Obtains and applies one AI move, already expressed as 0-based indices.
void Game::handleAIMove(AIPlayer* aiPlayer)
{
    if (aiPlayer == nullptr || checkGameEnd())
        return;

    aiPlayer->setBoard(&board);
    int row = -1;
    int col = -1;
    aiPlayer->getMove(row, col);

    if (!board.makeMove(row, col, aiPlayer->getSymbol()))
    {
        roundStopped = true;
        cout << "The AI returned an invalid move.\n";
        return;
    }

    cout << aiPlayer->getName() << " (" << aiPlayer->getSymbol()
        << ") played row " << row + 1
        << ", column " << col + 1 << ".\n";
}

// Detects wins, draws, and interrupted rounds.
bool Game::checkGameEnd()
{
    if (roundStopped)
        return true;

    if (player1 == nullptr || player2 == nullptr)
        return false;

    return board.checkWin(player1->getSymbol()) ||
        board.checkWin(player2->getSymbol()) ||
        board.isFull();
}

// Checks wins before a draw, including when the last move fills the board.
void Game::displayResult() const
{
    if (player1 != nullptr && board.checkWin(player1->getSymbol()))
        cout << player1->getName() << " (" << player1->getSymbol() << ") wins!\n";
    else if (player2 != nullptr && board.checkWin(player2->getSymbol()))
        cout << player2->getName() << " (" << player2->getSymbol() << ") wins!\n";
    else if (board.isFull())
        cout << "It's a draw!\n";
    else if (roundStopped)
        cout << "The round stopped before completion.\n";
    else
        cout << "There is no final result yet.\n";
}
