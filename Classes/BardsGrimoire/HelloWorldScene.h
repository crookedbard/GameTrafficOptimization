#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
//#define COCOS2D_DEBUG 1

//#include "cocos2d.h"
//#include "CharacterCreation.h"
#include "HUDLayerOld.h"
//#include "Utils.h"
#include "GameController.h"

USING_NS_CC;
using namespace std;

class HelloWorld : public Layer, public GameController
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene(int character, string name);
	Size visibleSize;
	Point origin;

	static int _character;
	static string _name;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	void onTouchEnded(Touch *touch, Event *unused_event);
	//bool onTouchBegun(CCTouch* touch, CCEvent* event);
	void setViewPointCenter(Point position);
	void movePlr();
	void setViewPointMapTopLeft();
	void setViewPointPlrCenter();
	void onKeyReleased( EventKeyboard::KeyCode keycode, Event *event);

	static void createOtherPlayer(int character, string name);
	//wherever not my function, you should call UICords to convert it to UI cordinate
	//Point getScreenBottomLeftCoords();
	//Point getScreenTopLeftCoords();
	void setViewPoint(Point position);
	Point getViewPointTopLeft();
	Point getViewPointTopRight();
	Point getViewPointBottomLeft();
	Point getViewPointBottomRight();


private:
	TMXTiledMap *_tileMap;
	TMXLayer *_tileLayer;
	Sprite *_sprPlr;
	SpriteFrameCache *cache;
	static SpriteBatchNode *_spriteBatchNode;
	Vector<Sprite*> *_otherPlayers;

	int _x;
	int _y;
	
	//static int _hudLayerTag;
	static class HUDLayer *layerHUD;
};

#endif // __HELLOWORLD_SCENE_H__
