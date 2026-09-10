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