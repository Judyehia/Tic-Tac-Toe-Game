#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <limits>
using namespace std;

class Board {
private:
    vector<vector<char>> grid;
    const int size;

public:
    Board(int size = 3) : size(size) {
    grid.resize(size, vector<char>(size, ' '));
}

   void display() const {
    cout << "    ";
    for (int i = 0; i < size; ++i) {
        cout << i << "   ";
    }
    cout << endl;

    for (int i = 0; i < size; ++i) {
        cout << i << "   ";

        for (int j = 0; j < size; ++j) {
            cout << grid[i][j];

            if (j < size - 1)
                cout << " | ";
        }

        cout << endl;

        if (i < size - 1) {
            cout << "   ";
            for (int j = 0; j < size; ++j) {
                cout << "---";
                if (j < size - 1)
                    cout << "+";
            }
            cout << endl;
        }
    }
}

    bool isValidMove(int row, int col) const {
        return row >= 0 && row < size &&
               col >= 0 && col < size &&
               grid[row][col] == ' ';
    }

    bool makeMove(int row, int col, char symbol) {
        if (!isValidMove(row, col)) {
            return false;
        }

        grid[row][col] = symbol;
        return true;
    }

// Checks rows, columns, and diagonals for a winning condition
bool checkWin(char symbol) const
{
    // 1. check rows & columns
    for (int i = 0; i < size; ++i)
    {
        bool rowWin = true;
        bool colWin = true;
        for (int j = 0; j < size; ++j)
        {
            if (grid[i][j] != symbol)
                rowWin = false; // check row
            if (grid[j][i] != symbol)
                colWin = false; // check column
        }
        if (rowWin || colWin)
            return true;
    }

    // 2. check diagonals
    bool mainDiagWin = true;
    bool antiDiagWin = true;
    for (int i = 0; i < size; ++i)
    {
        if (grid[i][i] != symbol)
            mainDiagWin = false; // main diagonal
        if (grid[i][size - 1 - i] != symbol)
            antiDiagWin = false; // anti diagonal
    }
    if (mainDiagWin || antiDiagWin)
        return true;

    return false; // no wins
}

// Checks whether the board is full
bool isFull() const
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (grid[i][j] == ' ')
                return false;
        }
    }
    return true;
}

// Returns the value of the specified cell
char getCell(int row, int col) const
{
    return grid[row][col];
}

// Returns the size of the board
int getSize() const
{
    return size;
}

// Resets the board to its initial empty state
void reset()
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
            grid[i][j] = ' ';
    }
}
void setCell(int row, int col, char value)
{
    grid[row][col] = value;
}
};
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

Player::Player(const string& name, char symbol)
    : name(name), symbol(symbol) {
}

string Player::getName() const {
    return name;
}

char Player::getSymbol() const {
    return symbol;
}

void Player::setName(const string& newName) {
    name = newName;
}


class HumanPlayer : public Player {
public:
    HumanPlayer(const string& name, char symbol);

    void getMove(int& row, int& col) override;
};

HumanPlayer::HumanPlayer(const string& name, char symbol)
    : Player(name, symbol) {
}

void HumanPlayer::getMove(int& row, int& col) {
    cout << name << " (" << symbol << "), enter your move (row and column): ";
    cin >> row >> col;
}
enum class Difficulty
{
    EASY,
    HARD
};

class AIPlayer : public Player
{
private:
    Difficulty difficulty;
    const Board* board;

public:
    AIPlayer(const string& name, char symbol, Difficulty difficulty)
        : Player(name, symbol), difficulty(difficulty), board(nullptr)
    {
    }

    void setBoard(const Board* b)
    {
        board = b;
    }

    void getMove(int& row, int& col) override
    {
        if (board == nullptr)
            return;

        if (difficulty == Difficulty::EASY)
            getRandomMove(*board, row, col);
        else
            getBestMove(*const_cast<Board*>(board), row, col);
    }

    void setDifficulty(Difficulty newDifficulty)
    {
        difficulty = newDifficulty;
    }

private:
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

