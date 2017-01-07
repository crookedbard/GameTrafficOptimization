#include "HudLayer.h"
#include "39dll.h"

USING_NS_CC;
using namespace std;

bool HudLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		pressedDL = false;
		pressedDR = false;
		pressedUL = false;
		pressedUR = false;

		// Enable keypad/keyboard actions
		//this->setKeypadEnabled(true);

		auto touchListener = EventListenerTouchOneByOne::create();
		//touchListener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true; };
		touchListener->onTouchBegan = CC_CALLBACK_2(HudLayer::onTouchBegan, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(HudLayer::onTouchEnded, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		auto keyboardListener = EventListenerKeyboard::create();
		keyboardListener->onKeyPressed = CC_CALLBACK_2(HudLayer::onKeyPressed, this);
		keyboardListener->onKeyReleased = CC_CALLBACK_2(HudLayer::onKeyReleased, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
		/*
		auto downLeft = MenuItemImage::create(
			"buttons/DPadLarge.png",
			"buttons/DPadLargeClicked.png",
			CC_CALLBACK_1(HudLayer::onDownLeft, this));
		downLeft->setAnchorPoint(Vec2(0, 0));
		downLeft->setPosition(Point(VisibleRect::leftBottom().x + 2, VisibleRect::leftBottom().y + 2));
		
		auto downRight = MenuItemImage::create(
			"buttons/DPadLarge.png",
			"buttons/DPadLargeClicked.png",
			CC_CALLBACK_1(HudLayer::onDownRight, this));
		downRight->setAnchorPoint(Vec2(0, 0));
		downRight->setPosition(Point(VisibleRect::leftBottom().x + 2 * 2 + 58, VisibleRect::leftBottom().y + 2));

		auto upLeft = MenuItemImage::create(
			"buttons/DPadLarge.png",
			"buttons/DPadLargeClicked.png",
			CC_CALLBACK_1(HudLayer::onUpLeft, this));
		upLeft->setAnchorPoint(Vec2(0, 0));
		upLeft->setPosition(Point(VisibleRect::leftBottom().x + 2, VisibleRect::leftBottom().y + 2 + 58));

		auto upRight = MenuItemImage::create(
			"buttons/DPadLarge.png",
			"buttons/DPadLargeClicked.png",
			CC_CALLBACK_1(HudLayer::onUpRight, this));
		upRight->setAnchorPoint(Vec2(0, 0));
		upRight->setPosition(Point(VisibleRect::leftBottom().x + 2 * 2 + 58, VisibleRect::leftBottom().y + 2 + 58));
		*/
		auto sprDownLeft = Sprite::create("buttons/DPadLarge.png");
		sprDownLeft->setAnchorPoint(Vec2(0, 0));
		sprDownLeft->setPosition(Point(VisibleRect::leftBottom().x + 2, VisibleRect::leftBottom().y + 2));
		sprDownLeft->setName("DownLeft");
		this->addChild(sprDownLeft);

		auto sprDownRight = Sprite::create("buttons/DPadLarge.png");
		sprDownRight->setAnchorPoint(Vec2(0, 0));
		sprDownRight->setPosition(Point(VisibleRect::leftBottom().x + 2 * 2 + 58, VisibleRect::leftBottom().y + 2));
		sprDownRight->setName("DownRight");
		this->addChild(sprDownRight);

		auto sprUpLeft = Sprite::create("buttons/DPadLarge.png");
		sprUpLeft->setAnchorPoint(Vec2(0, 0));
		sprUpLeft->setPosition(Point(VisibleRect::leftBottom().x + 2, VisibleRect::leftBottom().y + 2 + 2 + 58));
		sprUpLeft->setName("UpLeft");
		this->addChild(sprUpLeft);

		auto sprUpRight = Sprite::create("buttons/DPadLarge.png");
		sprUpRight->setAnchorPoint(Vec2(0, 0));
		sprUpRight->setPosition(Point(VisibleRect::leftBottom().x + 2 * 2 + 58, VisibleRect::leftBottom().y + 2 + 2 + 58));
		sprUpRight->setName("UpRight");
		this->addChild(sprUpRight);

		//auto menu = Menu::create(downLeft, downRight, upLeft, upRight, NULL);
		//menu->setPosition(Point::ZERO);
		//menu->setAnchorPoint(Point::ZERO);
		//this->addChild(menu);

		vector<string> messages_all = { "chatas", "testas" };
		auto chatSprite = Sprite::create("hud/ChatWindow.png");
		auto chatTable = new ChatTableView(chatSprite, 290, 125, messages_all);
		//chat->setAnchorPoint(Point(0, 0));
		//chat->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 ));
		//chat->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		//this->addChild(chat);
		//this->addChild(chat);
		chatSprite->setAnchorPoint(Point(0, 0));
		chatSprite->setPosition(Point(VisibleRect::leftBottom().x + 2, VisibleRect::leftBottom().y + 2 + 2 + chatSprite->getContentSize().height));
		this->addChild(chatSprite);

		bRet = true;
	} while (0);

	return bRet;
}

/*void HudLayer::setPlayer(player *plr){
	//_gameLayer = gameLayer;
	this->_player = plr;// gameLayer->getPlayer();
	//this->scheduleUpdate();
	//this->schedule(schedule_selector(HudLayer::update));
}*/

void HudLayer::onDownLeft(Ref* pSender)
{
	//_player->steps.push_back(kDirectionDownLeft);
	CCLOG("DownLeft");
}

void HudLayer::onDownRight(Ref* pSender)
{
	//_player->steps.push_back(kDirectionDownRight);
	CCLOG("DownRight");
}

void HudLayer::onUpLeft(Ref* pSender)
{
	//_player->steps.push_back(kDirectionUpLeft);
	CCLOG("UpLeft");
}

void HudLayer::onUpRight(Ref* pSender)
{
	//_player->steps.push_back(kDirectionUpRight);
	CCLOG("UpRight");
}

void HudLayer::sendKeyPressed(Direction direction){
	clearbuffer(); 
	switch (direction){
	case kDirectionDownLeft: writebyte(MSG_PLRMOVEDL);
		break;
	case kDirectionDownRight: writebyte(MSG_PLRMOVEDR);
		break;
	case kDirectionUpLeft: writebyte(MSG_PLRMOVEUL);
		break;
	case kDirectionUpRight: writebyte(MSG_PLRMOVEUR);
		break;
	}
	sendmessage(serverSocket);
}

void HudLayer::sendKeyReleased(Direction direction){
	clearbuffer();
	switch (direction){
	case kDirectionDownLeft: writebyte(MSG_PLRSTOPDL);
		break;
	case kDirectionDownRight: writebyte(MSG_PLRSTOPDR);
		break;
	case kDirectionUpLeft: writebyte(MSG_PLRSTOPUL);
		break;
	case kDirectionUpRight: writebyte(MSG_PLRSTOPUR);
		break;
	}
	sendmessage(serverSocket);
}

void HudLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (pressedDL)pressedDL = false;
	if (pressedDR)pressedDR = false;
	if (pressedUL)pressedUL = false;
	if (pressedUR)pressedUR = false;
}

