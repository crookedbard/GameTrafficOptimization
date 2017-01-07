#include "LoadingLayer.h"

USING_NS_CC;
using namespace std;

Scene* LoadingLayer::createScene(Layer *layer)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	//auto layer = CharacterSelectionLayer::create();

	// add layer as a child to scene
	//layer->setName("LoadingLayer");
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool LoadingLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		// Enable touch/click actions
		/*this->setTouchEnabled(true);

		// Enable keypad/keyboard actions
		this->setKeypadEnabled(true);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true; };
		listener->onTouchEnded = CC_CALLBACK_2(LoadingLayer::onTouchEnded, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/
		//Add Login background image
		int r = 1 + (rand() % 4);
		auto backgroundImage = Sprite::create("hud/LoginBackground" + to_string(r) + ".png");
		backgroundImage->setPosition(Point(0, 0));
		backgroundImage->setAnchorPoint(Vec2(0, 0));
		backgroundImage->setScale(visibleSize.width / backgroundImage->getContentSize().width,
			visibleSize.height / backgroundImage->getContentSize().height);
		this->addChild(backgroundImage);
		
		auto label = LabelTTF::create("Loading", "Arial", 23);
		label->setAnchorPoint(Point(0.5, 0.5));
		label->setPosition(VisibleRect::center());
		this->addChild(label);
		
		auto labelPercents = LabelTTF::create(" 0 %", "Arial", 23);
		labelPercents->setAnchorPoint(Point(0.5, 0.5));
		labelPercents->setPosition(Vec2(VisibleRect::center().x + label->getContentSize().width , VisibleRect::center().y));
		this->addChild(labelPercents);
		labelPercents->setName("LabelLoadingPercentage");


		auto raisePercent = MenuItemImage::create(
			"buttons/Ok.png",
			"buttons/OkClicked.png",
			CC_CALLBACK_1(LoadingLayer::onRaise, this));
		raisePercent->setAnchorPoint(Vec2(0, 0));
		raisePercent->setPosition(Point(VisibleRect::left().x + 5, VisibleRect::left().y));

		auto menu = Menu::create(raisePercent, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu);

		bRet = true;
	} while (0);

	return bRet;
}

void LoadingLayer::onRaise(Ref* pSender){
	CCLOG("onRaise");
	auto label = (LabelTTF*)this->getChildByName("LabelLoadingPercentage");
	if (label){
		label->setString(" " + to_string(rand() % 90) + " %");
	}
}

void LoadingLayer::setPercentage(int pct){

	auto label = (LabelTTF*)this->getChildByName("LabelLoadingPercentage");
	if (label){
		label->setString(" " + to_string(pct) + " %");
	}

}