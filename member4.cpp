// ===== AIPlayer: Structure & Easy AI (Member 4 / TTTG-4) =====
#include <vector>
#include <utility>
#include <cstdlib>

// Shared with Member 5 (Hard AI). Only one definition should survive
// once everyone's files are merged into the final single-file class.
enum class Difficulty
{
    EASY,
    HARD
};

// NOTE for whoever assembles the final AIPlayer class declaration:
// besides name/symbol inherited from Player, AIPlayer needs these
// private members:
//     Difficulty difficulty;
//     const Board* board;   // set via setBoard() before getMove() is called

// Initializes the AI player with a name, symbol, and difficulty
AIPlayer(const string& name, char symbol, Difficulty difficulty)
    : Player(name, symbol), difficulty(difficulty), board(nullptr)
{
}

// Gives the AI a live view of the board. Game must call this
// (e.g. in handleAIMove) before calling getMove().
void setBoard(const Board* b)
{
    board = b;
}

// Determines the AI move based on difficulty level
void getMove(int& row, int& col) override
{
    if (board == nullptr)
        return; // Game must call setBoard() first

    if (difficulty == Difficulty::EASY)
        getRandomMove(*board, row, col);
    else
        getBestMove(*board, row, col); // Member 5: Hard AI / Minimax
}

// Changes AI difficulty setting
void setDifficulty(Difficulty newDifficulty)
{
    difficulty = newDifficulty;
}

// Selects a random valid move from the available board positions
void getRandomMove(const Board& board, int& row, int& col) const
{
    vector<pair<int, int>> emptyCells;
    int size = board.getSize();

    for (int r = 0; r < size; ++r)
    {
        for (int c = 0; c < size; ++c)
        {
            if (board.getCell(r, c) == ' ')
                emptyCells.push_back({r, c});
        }
    }

    if (emptyCells.empty())
        return; // no valid moves; caller should check isFull() first

    int index = rand() % static_cast<int>(emptyCells.size());
    row = emptyCells[index].first;
    col = emptyCells[index].second;
}
