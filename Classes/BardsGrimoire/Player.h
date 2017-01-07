#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "GameConfig.h"
//#include "GameLayer.h"
//class GameLayer;

class player : public cocos2d::Sprite
{
public:

	static const int WALK_FRAMES = 4;

	player(Race race, Gender gender, Facing facing);
	~player();

	static player* create(Race race, Gender gender, Facing facing);
	//int _id;
	//std::string _name;
	//int _character;
	//static cocos2d::Action *actionMove;
	
	//static GameLayer *gameLayer;
	//static void initialize(GameLayer *layer);
	//void initialize(GameLayer *layer);

	//static cocos2d::Action *actionMoveDL;
	//static cocos2d::Action *actionMoveDR;
	//static cocos2d::Action *actionMoveUL;
	//static cocos2d::Action *actionMoveUR;
	
	//void movingDL();
	//void movingDR();
	//void movingUL();
	//void movingUR();

	void move();
	//void moveEnd();
	//void move(/*cocos2d::Node* plr*/); //testing

	
	void initPlayer(int id, std::string name, int hairModal, int hairColor, int x, int y);
	void setLocation(cocos2d::Point position);
	//void move(cocos2d::Point position); //not position later but up down left right

	void update(float dt);

	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(std::string, _name, Name);
	CC_SYNTHESIZE(Race, _race, Race);
	CC_SYNTHESIZE(Gender, _gender, Gender);
	CC_SYNTHESIZE(int, _hairModal, HairModal);
	CC_SYNTHESIZE(int, _hairColor, HairColor);
	CC_SYNTHESIZE(Facing, _facing, Facing);

	//CC_SYNTHESIZE(int, _x, X);
	//CC_SYNTHESIZE(int, _y, Y);
	CC_SYNTHESIZE(State, _state, State);
	CC_SYNTHESIZE(cocos2d::Point, _currentPos, CurrentPos);
	CC_SYNTHESIZE(cocos2d::Point, _nextPos, NextPos);

	//CC_SYNTHESIZE(std::vector<Direction>, _steps, Steps);
	std::vector<Direction> steps;
private:
	//GameLayer *_gameLayer;
	//bool _stepOne;
	//std::vector<Direction> _steps;
	//int _x;
	//int _y;
	int _currentFrame;
	float _lastUpdateTime;
};

#endif // __PLAYER_H__