#include "GameLayer.h"
#include "Players.h"
//#include "39dll.h"
//#include "Platform.h"
//#include "GSwitch.h"
//#include "MenuLayer.h"

USING_NS_CC;
using namespace std;
//using namespace CocosDenshion;

//static GameLayer* _instance = NULL;

GameLayer::~GameLayer()
{
    
    //CC_SAFE_RELEASE(_platformPool);
    //CC_SAFE_RELEASE(_gSwitchPool);
    //CC_SAFE_RELEASE(_maps);
    CC_SAFE_RELEASE(_player);
    //CC_SAFE_RELEASE(_buttons);
    
    //delete _world;
    //_world = NULL;
    
    //delete m_debugDraw;
}

GameLayer::GameLayer() {
    
    
    //_screenSize = Director::sharedDirector()->getWinSize();
    //_running = false;
    
    //createScreen();
    
    /*std::string levelsFile = FileUtils::sharedFileUtils()->fullPathFromRelativePath("levels.plist");
    _maps = Array::createWithContentsOfFileThreadSafe(levelsFile.c_str());
	_maps->retain();*/
    
    //setTouchEnabled( true );
    
	//if we load this scene first, load sprite frames
	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
	
	//_spriteBatchNode = SpriteBatchNode::create("characters.png");
	//_spriteBatchNode->add("plr_walk.png");
	//this->addChild(_spriteBatchNode);
	//auto plrWalkBatchNode = SpriteBatchNode::create("characters/player/plr_walk.png");
	//this->addChild(plrWalkBatchNode);

	
	//this->schedule(schedule_selector(GameLayer::update));
}

Scene* GameLayer::createScene(Layer *layer)
{
	
	// 'scene' is an autorelease object
	Scene *scene = Scene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	auto gameLayer = (GameLayer *)layer;
	gameLayer->setHudLayer(HudLayer::create());
	scene->addChild(gameLayer->getHudLayer(), kForeground);

	return scene;
}

bool GameLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		//SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("characters.plist", "characters.png");
		SpriteFrameCache::sharedSpriteFrameCache()->destroyInstance();
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("characters/player/plr_stand.plist", "characters/player/plr_stand.png");
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("characters/player/plr_walk.plist", "characters/player/plr_walk.png");
		_lastUpdateTime = 0;
		pressingDL = false;
		pressingDR = false;
		pressingUL = false;
		pressingUR = false;
		this->scheduleUpdate();

		//player::initialize(this);

		bRet = true;
	} while (0);

	return bRet;
}

void GameLayer::update(float dt) {

	//if (_player->getCurrentPos() == _player->getNextPos()) checkKeys();
	/*if (_player->getState() == kStateIdle) { checkKeys(); }
	else if (_player->getState() == kStateMove) { setViewPointPlrCenter(); }*/
	//CCLOG("update since last: %f", dt);
	//return;
	_lastUpdateTime += dt;
	checkKeys();
	//checkReleased();
	
	switch (_player->getState()){
	case kStateIdle: //if (_lastUpdateTime >= static_cast<float>(0.576)){ _lastUpdateTime = 0; /*CCLOG("checkKeys");*/ checkKeys(); }
		break;
	case kStateMove: //if (floor(_player->getPosition().x) != floor(this->getPosition().x) || floor(_player->getPosition().y) != floor(this->getPosition().y)){ setViewPointPlrCenter(); }//setViewPointPlrCenter();/*CCLOG("%f", dt - _lastUpdateTime);*/ //if (_lastUpdateTime > 0.02f) { setViewPointPlrCenter(); _lastUpdateTime = 0; }
		break;
	}

	for (const auto& plr : players::list)
	{
		plr->update(dt);
	}

	if (floor(_player->getPosition().x) != floor(this->getPosition().x) || floor(_player->getPosition().y) != floor(this->getPosition().y)){ setViewPointPlrCenter(); }
}

