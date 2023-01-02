#ifndef TEAM_SOURCE
#define TEAM_SOURCE


#include "Team.h"

Team::Team(int teamId) {
    this->teamId = teamId;
    this->teamPoints = 0;
    this->playersCount = 0;
    this->teamAbility = 0;
    this->goalKeeper = false;
    this->captain = nullptr;
    this->teamSpirit = new permutation_t(permutation_t::neutral());
}

Team::~Team() {
    delete this->teamSpirit;
}

int Team::getId() {
    return this->teamId;
}

int Team::getTeamPoints() {
    return this->teamPoints;
}

int Team::getTeamAbility() {
    return this->teamAbility;
}

int Team::getPlayersCount() {
    return this->playersCount;
}

bool Team::getGoalKeeper() {
    return this->goalKeeper;
}

Player* Team::getCaptain() {
    return this->captain;
}

permutation_t* Team::getTeamSpirit() {
    return this->teamSpirit;
}

void Team::setTeamId(int teamId) {
    this->teamId = teamId;
}

void Team::setCaptain(Player* captain) {
    this->captain = captain;
}

void Team::setTeamPoints(int teamPoints) {
    this->teamPoints = teamPoints;
}

void Team::setTeamAbility(int teamAbility) {
    this->teamAbility = teamAbility;
}

void Team::setPlayersCount(int playersCount) {
    this->playersCount = playersCount;
}

void Team::setGoalKeeper(bool goalKeeper) {
    this->goalKeeper = goalKeeper;
}

void Team::setTeamSpirit(permutation_t* teamSpirit) {
    delete this->teamSpirit;
    this->teamSpirit = teamSpirit;
}



#endif // TEAM_SOURCE