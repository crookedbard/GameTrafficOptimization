#ifndef __CHARACTERCREATION_SCENE_H__
#define __CHARACTERCREATION_SCENE_H__
//#define COCOS2D_DEBUG 1

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "HelloWorldScene.h"
#include "MainmenuScene.h"
#include "Utils.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CharacterCreation : public Layer, public EditBoxDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();
	Size visibleSize;
	Point origin;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Ref* pSender);

	//Handle key events
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);

	void onCreate(Ref* pSender);

	void leftArrowReleased(Ref* pSender);
	void rightArrowReleased(Ref* pSender);
	//Handle editBox return key
	virtual void editBoxReturn(EditBox* editBox);

    // implement the "static create()" method manually
	CREATE_FUNC(CharacterCreation);

private:
	Sprite *_sprPlr;
	int _character;
	Vector<SpriteFrame*> _animFrames;// (3);
	EditBox *m_pEditName;
};

#endif // __CHARACTERCREATION_SCENE_H__
