#ifndef PLAYER_SOURCE
#define PLAYER_SOURCE

#include "Player.h"

Player::Player(int playerId, int gamesPlayed, int ability, int cards, bool goalKeeper, const permutation_t* spirit) {
    this->playerId = playerId;
    this->gamesPlayed = gamesPlayed;
    this->ability = ability;
    this->cards = cards;
    this->goalKeeper = goalKeeper;
    this->daddy = nullptr;
    this->captain = false;
    this->team = nullptr;
    this->spirit = spirit;
    this->spiritExtra = new permutation_t();
}

Player::~Player() {
    delete this->spirit;
    delete this->spiritExtra;
}

int Player::getId() {
    return this->playerId;
}

int Player::getGamesPlayed() {
    return this->gamesPlayed;
}

int Player::getAbility() {
    return this->ability;
}

int Player::getCards() {
    return this->cards;
}

bool Player::getGoalKeeper() {
    return this->goalKeeper;
}

Player* Player::getDaddy() {
    return this->daddy;
}

bool Player::getCaptain() {
    return this->captain;
}

Team* Player::getTeam() {
    return this->team;
}

const permutation_t* Player::getSpirit() {
    return this->spirit;
}   

permutation_t* Player::getSpiritExtra() {
    return this->spiritExtra;
}

void Player::setPlayerId(int playerId) {
    this->playerId = playerId;
}

void Player::setGamesPlayed(int gamesPlayed) {
    this->gamesPlayed = gamesPlayed;
}

void Player::setAbility(int ability) {
    this->ability = ability;
}

void Player::setCards(int cards) {
    this->cards = cards;
}

void Player::setGoalKeeper(bool goalKeeper) {
    this->goalKeeper = goalKeeper;
}

void Player::setDaddy(Player* daddy) {
    this->daddy = daddy;
}

void Player::setCaptain(bool captain) {
    this->captain = captain;
}

void Player::setTeam(Team* team) {
    this->team = team;
}

void Player::setSpirit(permutation_t* spirit) {
    delete this->spirit;
    this->spirit = spirit;
}

void Player::setSpiritExtra(permutation_t* spiritExtra) {
    delete this->spiritExtra;
    this->spiritExtra = spiritExtra;
}


#endif