/*void GameLayer::checkReleased(){

	if (pressingDL){
		if (!_hudLayer->pressedDL || _hudLayer->pressedDR || _hudLayer->pressedUR) {
			pressingDL = false;
			_hudLayer->sendKeyReleased(kDirectionDownLeft);
		}
		return;
	}

	if (pressingDR){
		if (!_hudLayer->pressedDR || _hudLayer->pressedUR || _hudLayer->pressedUL) {
			pressingDR = false;
			_hudLayer->sendKeyReleased(kDirectionDownRight);
		}
		return;
	}

	if (pressingUL){
		if (!_hudLayer->pressedUL || _hudLayer->pressedDL || _hudLayer->pressedDR) {
			pressingUL = false;
			_hudLayer->sendKeyReleased(kDirectionUpLeft);
		}
		return;
	}

	if (pressingUR){
		if (!_hudLayer->pressedUR || _hudLayer->pressedUL || _hudLayer->pressedDL) {
			pressingUR = false;
			_hudLayer->sendKeyReleased(kDirectionUpRight);
		}
		return;
	}

}*/

/*void GameLayer::checkPressed(){

	if ((_hudLayer->pressedDL && _hudLayer->pressedUR) || (_hudLayer->pressedUL && _hudLayer->pressedDR))return;

	if (_hudLayer->pressedUR && _hudLayer->pressedDR) { if (_player->getCurrentPos().y > 0){ pressingUR = true; _hudLayer->sendKeyPressed(kDirectionUpRight); } return; }
	if (_hudLayer->pressedUL && _hudLayer->pressedUR) { if (_player->getCurrentPos().x > 0){ pressingUL = true; _hudLayer->sendKeyPressed(kDirectionUpLeft); } return; }
	if (_hudLayer->pressedDL && _hudLayer->pressedUL) { if (static_cast<int>(_player->getCurrentPos().y) < _mapHeight - 1){ pressingDL = true; _hudLayer->sendKeyPressed(kDirectionDownLeft); } return; }
	if (_hudLayer->pressedDR && _hudLayer->pressedDL) { if (static_cast<int>(_player->getCurrentPos().x) < _mapWidth - 1){ pressingDR = true; _hudLayer->sendKeyPressed(kDirectionDownRight); } return; }

	if (_hudLayer->pressedDL){ if (static_cast<int>(_player->getCurrentPos().y) < _mapHeight - 1){ pressingDL = true; _hudLayer->sendKeyPressed(kDirectionDownLeft); } return; }
	if (_hudLayer->pressedDR){ if (static_cast<int>(_player->getCurrentPos().x) < _mapWidth - 1){ pressingDR = true; _hudLayer->sendKeyPressed(kDirectionDownRight); } return; }
	if (_hudLayer->pressedUL){ if (_player->getCurrentPos().x > 0){ pressingUL = true; _hudLayer->sendKeyPressed(kDirectionUpLeft); } return; }
	if (_hudLayer->pressedUR){ if (_player->getCurrentPos().y > 0){ pressingUR = true; _hudLayer->sendKeyPressed(kDirectionUpRight); } return; }

}*/

