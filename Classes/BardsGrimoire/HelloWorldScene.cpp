#include "HelloWorldScene.h"
#include "Player.h"

int HelloWorld::_character;
string HelloWorld::_name;
HUDLayer *HelloWorld::layerHUD;
SpriteBatchNode *HelloWorld::_spriteBatchNode;

Scene* HelloWorld::createScene(int character, string name)
{
	
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
	/*layerGame->*/_character = character;
	/*layerGame->*/_name = name;

    // 'layer' is an autorelease object
    auto layerGame = HelloWorld::create();
	
	CCLOG("Char: %d",_character);
	layerHUD = HUDLayer::create();
	//_hudLayerTag = layerHUD->getTag();
    // add layer as a child to scene
    scene->addChild(layerGame);
	scene->addChild(layerHUD,1000);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    /*auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);*/

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    /*auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);*/
    
	////////////////////

	this->setAnchorPoint(Vec2::ZERO);
	//CCLOG("this worldspace %f %f",this->convertToWorldSpace(Vec2::ZERO).x,this->convertToWorldSpace(Vec2::ZERO).y);
	
	////////////////////
	// 4 create a TMX map
	_tileMap = TMXTiledMap::create("eoMap01.tmx");
	//auto map = TMXTiledMap::create("eo_map01_v2.tmx");
	_tileMap->setAnchorPoint(Point(0.5, 1));
	_tileMap->setPosition(Vec2::ZERO);
	addChild(_tileMap, -1);

	TMXObjectGroup *objects = _tileMap->getObjectGroup("Object Layer Functions");
	CCASSERT(NULL != objects, "'objPlr' object group not found");
	auto objPlr = objects->getObject("objPlr");
	CCASSERT(!objPlr.empty(), "PlayerShowUpPoint object not found");

	_x = 0;//objPlr["x"].asInt()/32;
	_y = 0;//objPlr["y"].asInt()/32;
	CCLOG("from map x: %d y: %d obj %d %d", _x,_y,objPlr["x"].asInt(),objPlr["y"].asInt());

	


	//player sprite
	cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("characters.plist","characters.png");
	CCLOG("Char: %d", HelloWorld::_character);
	_sprPlr = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(HelloWorld::_character) + "_m_d"));
	_sprPlr->setAnchorPoint(Point(0.5,0));

	int x=(_x - _y) * 64 / 2 ;//- _sprPlr->getContentSize().width/2;
	int y=((_x + _y) * 32 / 2 +16);// * -1; //open gl coord
	CCLOG("tilew: %d tileh: %d mapw: %f maph: %f", _tileMap->getTileSize().width, _tileMap->getMapSize().height,_tileMap->getMapSize().width,_tileMap->getMapSize().height);
	//CCLOG("x: %d y: %d mapw: %8.2f maph: %8.2f", x, y,_tileMap->getMapSize().width,_tileMap->getMapSize().height);

	auto plrcord = Point(x,y);
	_sprPlr->setPosition(GLCORDS(Point(x,y)));
	CCLOG("Plr start coords UI: %f %f GL: %f %f",plrcord.x,plrcord.y,_sprPlr->getPositionX(),_sprPlr->getPositionY());

	_spriteBatchNode = SpriteBatchNode::create("characters.png");
	_spriteBatchNode->addChild(_sprPlr);
	addChild(_spriteBatchNode);

	/*player* play = player::create(1, "Petras", 6);
	play->setLocation(Point(3, 3));
	_spriteBatchNode->addChild(play);*/
	/*auto _sprPlr2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_1_m_d"));
	_sprPlr2->setAnchorPoint(Point(0.5,0));
	_sprPlr2->setPosition(GLCORDS(Point(100,100)));
	spriteBatchNode->addChild(_sprPlr2);

	auto _sprPlr3 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_4_m_d"));
	_sprPlr3->setAnchorPoint(Point(0.5,0));
	_sprPlr3->setPosition(GLCORDS(Point(-100,100)));
	spriteBatchNode->addChild(_sprPlr3);

	auto _sprPlr4 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_3_f_d"));
	_sprPlr4->setAnchorPoint(Point(0.5,0));
	_sprPlr4->setPosition(GLCORDS(Point(0,0)));
	spriteBatchNode->addChild(_sprPlr4);

	auto _sprPlr5 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_7_f_d"));
	_sprPlr5->setAnchorPoint(Point(0.5,0));
	_sprPlr5->setPosition(GLCORDS(Point(0,200)));
	spriteBatchNode->addChild(_sprPlr5);*/


	/*Vector<SpriteFrame*> animFrames(2);

	auto frame1 = cache->getSpriteFrameByName( "char_6_m_d" );
	auto frame2 = cache->getSpriteFrameByName( "char_6_m_u" );
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);

    auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    auto animate = Animate::create(animation);*/
    

	//_sprPlr = Sprite::create();
	//_sprPlr->setPosition(Point(x,y));

	//_sprPlr->setTextureRect( CCRect(0,0,18,58));
	
	//_sprPlr->runAction(RepeatForever::create(animate));

	//setViewPointCenter(_tileMap->getPosition());
	//this->setPosition(Point(origin.x + _tileMap->getMapSize().width * _tileMap->getTileSize().width / 2,origin.y+visibleSize.height));
	//CCLOG("px: %4.2f py: %4.2f",_sprPlr->getPosition().x,_sprPlr->getPosition().y);
	//CCLOG("mx: %4.2f my: %4.2f",_tileMap->getPosition().x,_tileMap->getPosition().y);

	this->setViewPointPlrCenter();

	//hud after screen realloc

	/*auto _sprPlr6 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_5_f_d"));
	_sprPlr6->setAnchorPoint(Point(0,1));
	_sprPlr6->setPosition(GLCORDS(this->getViewPointTopLeft()));
	spriteBatchNode->addChild(_sprPlr6);

	auto _sprPlr7 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_5_f_d"));
	_sprPlr7->setAnchorPoint(Point(0,0));
	_sprPlr7->setPosition(GLCORDS(this->getViewPointBottomLeft()));
	spriteBatchNode->addChild(_sprPlr7);

	auto _sprPlr8 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_5_f_d"));
	_sprPlr8->setAnchorPoint(Point(1,1));
	_sprPlr8->setPosition(GLCORDS(this->getViewPointTopRight()));
	spriteBatchNode->addChild(_sprPlr8);

	auto _sprPlr9 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_5_f_d"));
	_sprPlr9->setAnchorPoint(Point(1,0));
	_sprPlr9->setPosition(GLCORDS(this->getViewPointBottomRight()));
	spriteBatchNode->addChild(_sprPlr9);*/

	//in header

	//auto corner=this->getScreenBottomLeftCoords();
    //m_tBeginPos = Point(corner.x,corner.y + VisibleRect::top().y - 50);
    //CCLabelTTF *backLabel = CCLabelTTF::create("Back", "Arial", 25);
    //CCMenuItemLabel *pBackMenuItem = CCMenuItemLabel::create(backLabel, this, NULL /* menu_selector(Chat::backCallback)*/);
    //CCMenu *pBackMenu = CCMenu::create(pBackMenuItem, NULL);
    //pBackMenu->setAnchorPoint(Vec2::ZERO);
    //pBackMenu->setPosition(Point(corner.x , corner.y+50 ));
	//CCLOG("chat %f %f",corner.x , corner.y+50 );
    //addChild(pBackMenu, 10);

	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true;};
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	this->setKeypadEnabled(true);
	//this->setKeyboardEnabled(true);

    return true;
}

