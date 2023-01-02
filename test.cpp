#include "worldcup23a2.h"

int main(){
    world_cup_t* obj = new world_cup_t();
    StatusType res = obj->add_team(1);
        
    res = obj->add_team(2);
        
    res = obj->add_team(3);
        
    res = obj->add_team(4);
        
    res = obj->add_team(5);
        
    res = obj->add_team(6);
        
    res = obj->add_team(7);
        
    res = obj->add_team(8);
        
    res = obj->add_player(1001, 1, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(1002, 1, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(1003, 1, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(1004, 1, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(1005, 1, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(2001, 2, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(2002, 2, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(2003, 2, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(3001, 3, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(4001, 4, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(4002, 4, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(4003, 4, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(5001, 5, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(6001, 6, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(7001, 7, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(7002, 7, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(8001, 8, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(8002, 8, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(8003, 8, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->add_player(8004, 8, permutation_t::neutral(), 1, 1, 0, true);
        
    res = obj->buy_team(2, 3);
        
    res = obj->buy_team(6, 1);
    
    res = obj->buy_team(7, 8);
        
    res = obj->buy_team(5, 4);
        
    res = obj->buy_team(2, 7);
        
    res = obj->buy_team(5, 6);
        
    res = obj->buy_team(2, 5);
    
    delete obj;
}