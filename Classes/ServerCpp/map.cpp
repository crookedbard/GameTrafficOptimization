#include "map.h"
#include "maps.h"
#include "server.h"
#include "tinyxml2.h"

using namespace std;

Map::Map(int id, int width, int height){
	_id = id;
	_width = width;
	_height = height;

	readMapDataFromXML();
	//tile[0][0].walkable = true;
	//Store important variables..
	Maps::list.push_back(this);

	//Print success message
	server::print("New map initialized [ID: " + server::doubleToString(_id) + "]");
}

//======================================================
//Reads map data from xml file
//======================================================
void Map::readMapDataFromXML()
{
	tinyxml2::XMLDocument doc;
	//vector<pair<int, int> > tile_free; //for mob spawning

	int x = 0;
	int y = 0;
	bool walkable;
	stringstream tempName;
	tempName << "database\\maps\\" + to_string(_id) + ".tmx";
	if (doc.LoadFile(tempName.str().c_str()) == tinyxml2::XML_SUCCESS)
	{
		const char* text = doc.FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->GetText();
		_width = doc.FirstChildElement("map")->FirstChildElement("layer")->IntAttribute("width");
		_height = doc.FirstChildElement("map")->FirstChildElement("layer")->IntAttribute("height");
		//======================================
		// allocation
		tile = new Tile*[_width];
		//create 2d dynamic int array
		for (int i = 0; i < _width; i++)
		{
			tile[i] = new Tile[_height];
		}

		tinyxml2::XMLElement * child = doc.FirstChildElement("map")->FirstChildElement("objectgroup")->FirstChildElement("object")->ToElement();

		for (child; child; child = child->NextSiblingElement())
		{
			
			x = atoi(child->Attribute("x"));
			y = atoi(child->Attribute("y"));
			walkable = !!atoi(child->FirstChildElement("properties")->FirstChildElement("property")->NextSiblingElement()->Attribute("value")  );
			tile[(x / 32)][(y / 32)].walkable = walkable; 
			//cout << tile[(x / 32)][(y / 32)].walkable << endl;
		}

	}
	else
	{
		cout << "failed to open xml" << endl;
	}
}

Map::~Map()
{
	//Destructor

	//Remove from lists
	for (unsigned int i = 0; i<Maps::list.size(); i++)
	{
		if (Maps::list[i] == this)
		{
			Maps::list.erase(Maps::list.begin() + i);
		}
	}

}