bool HudLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);

	Sprite *spr = (Sprite *)this->getChildByName("DownLeft");
	if (spr) {
		Rect rect = spr->getBoundingBox();
		if (rect.containsPoint(touchLocation)) {
			pressedDL = true;
			return true;
		}
		else{
			if (pressedDL)pressedDL = false;
		}
	}

	spr = (Sprite *)this->getChildByName("DownRight");
	if (spr) {
		Rect rect = spr->getBoundingBox();
		if (rect.containsPoint(touchLocation)) {
			pressedDR = true;
			return true;
		}
		else{
			if (pressedDR)pressedDR = false;
		}
	}

	spr = (Sprite *)this->getChildByName("UpLeft");
	if (spr) {
		Rect rect = spr->getBoundingBox();
		if (rect.containsPoint(touchLocation)) {
			pressedUL = true;
			return true;
		}
		else{
			if (pressedUL)pressedUL = false;
		}
	}

	spr = (Sprite *)this->getChildByName("UpRight");
	if (spr) {
		Rect rect = spr->getBoundingBox();
		if (rect.containsPoint(touchLocation)) {
			pressedUR = true;
			return true;
		}
		else{
			if (pressedUR)pressedUR = false;
		}
	}

	return true;
}
/*void HudLayer::update(float dt) {

	//if (_player->getCurrentPos() == _player->getNextPos()){
		if ((_pressedDL && _pressedUR) || (_pressedUL && _pressedDR)){
			_pressedDL = false;
			_pressedDR = false;
			_pressedUL = false;
			_pressedUR = false;
		}*/
		/*else{
			if (_pressedDL)_player->steps.push_back(kDirectionDownLeft);
			else if (_pressedDR)_player->steps.push_back(kDirectionDownRight);
			else if (_pressedUL)_player->steps.push_back(kDirectionUpLeft);
			else if (_pressedUR)_player->steps.push_back(kDirectionUpRight);
		}*/
	//}
//}

void HudLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event *event){

	switch (keycode){
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW: pressedDL = true; //CCLOG("DOWN");
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: pressedDR = true; //CCLOG("RIGHT");
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW: pressedUR = true; //CCLOG("UP");
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW: pressedUL = true; //CCLOG("LEFT");
		break;
	default: return;
		break;
	}

	/*if ((pressedDL && pressedUR) || (pressedUL && pressedDR)){
		if (pressedDL)pressedDL = false;
		if (pressedDR)pressedDR = false;
		if (pressedUL)pressedUL = false;
		if (pressedUR)pressedUR = false;

		//clearbuffer();
		//writebyte(MSG_PLRSTOP);
		//sendmessage(serverSocket);
	}*/
	//if (pressedUR && pressedDR) pressedDR = false;
	//if (pressedUL && pressedUR) pressedUR = false;
	//if (pressedDL && pressedUL) pressedUL = false;
	//if (pressedDR && pressedDL) pressedDL = false;

}

void HudLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event){

	switch (keycode){
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW: pressedDL = false;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: pressedDR = false;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW: pressedUR = false;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW: pressedUL = false;
		break;

	case EventKeyboard::KeyCode::KEY_BACK:
	{
		Touch *touch = new Touch();
		touch->setTouchInfo(0, 0.0f, 0.0f);
		onTouchEnded(touch, NULL);
	}
		break;
	case EventKeyboard::KeyCode::KEY_MENU:
	{
		Touch *touch = new Touch();
		touch->setTouchInfo(0, 0.0f, 0.0f);
		onTouchEnded(touch, NULL);
	}
		MessageBox("message goes here", "MSG TITLE");
		break;
	case EventKeyboard::KeyCode::KEY_GRAVE:
	{
											 Touch *touch = new Touch();
											 touch->setTouchInfo(0, 0.0f, 0.0f);
											 onTouchEnded(touch, NULL);
	}
		MessageBox("message goes here", "MSG TITLE");
		break;
	}
}