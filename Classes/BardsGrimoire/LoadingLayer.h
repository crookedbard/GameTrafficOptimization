#ifndef __LOADINGLAYER_H_
#define __LOADINGLAYER_H_

#include "GameController.h"

class LoadingLayer : public cocos2d::Layer, public GameController
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* createScene(cocos2d::Layer *layer);

	//Handle key events
	//void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
	void onRaise(cocos2d::Ref* pSender);
	// implement the "static node()" method manually
	CREATE_FUNC(LoadingLayer);

	void setPercentage(int pct);
};

#endif // __LOADINGLAYER_H_