    int evaluateBoard(const Board& board) const
    {
        int size = board.getSize();
        char ai = getSymbol();

        for (int r = 0; r < size; ++r)
        {
            if (board.getCell(r,0) != ' ' &&
                board.getCell(r,0) == board.getCell(r,1) &&
                board.getCell(r,1) == board.getCell(r,2))
            {
                return (board.getCell(r,0) == ai) ? 10 : -10;
            }
        }

        for (int c = 0; c < size; ++c)
        {
            if (board.getCell(0,c) != ' ' &&
                board.getCell(0,c) == board.getCell(1,c) &&
                board.getCell(1,c) == board.getCell(2,c))
            {
                return (board.getCell(0,c) == ai) ? 10 : -10;
            }
        }

        if (board.getCell(0,0) != ' ' &&
            board.getCell(0,0) == board.getCell(1,1) &&
            board.getCell(1,1) == board.getCell(2,2))
        {
            return (board.getCell(0,0) == ai) ? 10 : -10;
        }

        if (board.getCell(0,2) != ' ' &&
            board.getCell(0,2) == board.getCell(1,1) &&
            board.getCell(1,1) == board.getCell(2,0))
        {
            return (board.getCell(0,2) == ai) ? 10 : -10;
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
        row = -1;
        col = -1;

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
    bool roundStopped;

public:
    Game()
        : board(3),
          player1(nullptr),
          player2(nullptr),
          currentPlayer(nullptr),
          mode(GameMode::PVP),
          roundStopped(false)
    {
    }

    ~Game()
    {
        delete player1;
        delete player2;
    }

    void showMenu()
    {
        cout << "\nTIC-TAC-TOE GAME\n";
        cout << "================\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Computer (Easy)\n";
        cout << "3. Player vs Computer (Hard)\n";
        cout << "4. Exit\n\n";
        cout << "Select game mode: ";
    }

    void setupPvP()
    {
        delete player1;
        delete player2;

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

    void setupPvC(Difficulty difficulty)
    {
        delete player1;
        delete player2;

        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = nullptr;

        string name;

        cout << "Enter your name: ";
        cin >> name;

        player1 = new HumanPlayer(name, 'X');
        player2 = new AIPlayer("Computer", 'O', difficulty);

        currentPlayer = player1;

        if (difficulty == Difficulty::EASY)
            mode = GameMode::PVC_EASY;
        else
            mode = GameMode::PVC_HARD;
    }

    void reset()
    {
        board.reset();
        currentPlayer = player1;
        roundStopped = false;
    }

    void start()
    {
        roundStopped = false;

        int choice;

        while (true)
        {
            showMenu();

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid input. Enter a number from 1 to 4.\n";
                continue;
            }

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

        srand(static_cast<unsigned int>(time(nullptr)));

        cout << "\nEnter moves using row and column numbers from 0 to 2.\n";

        board.display();

        while (!checkGameEnd())
        {
            AIPlayer* aiPlayer =
                dynamic_cast<AIPlayer*>(currentPlayer);

            if (aiPlayer != nullptr)
                handleAIMove(aiPlayer);

            else
                handleHumanMove(currentPlayer);

            if (roundStopped)
                break;

            board.display();

            if (checkGameEnd())
                break;

            switchPlayer();
        }

        displayResult();
    }

    void switchPlayer()
    {
        if (currentPlayer == player1)
            currentPlayer = player2;

        else
            currentPlayer = player1;
    }

    void handleHumanMove(Player* player)
    {
        while (true)
        {
            int row;
            int col;

            player->getMove(row, col);

            if (!cin)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Invalid input. Enter two numbers.\n";
                continue;
            }

            if (row < 0 || row >= board.getSize() ||
                col < 0 || col >= board.getSize())
            {
                cout << "Out of range. Use numbers from 0 to 2.\n";
                continue;
            }

            if (board.makeMove(row, col, player->getSymbol()))
                return;

            cout << "That cell is occupied. Try another cell.\n";
        }
    }

    void handleAIMove(AIPlayer* aiPlayer)
    {
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

        cout << aiPlayer->getName()
             << " (" << aiPlayer->getSymbol()
             << ") played row "
             << row
             << ", column "
             << col
             << ".\n";
    }

    bool checkGameEnd()
    {
        if (player1 == nullptr || player2 == nullptr)
            return false;

        return board.checkWin(player1->getSymbol()) ||
               board.checkWin(player2->getSymbol()) ||
               board.isFull();
    }

    void displayResult() const
    {
        if (player1 != nullptr &&
            board.checkWin(player1->getSymbol()))
        {
            cout << player1->getName()
                 << " wins!\n";
        }

        else if (player2 != nullptr &&
                 board.checkWin(player2->getSymbol()))
        {
            cout << player2->getName()
                 << " wins!\n";
        }

        else if (board.isFull())
        {
            cout << "It's a draw!\n";
        }
    }
};
int main()
{
    Game game;
    game.start();

    return 0;
}