void HelloWorld::onKeyReleased( EventKeyboard::KeyCode keycode, Event *event){
	//if(keycode==EventKeyboard::KeyCode::KEY_BACK) Director::getInstance()->end();
	//HUDLayer* hud = (HUDLayer*)(this->getParent()->getChildByTag(_hudLayerTag));

	switch(keycode){
		case EventKeyboard::KeyCode::KEY_BACK: 
			CCLOG("KEY_BACK");
			//sSocketMgr.disconnect(1);
			layerHUD->connected = false;
			//hud->connected = false;
			//Director::getInstance()->replaceScene(MainMenu::createScene());
			gotoLogin();
			break;
		case EventKeyboard::KeyCode::KEY_MENU : 

			MessageBox("message goes here","MSG TITLE");
			break;
		case EventKeyboard::KeyCode::KEY_SPACE : 

			MessageBox("message goes here","MSG TITLE");
			break;
	}
}

void HelloWorld::movePlr() {
	int x=(_x - _y) * 64 / 2 /*- _sprPlr->getContentSize().width/2*/;
	int y=((_x + _y) * 32 / 2 +16) ;//* -1; //open gl coord
	_sprPlr->setPosition(GLCORDS(Point(x,y)));

}

void HelloWorld::setViewPointMapTopLeft(){
	this->setPosition(Point(origin.x + _tileMap->getMapSize().width * _tileMap->getTileSize().width / 2,origin.y+visibleSize.height));
}


