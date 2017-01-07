#ifndef __SPLASHLAYER_H_
#define __SPLASHLAYER_H_

#include "GameController.h"

class SplashLayer : public cocos2d::Layer, public GameController
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* createScene();

	//Handle key events
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);

	// implement the "static node()" method manually
	CREATE_FUNC(SplashLayer);

};

#endif // __SPLASHLAYER_H_