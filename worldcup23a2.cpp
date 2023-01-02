#include "worldcup23a2.h"

int compareTeamsByAbility(Team* t1, Team* t2){
	if(t1->getTeamAbility() == t2->getTeamAbility()){
		return t1->getId() - t2->getId();
	}
	return t1->getTeamAbility() - t2->getTeamAbility();
}

int compare_team_by_id(Team* team1, Team* team2){
	return team1->getId() - team2->getId(); 
}

world_cup_t::world_cup_t()
{
	this->teamTreeByAbility = new RankAVLTree<Team>();
	this->teamTreeById = new RankAVLTree<Team>();
	this->players = new HashTable<Player>();
}

world_cup_t::~world_cup_t()
{
	delete this->players;
	this->teamTreeById->delete_values();
	this->teamTreeByAbility->delete_hollow_tree();
	this->teamTreeById->delete_hollow_tree();
	delete this->teamTreeByAbility;
	delete this->teamTreeById;
}

StatusType world_cup_t::add_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Team* newTeam = new Team(teamId);

	if(this->teamTreeById->findValue(newTeam, compare_team_by_id) != NULL){
		delete newTeam;
		return StatusType::FAILURE;
	}

	this->teamTreeById->insertValue(newTeam, compare_team_by_id);
	this->teamTreeByAbility->insertValue(newTeam, compareTeamsByAbility);

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Team* tempTeam = new Team(teamId);
	RankAVLTree<Team>::Node* tempNode = this->teamTreeById->findValue(tempTeam, compare_team_by_id);

	if(tempNode == nullptr){
		delete tempTeam;
		return StatusType::FAILURE;
	}

	delete tempTeam;
	tempTeam = tempNode->value;

	this->teamTreeById->removeValue(tempTeam, compare_team_by_id);
	this->teamTreeByAbility->removeValue(tempTeam, compareTeamsByAbility);

	if(tempTeam->getPlayersCount() > 0){
		tempTeam->getCaptain()->setTeam(nullptr);
	}

	delete tempTeam;

	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
	if(playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0){
		return StatusType::INVALID_INPUT;
	}

	Team* tempTeam = new Team(teamId);
	RankAVLTree<Team>::Node* tempNode = this->teamTreeById->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;
	if(tempNode == nullptr){
		return StatusType::FAILURE;
	}
	Team* teamToAdd = tempNode->value;

	Player* newPlayer = new Player(playerId, gamesPlayed, ability, cards, goalKeeper, spirit);

	if(this->players->search(newPlayer) != nullptr){
		delete newPlayer;
		return StatusType::FAILURE;
	}

	if(teamToAdd->getPlayersCount() == 0){
		teamToAdd->setCaptain(newPlayer);
		newPlayer->setTeam(teamToAdd);
		newPlayer->setCaptain(true);
		teamToAdd->setTeamSpirit(new permutation_t(spirit));
		newPlayer->setSpiritExtra(new permutation_t(spirit));
	}
	else{
		newPlayer->setDaddy(teamToAdd->getCaptain());
		newPlayer->setSpiritExtra(new permutation_t((*(teamToAdd->getCaptain()->getSpiritExtra())).inv() * (*teamToAdd->getTeamSpirit()) * spirit));
		teamToAdd->setTeamSpirit(new permutation_t((*teamToAdd->getTeamSpirit()) * spirit));
		newPlayer->setGamesPlayed(newPlayer->getGamesPlayed() - teamToAdd->getCaptain()->getGamesPlayed());
	}

	this->teamTreeByAbility->removeValue(teamToAdd, compareTeamsByAbility);

	teamToAdd->setPlayersCount(teamToAdd->getPlayersCount() + 1);
	teamToAdd->setTeamAbility(teamToAdd->getTeamAbility() + ability);

	this->teamTreeByAbility->insertValue(teamToAdd, compareTeamsByAbility);

	if(goalKeeper){
		teamToAdd->setGoalKeeper(true);
	}

	this->players->insert(newPlayer);

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
		return StatusType::INVALID_INPUT;
	}

	Team* tempTeam1 = new Team(teamId1);
	RankAVLTree<Team>::Node* tempNode1 = this->teamTreeById->findValue(tempTeam1, compare_team_by_id);
	Team* tempTeam2 = new Team(teamId2);
	RankAVLTree<Team>::Node* tempNode2 = this->teamTreeById->findValue(tempTeam2, compare_team_by_id);

	if(tempNode1 == nullptr || tempNode2 == nullptr){
		delete tempTeam1;
		delete tempTeam2;
		return StatusType::FAILURE;
	}

	if(!tempNode1->value->getGoalKeeper() || !tempNode2->value->getGoalKeeper()){
		delete tempTeam1;
		delete tempTeam2;
		return StatusType::FAILURE;
	}

	delete tempTeam1;
	delete tempTeam2;

	tempNode1->value->getCaptain()->setGamesPlayed(tempNode1->value->getCaptain()->getGamesPlayed() + 1);
	tempNode2->value->getCaptain()->setGamesPlayed(tempNode2->value->getCaptain()->getGamesPlayed() + 1);

	int teamPower1 = tempNode1->value->getTeamAbility() + tempNode1->value->getTeamPoints();
	int teamPower2 = tempNode2->value->getTeamAbility() + tempNode2->value->getTeamPoints();
	int teamSpiritPower1 = tempNode1->value->getTeamSpirit()->strength();
	int teamSpiritPower2 = tempNode2->value->getTeamSpirit()->strength();


	if(teamPower1 > teamPower2){
		tempNode1->value->setTeamPoints(tempNode1->value->getTeamPoints() + 3);
		return 1;
	}
	else if(teamPower1 < teamPower2){
		tempNode2->value->setTeamPoints(tempNode2->value->getTeamPoints() + 3);
		return 3;
	}
	else if(teamSpiritPower1 > teamSpiritPower2){
		tempNode1->value->setTeamPoints(tempNode1->value->getTeamPoints() + 3);
		return 2;
	}
	else if(teamSpiritPower1 < teamSpiritPower2){
		tempNode2->value->setTeamPoints(tempNode2->value->getTeamPoints() + 3);
		return 4;
	}
	else{
		tempNode1->value->setTeamPoints(tempNode1->value->getTeamPoints() + 1);
		tempNode2->value->setTeamPoints(tempNode2->value->getTeamPoints() + 1);
	}

	return 0;
}