void GameLayer::checkKeys(){
	/*
	if ((_hudLayer->pressedDL && _hudLayer->pressedUR) || (_hudLayer->pressedUL && _hudLayer->pressedDR))return;

	if (_hudLayer->pressedUR && _hudLayer->pressedDR) { if (_player->getCurrentPos().y > 0){ _hudLayer->sendKeyPressed(kDirectionUpRight); } return; } //_player->steps.push_back(kDirectionUpRight); 
	if (_hudLayer->pressedUL && _hudLayer->pressedUR) { if (_player->getCurrentPos().x > 0){ _hudLayer->sendKeyPressed(kDirectionUpLeft); } return; } //_player->steps.push_back(kDirectionUpLeft);
	if (_hudLayer->pressedDL && _hudLayer->pressedUL) { if (static_cast<int>(_player->getCurrentPos().y) < _mapHeight - 1){ _hudLayer->sendKeyPressed(kDirectionDownLeft); } return; } //_player->steps.push_back(kDirectionDownLeft);
	if (_hudLayer->pressedDR && _hudLayer->pressedDL) { if (static_cast<int>(_player->getCurrentPos().x) < _mapWidth - 1){ _hudLayer->sendKeyPressed(kDirectionDownRight); } return; } //_player->steps.push_back(kDirectionDownRight);

	if (_hudLayer->pressedDL){ if (static_cast<int>(_player->getCurrentPos().y) < _mapHeight - 1){ _hudLayer->sendKeyPressed(kDirectionDownLeft); } return; }
	if (_hudLayer->pressedDR){ if (static_cast<int>(_player->getCurrentPos().x) < _mapWidth - 1){ _hudLayer->sendKeyPressed(kDirectionDownRight); } return; }
	if (_hudLayer->pressedUL){ if (_player->getCurrentPos().x > 0){ _hudLayer->sendKeyPressed(kDirectionUpLeft); } return; }
	if (_hudLayer->pressedUR){ if (_player->getCurrentPos().y > 0){ _hudLayer->sendKeyPressed(kDirectionUpRight); } return; }

	if (!_hudLayer->pressedDL){ _hudLayer->sendKeyReleased(kDirectionDownLeft); return; }
	if (!_hudLayer->pressedDR){ _hudLayer->sendKeyReleased(kDirectionDownRight); return; }
	if (!_hudLayer->pressedUL){ _hudLayer->sendKeyReleased(kDirectionUpLeft); return; }
	if (!_hudLayer->pressedUR){ _hudLayer->sendKeyReleased(kDirectionUpRight); return; }
	*/
	
	if (pressingDL){
		if (_hudLayer->pressedDL && !_hudLayer->pressedDR && !_hudLayer->pressedUR) {
			if (_lastUpdateTime >= 0.48){ _hudLayer->sendKeyPressed(kDirectionDownLeft); _lastUpdateTime = _lastUpdateTime - 0.48; }
		}
		else{
			pressingDL = false;
			_hudLayer->sendKeyReleased(kDirectionDownLeft);
		}
		return;
	}

	if (pressingDR){
		if (_hudLayer->pressedDR && !_hudLayer->pressedUR && !_hudLayer->pressedUL) {
			if (_lastUpdateTime >= 0.48){ _hudLayer->sendKeyPressed(kDirectionDownRight); _lastUpdateTime = 0; }
		}
		else{
			pressingDR = false;
			_hudLayer->sendKeyReleased(kDirectionDownRight);
		}
		return;
	}

	if (pressingUL){
		if (_hudLayer->pressedUL && !_hudLayer->pressedDL && !_hudLayer->pressedDR) {
			if (_lastUpdateTime >= 0.48){ _hudLayer->sendKeyPressed(kDirectionUpLeft); _lastUpdateTime = 0; }
		}
		else{
			pressingUL = false;
			_hudLayer->sendKeyReleased(kDirectionUpLeft);
		}
		return;
	}

	if (pressingUR){
		if (_hudLayer->pressedUR && !_hudLayer->pressedUL && !_hudLayer->pressedDL) {
			if (_lastUpdateTime >= 0.48){ _hudLayer->sendKeyPressed(kDirectionUpRight); _lastUpdateTime = 0; }
		}
		else{
			pressingUR = false;
			_hudLayer->sendKeyReleased(kDirectionUpRight);
		}
		return;
	}

	if ((_hudLayer->pressedDL && _hudLayer->pressedUR) || (_hudLayer->pressedUL && _hudLayer->pressedDR))return;

	if (_hudLayer->pressedUR && _hudLayer->pressedDR) { if (_player->getCurrentPos().y > 0){ pressingUR = true; _hudLayer->sendKeyPressed(kDirectionUpRight); } return; } //_player->steps.push_back(kDirectionUpRight); 
	if (_hudLayer->pressedUL && _hudLayer->pressedUR) { if (_player->getCurrentPos().x > 0){ pressingUL = true; _hudLayer->sendKeyPressed(kDirectionUpLeft); } return; } //_player->steps.push_back(kDirectionUpLeft);
	if (_hudLayer->pressedDL && _hudLayer->pressedUL) { if (static_cast<int>(_player->getCurrentPos().y) < _mapHeight - 1){ pressingDL = true; _hudLayer->sendKeyPressed(kDirectionDownLeft); } return; } //_player->steps.push_back(kDirectionDownLeft);
	if (_hudLayer->pressedDR && _hudLayer->pressedDL) { if (static_cast<int>(_player->getCurrentPos().x) < _mapWidth - 1){ pressingDR = true; _hudLayer->sendKeyPressed(kDirectionDownRight); } return; } //_player->steps.push_back(kDirectionDownRight);

	if (_hudLayer->pressedDL){ if (static_cast<int>(_player->getCurrentPos().y) < _mapHeight - 1){ pressingDL = true; _hudLayer->sendKeyPressed(kDirectionDownLeft); } return; }
	if (_hudLayer->pressedDR){ if (static_cast<int>(_player->getCurrentPos().x) < _mapWidth - 1){ pressingDR = true; _hudLayer->sendKeyPressed(kDirectionDownRight); } return; }
	if (_hudLayer->pressedUL){ if (_player->getCurrentPos().x > 0){ pressingUL = true; _hudLayer->sendKeyPressed(kDirectionUpLeft); } return; }
	if (_hudLayer->pressedUR){ if (_player->getCurrentPos().y > 0){ pressingUR = true; _hudLayer->sendKeyPressed(kDirectionUpRight); } return; }
	
}
//create Box2D world with default gravity
/*void GameLayer::initMap() {

    //_player = player::create(this);
    //_gameBatchNode->addChild(_player, kMiddleground);
}*/

