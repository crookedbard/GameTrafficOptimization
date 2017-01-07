#include "maps.h"

using namespace std;

vector<Map*> Maps::list;


void Maps::init()
{
	//players::plrs = new players();
	//players_online=0;
	new Map(0, 32, 32);
	new Map(1, 13, 13);
}

Map* Maps::findMap(int id)
{
	for (const auto& child : Maps::list){
		if (child->getId() == id)
		{
			return child;
		}
	}

	//Else nothing found
	return nullptr;
}