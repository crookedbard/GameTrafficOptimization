#ifndef __OTHERPLAYER_H__
#define __OTHERPLAYER_H__

#include "cocos2d.h"
#include "GameConfig.h"
#include "Players.h"
//#include "GameLayer.h"

class OtherPlayer : /*public cocos2d::Sprite,*/ public players
{
public:
	OtherPlayer(Race race, Gender gender, Facing facing);
	~OtherPlayer();
	
	static OtherPlayer* create(Race race, Gender gender, Facing facing);
	//virtual bool init();
	//int _id;
	//std::string _name;
	//int _character;
	//static cocos2d::Action *actionMove;
	
	//static GameLayer *gameLayer;
	//static void initialize(GameLayer *layer);

	//static cocos2d::Action *actionMoveDL;
	//static cocos2d::Action *actionMoveDR;
	//static cocos2d::Action *actionMoveUL;
	//static cocos2d::Action *actionMoveUR;

	//void movingDL();
	//void movingDR();
	//void movingUL();
	//void movingUR();

	
	//void move(/*cocos2d::Node* plr*/); //testing

	
	void initPlayer(int id, std::string name, int hairModal, int hairColor, int x, int y);
	
	void update(float dt);

	//CC_SYNTHESIZE(int, _id, ID);

private:
	void setLocation(cocos2d::Point position);
	//void move(cocos2d::Point position); //not position later but up down left right
	void moveFinished();
	//bool _stepOne;
	//std::vector<Direction> _steps;
	//int _x;
	//int _y;

	std::string _name;
	Race _race;
	Gender _gender;
	int _hairModal;
	int _hairColor;
	Facing _facing;

	
	cocos2d::Point _currentPos;
	cocos2d::Point _nextPos;
	State _state;
	int _currentFrame;
};

#endif // __OTHERPLAYER_H__