#include "Player.h"
Player::Player(const string& name, char symbol)
    : name(name), symbol(symbol) {}
string Player::getName() const {
    return name;
}

char Player::getSymbol() const {
    return symbol;
}

void Player::setName(const string& newName) {
    name = newName;
}