void HelloWorld::setViewPointPlrCenter(){
	/*auto plrPos=_sprPlr->getPosition();
	this->setPosition(Point((origin.x + plrPos.x - visibleSize.width / 2)*-1 ,(origin.y + plrPos.y - visibleSize.height / 2)*-1 ));
	CCLOG("map pos %f %f",(origin.x + plrPos.x - visibleSize.width / 2)*-1,(origin.y + plrPos.y - visibleSize.height / 2)*-1);
	*/
	auto plrPos=/*UICORDS(*/_sprPlr->getPosition()/*)*/;
	CCLOG("PlrCenter() coords UI: %f %f GL: %f %f",plrPos.x,plrPos.y,_sprPlr->getPositionX(),_sprPlr->getPositionY());
	CCLOG(  "visibleSize %f %f"  ,  visibleSize.width  ,  visibleSize.height  );
	CCLOG(  "mapaa pos %f %f"  ,  plrPos.x - visibleSize.width / 2  ,  plrPos.y - visibleSize.height / 2 +16 );
	this->setViewPoint(  Point(  plrPos.x - visibleSize.width / 2  ,  plrPos.y - visibleSize.height / 2 +16 )  );
}

/*Point HelloWorld::getScreenBottomLeftCoords(){
	auto plrPos=_sprPlr->getPosition();
	return Point(origin.x + plrPos.x - visibleSize.width / 2, origin.y + plrPos.y - visibleSize.height / 2 );
}*/

///very weird  this->setPosition works strange, but 
void HelloWorld::setViewPoint(Point position){
	CCLOG(  "mapaa2 pos %f %f"  ,  origin.x + position.x  ,  origin.y + position.y );
	this->setPosition(  Point(  (origin.x + position.x )*-1 , ( origin.y + position.y)*-1  )  ); //I dont know why *-1

	CCLOG(  "mapaa2 pos %f %f"  ,  this->getPositionX()  , this->getPositionY()  );

	//auto testuoju = this->getViewPoint();
	//CCLOG(  "mapaa2 pos %f %f"  ,  testuoju.x  , testuoju.y  );

	//reallocate hud here
}

Point HelloWorld::getViewPointTopLeft(){
	return Point(  ( origin.x + this->getPositionX() ) * -1 ,  ( origin.y + this->getPositionY() ) * -1   );
	//return UICORDS(  Point(  origin.x + this->getPositionX()  ,  origin.y + this->getPositionY() + visibleSize.height  )  );
}

Point HelloWorld::getViewPointBottomLeft(){
	return Point(  ( origin.x + this->getPositionX() ) * -1 ,  ( origin.y + this->getPositionY() ) * -1 +visibleSize.height   );
}

Point HelloWorld::getViewPointTopRight(){
	return Point(  ( origin.x + this->getPositionX() ) * -1 + visibleSize.width,  ( origin.y + this->getPositionY() ) * -1  );
}

Point HelloWorld::getViewPointBottomRight(){
	return Point(  ( origin.x + this->getPositionX() ) * -1 + visibleSize.width,  ( origin.y + this->getPositionY() ) * -1 + visibleSize.height );
}

/*Vec2 HelloWorld::UICords(Vec2 coords){
	return Point(coords.x,coords.y*-1);
}*/

