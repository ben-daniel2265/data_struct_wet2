#ifndef PLAYER_H
#define PLAYER_H

#include "Team.h"
#include "wet2util.h"
class Team;

class Player {
    int playerId;
    int gamesPlayed;
    int ability;
    int cards;
    bool goalKeeper;


    Player* daddy;

    bool captain;
    Team* team;

    const permutation_t* spirit;
    permutation_t* spiritExtra;

    public:
        Player(int playerId, int gamesPlayed, int ability, int cards, bool goalKeeper, permutation_t spirit);
        ~Player();
        int getId();
        int getGamesPlayed();
        int getAbility();
        int getCards();
        bool getGoalKeeper();
        Player* getDaddy();
        bool getCaptain();
        Team* getTeam();
        const permutation_t* getSpirit();
        permutation_t* getSpiritExtra();
        void setPlayerId(int playerId);
        void setGamesPlayed(int gamesPlayed);
        void setAbility(int ability);
        void setCards(int cards);
        void setGoalKeeper(bool goalKeeper);
        void setDaddy(Player* daddy);
        void setCaptain(bool captain);
        void setTeam(Team* team);
        void setSpirit(permutation_t* spirit);
        void setSpiritExtra(permutation_t* spiritExtra);

};


#endif // PLAYER_H