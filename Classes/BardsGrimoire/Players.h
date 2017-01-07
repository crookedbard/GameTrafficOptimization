#ifndef __PLAYERS_H__
#define __PLAYERS_H__

#include <iostream>
#include "Player.h"
//#include "OtherPlayer.h"
#include <vector>

class player;

class players : public cocos2d::Sprite
{
public:
	

	static std::vector<player*> list;
	static player* findPlayerById(int id);
	static player* findPlayerByName(std::string name);

	static cocos2d::Vector<players *> list2;
	//static OtherPlayer* findPlayerById2(int id);

	int Id;
	std::vector<Direction> steps;
private:

};

#endif // __PLAYERS_H__