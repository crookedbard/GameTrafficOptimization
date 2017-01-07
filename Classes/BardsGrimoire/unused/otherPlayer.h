#ifndef __OTHERPLAYER_H__
#define __OTHERPLAYER_H__

#include <iostream>

class otherPlayer
{
public:
	otherPlayer();
	~otherPlayer();

private:
	int _id;
	std::string _name;
	int _character;
	int _x;
	int _y;
};

#endif // __OTHERPLAYER_H__