//
void GameLayer::loadMap(int mapID) {
    
	_mapID = mapID;

	TMXTiledMap* _tileMap;
	
	switch (_mapID){
	case 0: _tileMap = TMXTiledMap::create("eoMap02.tmx");
		break;
	}

	_mapWidth = _tileMap->getTileSize().width / 2;//maybe bug but height returns double the size??
	_mapHeight = _tileMap->getTileSize().height; 
	CCLOG("%d %d", _mapWidth, _mapHeight);

	_tileMap->setAnchorPoint(Vec2(0.5, 1));
	_tileMap->setPosition(Vec2::ZERO);
	addChild(_tileMap, kBackground);

    //clearLayer();
    
	//_currentMap = map;
    
    //resetLevel();
    
    //Dictionary * levelData = (Dictionary *) _maps->objectAtIndex(_currentMap);
    
    //Dictionary * data;
    
}

void GameLayer::addPlayer(Race race, Gender gender, Facing facing, int id, string name, int hairModal, int hairColor, int x, int y){
	auto plr = player::create(race, gender, facing);
	plr->initPlayer(id, name, hairModal, hairColor, x, y);
	//plr->retain();
	
	this->addChild(plr);
	//auto hudLayer = (HudLayer *)this->getChildByName("HudLayer");
	//hudLayer->setPlayer(plr);
}

void GameLayer::setViewPointPlrCenter(){

	auto plrPos = _player->getPosition();
	plrPos.x = floor(plrPos.x);
	plrPos.y = floor(plrPos.y) ;
	//CCLOG("PlrCenter() coords UI: %f %f GL: %f %f", plrPos.x, plrPos.y, _sprPlr->getPositionX(), _sprPlr->getPositionY());
	//CCLOG("visibleSize %f %f", visibleSize.width, visibleSize.height);
	//CCLOG("mapaa pos %f %f", plrPos.x - visibleSize.width / 2, plrPos.y - visibleSize.height / 2 + 16);
	this->setViewPoint(Point(plrPos.x - visibleSize.width / 2, plrPos.y - visibleSize.height / 2 /*+ 16 + 58*/ - 16));
}

void GameLayer::setViewPoint2(Point position){
	//CCLOG("mapaa2 pos %f %f", origin.x + position.x, origin.y + position.y);
	this->setPosition(Point((origin.x + (position.x - visibleSize.width / 2))*-1, (origin.y + (position.y - visibleSize.height / 2 + 16))*-1)); //I dont know why *-1

	//CCLOG("mapaa2 pos %f %f", this->getPositionX(), this->getPositionY());

}

void GameLayer::setViewPoint(Point position){
	//CCLOG("mapaa2 pos %f %f", origin.x + position.x, origin.y + position.y);
	this->setPosition(Point((origin.x + position.x)*-1, (origin.y + position.y)*-1)); //I dont know why *-1

	//CCLOG("mapaa2 pos %f %f", this->getPositionX(), this->getPositionY());

}

void GameLayer::setViewPoint(Node *node){
	this->setPosition(Point((origin.x + node->getPosition().x)*-1, (origin.y + node->getPosition().y)*-1)); //I dont know why *-1
}