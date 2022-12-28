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
	this->teamTable = new HashTable<Team>();
	this->players = new HashTable<Player>();
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
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
	this->teamTable->insert(newTeam);

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

	this->teamTable->remove(tempTeam);

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
	Team* teamToAdd = this->teamTable->search(tempTeam);
	if(teamToAdd == nullptr){
		delete tempTeam;
		return StatusType::FAILURE;
	}
	delete tempTeam;

	Player* newPlayer = new Player(playerId, gamesPlayed, ability, cards, goalKeeper, &spirit);

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
		newPlayer->setSpiritExtra(&((*(teamToAdd->getCaptain()->getSpiritExtra())).inv() * (*teamToAdd->getTeamSpirit()) * spirit));
		teamToAdd->setTeamSpirit(&((*teamToAdd->getTeamSpirit()) * spirit));
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

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	// TODO: Your code goes here
	return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
	// TODO: Your code goes here
	return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