void HelloWorld::createOtherPlayer(int character, string name)
{
	auto _sprOtherPlr = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(character)+ "_m_d"));
	_sprOtherPlr->setAnchorPoint(Point(0.5, 0));

	int _x = 2;
	int _y = 2;
	int x = (_x - _y) * 64 / 2;//- _sprPlr->getContentSize().width/2;
	int y = ((_x + _y) * 32 / 2 + 16);// * -1; //open gl coord

	_sprOtherPlr->setPosition(GLCORDS(Point(x, y)));
	_spriteBatchNode->addChild(_sprOtherPlr);
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	

	touchLocation = this->convertToNodeSpace(touchLocation);
	//touchLocation = this->convertToWorldSpace(touchLocation);
	CCLOG(" TouchLocation X=%f TouchLocation Y=%f",touchLocation.x,touchLocation.y);
	auto playerPos = _sprPlr->getPosition();
	auto diff = touchLocation - playerPos;

	if (diff.x > 0 && diff.y>0) {
		//CCLOG("%s","UPRIGHT");
		_sprPlr->setFlippedX(true);
		_sprPlr->setSpriteFrame(cache->getSpriteFrameByName("char_" + to_string(HelloWorld::_character) + "_m_u"));
		if(_y>0) _y--;
	}else if (diff.x < 0 && diff.y>0) {
		//CCLOG("%s","UPLEFT");
		_sprPlr->setFlippedX(false);
		_sprPlr->setSpriteFrame(cache->getSpriteFrameByName("char_" + to_string(HelloWorld::_character) + "_m_u"));
		if(_x>0) _x--;
	}else if (diff.x > 0 && diff.y<0) {
		//CCLOG("%s","DOWNRIGHT");
		_sprPlr->setFlippedX(true);
		_sprPlr->setSpriteFrame(cache->getSpriteFrameByName("char_" + to_string(HelloWorld::_character) + "_m_d"));
		if(_x<_tileMap->getMapSize().width-1) _x++;
	}else {
		//CCLOG("%s","DOWNLEFT");
		_sprPlr->setFlippedX(false);
		_sprPlr->setSpriteFrame(cache->getSpriteFrameByName("char_" + to_string(HelloWorld::_character) + "_m_d"));
		if(_y<_tileMap->getMapSize().height-1) _y++;
	}
	CCLOG("plrX: %d plrY: %d",_x,_y);
	this->movePlr();
	//this->setViewPointCenter(_sprPlr->getPosition());
	this->setViewPointPlrCenter();
	//this->setViewPointCenter(_sprPlr->getPosition());
}

/*
void HelloWorld::process_login(int _tag, WorldPacket & packet)
  {
      CCLOG("process_login len:%u", packet.size());
      // 定义协议包
      WorldPacket newP;
      newP.clear();    
      newP.SetOpcode(0x00B6);// 设置协议头
      newP    << uint16(0x00B6) 
              << uint16(0);// 协议长度
      newP.SetLength(newP.size());// 设置协议长度    
      sSocketMgr.SendPacket(1, &newP);// 发送数据
	  
  }
  
  void HelloWorld::process_openbackpack(int _tag, WorldPacket & packet)
  {
      CCLOG("process_openbackpack len:%u", packet.size());
  }
  
  bool HelloWorld::process_all(int _tag, int _cmd, WorldPacket & packet)
  {
      CCLOG("process_all _tag:%u, _cmd:%u, len:%u", _tag, _cmd, packet.size());
      return false;
  }
  
  void HelloWorld::OnConnect(int _tag, bool isConnect)
  {
      // Define the protocol packet
		WorldPacket packet;
		packet.clear();
		// Set the protocol header
		packet.SetOpcode(0x0010);
		packet    <<uint16(0x0010) 
				<<uint16(0)// Protocol length
				<<uint32(1) 
				<<uint32(0);
		// Add the string data (uint8 string representing the length of the byte, 1 bytes)
		packet.AppendPacketString<uint8>(std::string("aaa:88889083:d5956683c17d7e284d33ee295b277b52"));
		// The agreement set length
		packet.SetLength(packet.size());
		// To send data
		sSocketMgr.SendPacket(1, &packet);
      CCLOG("OnConnect:%u, isConnect[%u]", _tag, isConnect);
  }
  
  void HelloWorld::onDisconnect(int _tag)
  {
      CCLOG("desconnect:%u", _tag);
  }
  */
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
