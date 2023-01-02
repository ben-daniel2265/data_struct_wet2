#ifndef TEAM_H
#define TEAM_H

#include "Player.h"
#include "wet2util.h"
class Player;


class Team {
    Player* captain;
    int teamId;
    int teamPoints;
    int teamAbility;
    int playersCount;
    bool goalKeeper;
    permutation_t* teamSpirit;

    public:
        Team(int teamId);
        ~Team();
        int getId();
        int getTeamPoints();
        int getTeamAbility();
        int getPlayersCount();
        bool getGoalKeeper();
        Player* getCaptain();
        permutation_t* getTeamSpirit();
        void setTeamId(int teamId);
        void setCaptain(Player* captain);
        void setTeamPoints(int teamPoints);
        void setTeamAbility(int teamAbility);
        void setPlayersCount(int playersCount);
        void setGoalKeeper(bool goalKeeper);
        void setTeamSpirit(permutation_t* teamSpirit);
};


#endif // TEAM_H