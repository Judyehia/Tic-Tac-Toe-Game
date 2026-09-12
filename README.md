# Tic-Tac-Toe Game
// ===================== MEMBER 6 ==========================
// Game() — Initializes game with empty board and null players
Game() : board(3), player1(nullptr), player2(nullptr),
          currentPlayer(nullptr), mode(GameMode::PVP) {}

// showMenu() — Displays mode selection menu
void showMenu() {
    cout << "\nTIC-TAC-TOE GAME\n";
    cout << "================\n";
    cout << "1. Player vs Player\n";
    cout << "2. Player vs Computer (Easy)\n";
    cout << "3. Player vs Computer (Hard)\n";
    cout << "4. Exit\n\n";
    cout << "Select game mode: ";
}

// setupPvP() — Configures Player vs Player mode with user input
void setupPvP() {
    delete player1;
    delete player2;
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

// setupPvC(Difficulty difficulty) — Configures Player vs Computer mode
void setupPvC(Difficulty difficulty) {
    delete player1;
    delete player2;
    string name1;

    cout << "Enter your name: ";
    cin >> name1;

    player1 = new HumanPlayer(name1, 'X');
    player2 = new AIPlayer("Computer", 'O', difficulty);
    currentPlayer = player1;
    mode = (difficulty == Difficulty::EASY) ? GameMode::PVC_EASY : GameMode::PVC_HARD;
}

// reset() — Prepares game for new round
void reset() {
    board.reset();
    currentPlayer = player1;
}
// ===========================================================


