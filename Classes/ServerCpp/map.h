#ifndef __MAP_H__
#define __MAP_H__

#include <iostream>
#include "game.h"

struct Tile
{
	bool walkable;
};

/*struct warp
{
	int x;
	int y;
	int tp_to_map;
	int tp_to_x;
	int tp_to_y;
};*/

class Map
{
public:

	Map(int id, int width, int height);
	~Map();

	Tile ** tile;

	void readMapDataFromXML();

	CC_SYNTHESIZE(int, _id, Id);
	CC_SYNTHESIZE(int, _width, Width);
	CC_SYNTHESIZE(int, _height, Height);
};

#endif // __MAP_H__