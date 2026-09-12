#include <iostream>
#include <vector>
using namespace std;

class Board {
private:
    vector<vector<char>> grid;
    int size;

public:
    Board(int size = 3) {
        this->size = size;
        grid.resize(size, vector<char>(size, ' '));
    }

    void display() const {
        cout << "  ";
        for (int i = 1; i <= size; i++) {
            cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < size; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < size; j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
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
};
