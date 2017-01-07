#ifndef __MAINMENU_SCENE_H_
#define __MAINMENU_SCENE_H_

#include "cocos2d.h"
#include "CharacterCreation.h"
//#include "HelloWorldScene.h"

USING_NS_CC;

class MainMenu : public Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static Scene* createScene();
	
	// functions for click even handling
	void onNewGame(Ref* pSender);
	void onOptions(Ref* pSender);
	void onQuit(Ref* pSender);

	//Handle key events
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);

	// implement the "static node()" method manually
    CREATE_FUNC(MainMenu);
};

#endif // __MAINMENU_SCENE_H_
