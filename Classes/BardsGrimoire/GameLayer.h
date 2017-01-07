#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "GameController.h"
#include "Player.h"
#include "HudLayer.h"
//#include "Igloo.h"

//#include "SimpleAudioEngine.h"

//USING_NS_CC;
//using namespace CocosDenshion;

class GameLayer : public cocos2d::Layer, public GameController {
public:
    
    ~GameLayer();
    GameLayer();
    
	virtual bool init();

	static cocos2d::Scene* createScene(cocos2d::Layer *layer);
    
	bool pressingDL;
	bool pressingDR;
	bool pressingUL;
	bool pressingUR;

    //virtual void draw();
    //virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    //virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

    //void update(float dt);
	
	void addPlayer(Race race, Gender gender, Facing facing, int id, std::string name, int hairModal, int hairColor, int x, int y);
	
	//void removePlayer();
    //void newLevel(void);
	CREATE_FUNC(GameLayer);

	void loadMap(int mapID);
	void setViewPointPlrCenter();
	void setViewPoint(cocos2d::Node *node);
	void setViewPoint2(cocos2d::Point position);

	//void checkReleased();
	//void checkPressed();

	CC_SYNTHESIZE(player *, _player, Player); //needs game controller
	//CC_SYNTHESIZE(int, _mapWidth, MapWidth);
	//CC_SYNTHESIZE(int, _mapHeight, MapHeight);
	CC_SYNTHESIZE(HudLayer *, _hudLayer, HudLayer); //needs static scene constructor
private:
    
	void update(float dt);
	
	void setViewPoint(cocos2d::Point position);
	
	void checkKeys();
	

	int _mapWidth;
	int _mapHeight;
	//HudLayer *_hudLayer;
	//cocos2d::SpriteBatchNode * _spriteBatchNode;
    
	//player * _player;
	int _mapID;
	//float _previousUpdateTime;
	float _lastUpdateTime;
	//cocos2d::Sprite * _btnStart;
	//cocos2d::Sprite * _btnReset;
	//cocos2d::Sprite * _btnPause;
	//cocos2d::Sprite * _btnAgain;
	//cocos2d::Sprite * _btnMenu;
	//cocos2d::CCLabelBMFont * _messages;
	//cocos2d::CCLabelTTF * _tutorialLabel;
    
	//cocos2d::ParticleSystem * _smoke;
    
	//cocos2d::Array * _platformPool;
	//cocos2d::Array * _gSwitchPool;
	//cocos2d::Array * _maps;
	//cocos2d::Array * _buttons;
    
    
    //Igloo * _igloo;
    
	//cocos2d::Size _screenSize;
    //bool _running;
    //int _gameState;
    //int _currentMap;
    
    //void initMap();
    //void createScreen(void);
    //void resetMap(void);
    //void levelCompleted(void);
    //void createPools(void);
    //void clearLayer(void);
    
};

#endif // __GAMELAYER_H__
