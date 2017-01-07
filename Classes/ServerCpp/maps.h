#ifndef __MAPS_H__
#define __MAPS_H__

#include <iostream>
#include <vector>
#include "game.h"
#include "map.h"

//Class declarations below
class Map;

//Login manager class, manage all players that are logging in
class Maps
{
public:
	static void init();
	static Map* findMap(int id); //Find a given player
	//static Map* findPlayer(std::string name); //Find a given player

	static std::vector<Map*> list;
};

#endif // __MAPS_H__