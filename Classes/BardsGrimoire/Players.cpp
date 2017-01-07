#include "Players.h"

USING_NS_CC;
using namespace std;

vector<player*> players::list;
Vector<players*> players::list2;

player* players::findPlayerById(int id)
{
	//Find a given player with playerid
	for (unsigned int i = 0; i<list.size(); i++)
	{
		if (list[i]->getID() == id)
		{
			return list[i];
		}
	}

	//Else nothing found
	return NULL;
}

/*OtherPlayer* players::findPlayerById2(int id)
{
	//Find a given player with playerid
	for (auto child : list2)
	{
		if (child->Id == id)
		{
			return static_cast<OtherPlayer *>(child);
		}
	}
	//Else nothing found
	return NULL;
}*/

player* players::findPlayerByName(string name)
{
	//Find a given player with playerid
	for (unsigned int i = 0; i<list.size(); i++)
	{
		if (list[i]->getName() == name)
		{
			return list[i];
		}
	}

	//Else nothing found
	return NULL;
}