#ifndef __PLAYERS_H__
#define __PLAYERS_H__

//Additional includes dependancies
#include <iostream>
//#include <string>
#include <vector>

#include "player.h"
#include "timer.h"
#include "utils.h"



//Class declarations below
class player;

//Login manager class, manage all players that are logging in
class players
{
	public:
		players();
		Timer plr_timer;
		static void initialize(); //Initialize(create)monsters
		static void listen(); //Listen for new information on the buffer
		static player* findPlayer(int id); //Find a given player
		static player* findPlayer(std::string username); //Find a given player

		static double acceptSocket; //Current result set for incoming connections
		static std::vector<player*> list; //A player list of the login player objects.

		static players *plrs; //needed for timer
		//global player variables
		//static int plr_exp[99];
		//static int players_online;
};

//Include protection
#endif // __PLAYERS_H__