// partial spirit = neutral; sum played games = 0
Team* findTeam(Player* player, int &sumPlayedGames, permutation_t &partialSpirit){

	Player* tempPlayer = player; // at end of loop is big daddy
	while(tempPlayer->getDaddy() != nullptr){
		sumPlayedGames += tempPlayer->getGamesPlayed();
		partialSpirit = (*tempPlayer->getSpiritExtra() * partialSpirit);
		tempPlayer = tempPlayer->getDaddy();
	}
	Player* bigDaddy = tempPlayer;
	sumPlayedGames += bigDaddy->getGamesPlayed();
	partialSpirit = (*bigDaddy->getSpiritExtra() * partialSpirit);

	permutation_t tempPerm = bigDaddy->getSpiritExtra()->inv() * permutation_t(partialSpirit);
	int tempPlayedGames = sumPlayedGames - tempPlayer->getGamesPlayed();

	tempPlayer = player;
	Player* nextPlayer = nullptr;
	permutation_t prevPerm;
	int prevPlayedGames;

	while(tempPlayer->getDaddy() != nullptr){
		prevPerm = permutation_t(*(tempPlayer->getSpiritExtra()));
		prevPlayedGames = tempPlayer->getGamesPlayed();

		tempPlayer->setSpiritExtra(new permutation_t(tempPerm));
		tempPlayer->setGamesPlayed(tempPlayedGames);

		tempPerm = tempPerm * prevPerm.inv();
		tempPlayedGames = tempPlayedGames - prevPlayedGames;
	
		nextPlayer = tempPlayer->getDaddy();

		tempPlayer->setDaddy(bigDaddy);
		tempPlayer = nextPlayer;
	}

	return bigDaddy->getTeam();
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Player* tempPlayer = new Player(playerId, 0, 0, 0, 0, permutation_t());
	Player* player = this->players->search(tempPlayer);
	delete tempPlayer;
	if(player == nullptr){
		return StatusType::FAILURE;
	}

	int playedGames = 0;
	permutation_t partialSpirit = permutation_t(permutation_t::neutral());
	findTeam(player, playedGames, partialSpirit);

	return playedGames;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	if(playerId <= 0 || cards < 0){
		return StatusType::INVALID_INPUT;
	}
	
	Player* tempPlayer = new Player(playerId, 0, 0, 0, 0, permutation_t());
	Player* player = this->players->search(tempPlayer);
	delete tempPlayer;
	if(player == nullptr){
		return StatusType::FAILURE;
	}

	int gamesPlayed = 0;
	permutation_t partialSpirit = permutation_t(permutation_t::neutral());
	Team* tempTeam = findTeam(player, gamesPlayed, partialSpirit);

	if(tempTeam == nullptr){
		return StatusType::FAILURE;
	}

	player->setCards(player->getCards() + cards);

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}
	
	Player* tempPlayer = new Player(playerId, 0, 0, 0, 0, permutation_t());
	Player* player = this->players->search(tempPlayer);
	delete tempPlayer;
	if(player == nullptr){
		return StatusType::FAILURE;
	}

	return player->getCards();
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if(teamId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Team* tempTeam = new Team(teamId);
	RankAVLTree<Team>::Node* teamNode = this->teamTreeById->findValue(tempTeam, compare_team_by_id);
	delete tempTeam;
	if(teamNode == nullptr){
		return StatusType::FAILURE;
	}

	
	return teamNode->value->getTeamPoints();
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	if(i < 0){
		return StatusType::FAILURE;
	}
	if(this->teamTreeByAbility->root == nullptr){
		return StatusType::FAILURE;
	}
	if(i >= teamTreeByAbility->root->rank){
		return StatusType::FAILURE;
	}
	
	return this->teamTreeByAbility->searchValue(i)->getId();
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
	if(playerId <= 0){
		return StatusType::INVALID_INPUT;
	}

	Player* tempPlayer = new Player(playerId, 0, 0, 0, 0, permutation_t());
	Player* player = this->players->search(tempPlayer);
	delete tempPlayer;
	if(player == nullptr){
		return StatusType::FAILURE;
	}

	int playedGames = 0;
	permutation_t partialSpirit = permutation_t(permutation_t::neutral());
	Team* team = findTeam(player, playedGames, partialSpirit);

	if(team == nullptr){
		return StatusType::FAILURE;
	}

	return partialSpirit;
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
		return StatusType::INVALID_INPUT;
	}

	Team* tempTeam1 = new Team(teamId1);
	Team* tempTeam2 = new Team(teamId2);
	RankAVLTree<Team>::Node* nodeTeam1 = this->teamTreeById->findValue(tempTeam1, compare_team_by_id);
	RankAVLTree<Team>::Node* nodeTeam2 = this->teamTreeById->findValue(tempTeam2, compare_team_by_id);
	delete tempTeam1;
	delete tempTeam2;

	if(nodeTeam1 == nullptr || nodeTeam2 == nullptr){
		return StatusType::FAILURE;
	}

	tempTeam1 = nodeTeam1->value;
	tempTeam2 = nodeTeam2->value;

	int sizeTeam1 = tempTeam1->getPlayersCount();
	int sizeTeam2 = tempTeam2->getPlayersCount();
	permutation_t* team1Spirit = tempTeam1->getTeamSpirit();
	permutation_t* team2Spirit = tempTeam2->getTeamSpirit();

	// need check empty teams

	if(sizeTeam1 == 0 && sizeTeam2 == 0){
		this->remove_team(teamId2);
		return StatusType::SUCCESS;
	}
	else if(sizeTeam2 == 0){
		this->remove_team(teamId2);
		return StatusType::SUCCESS;
	}
	else if(sizeTeam1 == 0){
		this->teamTreeByAbility->removeValue(tempTeam2, compareTeamsByAbility);
		this->teamTreeByAbility->removeValue(tempTeam1, compareTeamsByAbility);
	
		tempTeam1->setGoalKeeper(tempTeam2->getGoalKeeper() || tempTeam1->getGoalKeeper());
		tempTeam1->setPlayersCount(sizeTeam1 + sizeTeam2);
		tempTeam1->setTeamAbility(tempTeam1->getTeamAbility() + tempTeam2->getTeamAbility());
		tempTeam1->setTeamPoints(tempTeam1->getTeamPoints() + tempTeam2->getTeamPoints());
		tempTeam1->setTeamSpirit(new permutation_t((*team1Spirit) * (*team2Spirit)));
		tempTeam1->setCaptain(tempTeam2->getCaptain());
		tempTeam1->getCaptain()->setTeam(tempTeam1);
		
		this->teamTreeById->removeValue(tempTeam2, compare_team_by_id);
		delete tempTeam2;
		this->teamTreeByAbility->insertValue(tempTeam1, compareTeamsByAbility);
	
		return StatusType::SUCCESS;
	}

	permutation_t* bigDaddy1Extra = tempTeam1->getCaptain()->getSpiritExtra();
	permutation_t* bigDaddy2Extra = tempTeam2->getCaptain()->getSpiritExtra();
	int playedGames1 = tempTeam1->getCaptain()->getGamesPlayed();
	int playedGames2 = tempTeam2->getCaptain()->getGamesPlayed();

	if(sizeTeam1 >= sizeTeam2){
		tempTeam2->getCaptain()->setSpiritExtra(new permutation_t(bigDaddy1Extra->inv() * (*team1Spirit) * (*bigDaddy2Extra)));
		tempTeam2->getCaptain()->setGamesPlayed(playedGames2 - playedGames1);
		tempTeam2->getCaptain()->setDaddy(tempTeam1->getCaptain());
		tempTeam2->getCaptain()->setCaptain(false);
		tempTeam2->getCaptain()->setTeam(nullptr);
	
		this->teamTreeByAbility->removeValue(tempTeam1, compareTeamsByAbility);
		this->teamTreeByAbility->removeValue(tempTeam2, compareTeamsByAbility);

		tempTeam1->setGoalKeeper(tempTeam2->getGoalKeeper() || tempTeam1->getGoalKeeper());
		tempTeam1->setPlayersCount(sizeTeam1 + sizeTeam2);
		tempTeam1->setTeamAbility(tempTeam1->getTeamAbility() + tempTeam2->getTeamAbility());
		tempTeam1->setTeamPoints(tempTeam1->getTeamPoints() + tempTeam2->getTeamPoints());
		tempTeam1->setTeamSpirit(new permutation_t((*team1Spirit) * (*team2Spirit)));
		
		this->teamTreeByAbility->insertValue(tempTeam1, compareTeamsByAbility);
		this->teamTreeById->removeValue(tempTeam2, compare_team_by_id);
		delete tempTeam2;
	}
	else{
		tempTeam2->getCaptain()->setSpiritExtra(new permutation_t((*team1Spirit) * (*bigDaddy2Extra)));
		bigDaddy2Extra = tempTeam2->getCaptain()->getSpiritExtra();
		tempTeam1->getCaptain()->setSpiritExtra(new permutation_t(bigDaddy2Extra->inv() * (*bigDaddy1Extra)));
		tempTeam1->getCaptain()->setGamesPlayed(playedGames1 - playedGames2);
		tempTeam1->getCaptain()->setDaddy(tempTeam2->getCaptain());
		tempTeam1->getCaptain()->setCaptain(false);
		tempTeam1->getCaptain()->setTeam(nullptr);

		this->teamTreeByAbility->removeValue(tempTeam2, compareTeamsByAbility);
		this->teamTreeByAbility->removeValue(tempTeam1, compareTeamsByAbility);
	
		tempTeam1->setGoalKeeper(tempTeam2->getGoalKeeper() || tempTeam1->getGoalKeeper());
		tempTeam1->setPlayersCount(sizeTeam1 + sizeTeam2);
		tempTeam1->setTeamAbility(tempTeam1->getTeamAbility() + tempTeam2->getTeamAbility());
		tempTeam1->setTeamPoints(tempTeam1->getTeamPoints() + tempTeam2->getTeamPoints());
		tempTeam1->setTeamSpirit(new permutation_t((*team1Spirit) * (*team2Spirit)));
		tempTeam1->setCaptain(tempTeam2->getCaptain());
		tempTeam1->getCaptain()->setTeam(tempTeam1);
		
		this->teamTreeById->removeValue(tempTeam2, compare_team_by_id);
		delete tempTeam2;

		this->teamTreeByAbility->insertValue(tempTeam1, compareTeamsByAbility);
	}

	return StatusType::SUCCESS;
}
