#include <vector>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;

enum class Difficulty
{
    EASY,
    HARD
};

class AIPlayer : public Player
{
private:
    Difficulty difficulty;
    const Board* board; // set via setBoard() before getMove()

public:
    // Constructor
    AIPlayer(const string& name, char symbol, Difficulty difficulty)
        : Player(name, symbol), difficulty(difficulty), board(nullptr)
    {
    }

    // Attach board reference
    void setBoard(const Board* b)
    {
        board = b;
    }

    // Main move function
    void getMove(int& row, int& col) override
    {
        if (board == nullptr)
            return; // must call setBoard() first

        if (difficulty == Difficulty::EASY)
            getRandomMove(*board, row, col);
        else
            getBestMove(*const_cast<Board*>(board), row, col);
    }

    // Change difficulty
    void setDifficulty(Difficulty newDifficulty)
    {
        difficulty = newDifficulty;
    }

private:
    // Easy AI: random move
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
            return;

        int index = rand() % static_cast<int>(emptyCells.size());
        row = emptyCells[index].first;
        col = emptyCells[index].second;
    }

    // Hard AI: Minimax
    int evaluateBoard(const Board& board) const
    {
        int size = board.getSize();
        char ai = getSymbol();
        char opp = (ai == 'X') ? 'O' : 'X';

        // Rows
        for (int r = 0; r < size; ++r)
        {
            if (board.getCell(r,0) != ' ' &&
                board.getCell(r,0) == board.getCell(r,1) &&
                board.getCell(r,1) == board.getCell(r,2))
            {
                return (board.getCell(r,0) == ai) ? +10 : -10;
            }
        }
        // Columns
        for (int c = 0; c < size; ++c)
        {
            if (board.getCell(0,c) != ' ' &&
                board.getCell(0,c) == board.getCell(1,c) &&
                board.getCell(1,c) == board.getCell(2,c))
            {
                return (board.getCell(0,c) == ai) ? +10 : -10;
            }
        }
        // Diagonals
        if (board.getCell(0,0) != ' ' &&
            board.getCell(0,0) == board.getCell(1,1) &&
            board.getCell(1,1) == board.getCell(2,2))
        {
            return (board.getCell(0,0) == ai) ? +10 : -10;
        }
        if (board.getCell(0,2) != ' ' &&
            board.getCell(0,2) == board.getCell(1,1) &&
            board.getCell(1,1) == board.getCell(2,0))
        {
            return (board.getCell(0,2) == ai) ? +10 : -10;
        }
        return 0;
    }

    int minimax(Board& board, int depth, bool isMax) const
    {
        int score = evaluateBoard(board);

        if (score == 10) return score - depth;
        if (score == -10) return score + depth;
        if (board.isFull()) return 0;

        char ai = getSymbol();
        char opp = (ai == 'X') ? 'O' : 'X';

        if (isMax)
        {
            int best = -1000;
            for (int r = 0; r < board.getSize(); ++r)
            {
                for (int c = 0; c < board.getSize(); ++c)
                {
                    if (board.getCell(r,c) == ' ')
                    {
                        board.setCell(r,c,ai);
                        best = max(best, minimax(board, depth+1, false));
                        board.setCell(r,c,' ');
                    }
                }
            }
            return best;
        }
        else
        {
            int best = 1000;
            for (int r = 0; r < board.getSize(); ++r)
            {
                for (int c = 0; c < board.getSize(); ++c)
                {
                    if (board.getCell(r,c) == ' ')
                    {
                        board.setCell(r,c,opp);
                        best = min(best, minimax(board, depth+1, true));
                        board.setCell(r,c,' ');
                    }
                }
            }
            return best;
        }
    }

    void getBestMove(Board& board, int& row, int& col) const
    {
        int bestVal = -1000;
        row = -1; col = -1;

        for (int r = 0; r < board.getSize(); ++r)
        {
            for (int c = 0; c < board.getSize(); ++c)
            {
                if (board.getCell(r,c) == ' ')
                {
                    board.setCell(r,c,getSymbol());
                    int moveVal = minimax(board, 0, false);
                    board.setCell(r,c,' ');

                    if (moveVal > bestVal)
                    {
                        row = r;
                        col = c;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }
};
