#ifndef __HUDLAYER_H_
#define __HUDLAYER_H_

#include "GameController.h"
#include "ChatTableView.h"
//#include "GameLayer.h"
//#include "Player.h"

class HudLayer : public cocos2d::Layer, public GameController
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	//static cocos2d::Scene* createScene();
	bool pressedDL;
	bool pressedDR;
	bool pressedUL;
	bool pressedUR;

	void onDownLeft(cocos2d::Ref* pSender);
	void onDownRight(cocos2d::Ref* pSender);
	void onUpLeft(cocos2d::Ref* pSender);
	void onUpRight(cocos2d::Ref* pSender);

	void sendKeyPressed(Direction direction);
	void sendKeyReleased(Direction direction);
	//void setPlayer(player *plr);

	//Handle key events
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	//Handle touch events
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	//void update(float dt);

	// implement the "static node()" method manually
	CREATE_FUNC(HudLayer);

	//CC_SYNTHESIZE(player *, _player, Player);

private:
	//GameLayer *_gameLayer;
	//player *_player;
	/*bool _pressedDL;
	bool _pressedDR;
	bool _pressedUL;
	bool _pressedUR;*/
	
};

#endif // __HUDLAYER_H_