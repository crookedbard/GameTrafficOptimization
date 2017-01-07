#include "CharacterSelectionLayer.h"
#include "39dll.h"

USING_NS_CC;
using namespace std;

CharacterSelectionLayer::~CharacterSelectionLayer(){
	CC_SAFE_RELEASE(_characterCreationLayer);
}

Scene* CharacterSelectionLayer::createScene(Layer *layer)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	//auto layer = CharacterSelectionLayer::create();

	// add layer as a child to scene
	layer->setName("CharacterSelectionLayer");
	scene->addChild(layer);

	// return the scene
	return scene;
}

/*CharacterSelectionLayer::~CharacterSelectionLayer(){
	CCLOG("Destructor");
	if (_slotData != nullptr){ delete [] _slotData; _slotData = nullptr; }
}*/

bool CharacterSelectionLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		// Enable touch/click actions
		//this->setTouchEnabled(true);

		// Enable keypad/keyboard actions
		this->setKeypadEnabled(true);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true; };
		listener->onTouchEnded = CC_CALLBACK_2(CharacterSelectionLayer::onTouchEnded, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		//Add Login background image
		int r = 1 + (rand() % 4);
		auto backgroundImage = Sprite::create("hud/LoginBackground" + to_string(r) + ".png");
		backgroundImage->setPosition(Point(0, 0));
		backgroundImage->setAnchorPoint(Vec2(0, 0));
		backgroundImage->setScale(visibleSize.width / backgroundImage->getContentSize().width,
			visibleSize.height / backgroundImage->getContentSize().height);
		this->addChild(backgroundImage);

		//Add Login picture
		r = 1 + (rand() % 4);
		auto loginImage = Sprite::create("hud/LoginPicture" + to_string(r) + ".png");
		loginImage->setPosition(VisibleRect::leftBottom());
		loginImage->setAnchorPoint(Vec2(0, 0));
		this->addChild(loginImage);

		//menu with button create
		auto btnAdd = MenuItemImage::create(
			"buttons/Add.png",
			"buttons/AddClicked.png",
			CC_CALLBACK_1(CharacterSelectionLayer::onCreate, this));
		btnAdd->setAnchorPoint(Vec2(1, 0));

		auto btnBack = MenuItemImage::create(
			"buttons/Back.png",
			"buttons/BackClicked.png",
			CC_CALLBACK_1(CharacterSelectionLayer::onBack, this));
		btnBack->setAnchorPoint(Vec2(1, 0));
	

		auto menu = Menu::create(btnAdd, btnBack, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 1);
		menu->setTag(kMenuCreate);

		//menu with buttons select delete
		auto btnLogin = MenuItemImage::create(
			"buttons/Login.png",
			"buttons/LoginClicked.png",
			CC_CALLBACK_1(CharacterSelectionLayer::onSelect, this));
		btnLogin->setAnchorPoint(Vec2(1, 0));
		

		auto btnDelete = MenuItemImage::create(
			"buttons/Delete.png",
			"buttons/DeleteClicked.png",
			CC_CALLBACK_1(CharacterSelectionLayer::onDelete, this));
		btnDelete->setAnchorPoint(Vec2(1, 0));
		

		auto btnBack2 = MenuItemImage::create(
			"buttons/Back.png",
			"buttons/BackClicked.png",
			CC_CALLBACK_1(CharacterSelectionLayer::onBack, this));
		btnBack2->setAnchorPoint(Vec2(1, 0));
		


#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
		btnAdd->setPosition(Point(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 150));
		btnBack->setPosition(Point(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 40));
		btnLogin->setPosition(Point(VisibleRect::rightBottom().x - 150, VisibleRect::rightBottom().y + 150));
		btnDelete->setPosition(Point(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 150));
		btnBack2->setPosition(Point(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 40));
#else
		btnAdd->setPosition(Point(VisibleRect::left().x + 195, VisibleRect::left().y + 30));
		btnBack->setPosition(Point(VisibleRect::left().x + 195, VisibleRect::left().y ));
		btnLogin->setPosition(Point(VisibleRect::left().x + 100, VisibleRect::left().y + 30));
		btnDelete->setPosition(Point(VisibleRect::left().x + 195, VisibleRect::left().y + 30));
		btnBack2->setPosition(Point(VisibleRect::left().x + 195, VisibleRect::left().y ));
#endif

		menu = Menu::create(btnLogin, btnDelete, btnBack2, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu, 1);
		menu->setTag(kMenuSelectDelete);
		menu->setVisible(false);

		
		/*SpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
		TextureCache::sharedTextureCache()->removeUnusedTextures();
		Director::sharedDirector()->purgeCachedData();*/
		//create sprite cache
		auto spriteFrameCache = SpriteFrameCache::getInstance();
		spriteFrameCache->addSpriteFramesWithFile("characters.plist", "characters.png");
		_spriteBatchNode = SpriteBatchNode::create("characters.png");
		this->addChild(_spriteBatchNode,1);

		
		
		//for (const characterInSlot & s : _slotData) {}
		//Add character creation layer
		_characterCreationLayer = CharacterCreationLayer::create();
		_characterCreationLayer->retain();
		this->addChild(_characterCreationLayer,1);
		_characterCreationLayer->setVisible(false);

		_dialog = Dialog::create();
		this->addChild(_dialog);

		//set slots and default char+label templates
		Sprite *characterSelectFrame;
		Sprite *characterSprite;
		LabelTTF *characterName;
		for (int i = 0; i < 3; i++){
			characterSelectFrame = Sprite::create("hud/PlayerSelectionContainer.png");
			characterSelectFrame->setPosition(Point(VisibleRect::rightTop().x - 50, VisibleRect::rightTop().y - 50 - i*characterSelectFrame->getContentSize().height));
			characterSelectFrame->setAnchorPoint(Vec2(1, 1));
			characterSelectFrame->setTag(i);
			this->addChild(characterSelectFrame);

			characterSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_1_m_d"));
			characterSprite->setAnchorPoint(Point(0.5, 0));
			characterSprite->setPosition(Vec2(VisibleRect::rightTop().x - characterSelectFrame->getContentSize().width + 25, VisibleRect::rightTop().y - 140 - i * characterSelectFrame->getContentSize().height));
			characterSprite->setName("CharacterSprite" + to_string(i));
			_spriteBatchNode->addChild(characterSprite);
			characterSprite->setVisible(false);

			characterName = LabelTTF::create("", "Arial", 12);
			characterName->setAnchorPoint(Point(0.5, 0));
			characterName->setPosition(Vec2(VisibleRect::rightTop().x - characterSelectFrame->getContentSize().width + 155, VisibleRect::rightTop().y - 94 - i * characterSelectFrame->getContentSize().height));
			characterName->setName("CharacterName" + to_string(i));
			this->addChild(characterName);
			characterName->setVisible(false);
		}
		//at start scene first slot is selected
		_slotSelected = kCharacterSlot1;

		//setSlotData();

		bRet = true;
	} while (0);

	return bRet;
}
//only callable from packetHandler when pressed delete button
void CharacterSelectionLayer::deleteCharacter(){
	_slotData[_slotSelected].isEmpty = true;
	_slotData[_slotSelected].charID = -1;
	_slotData[_slotSelected].name = "";
	_slotData[_slotSelected].character = 0;
	_slotData[_slotSelected].gender = 0;
	_slotData[_slotSelected].hairModal = 0;
	_slotData[_slotSelected].hairColor = 0;

	refreshSlotData();

	_dialog->hide();

}
//only callable from packetHandler when pressed create button
void CharacterSelectionLayer::addCharacter(int charID){
	_slotData[_slotSelected].isEmpty = false;
	_slotData[_slotSelected].charID = charID;
	_slotData[_slotSelected].name = _characterCreationLayer->getCharacterName();
	_slotData[_slotSelected].character = _characterCreationLayer->getSkinColor();
	_slotData[_slotSelected].gender = _characterCreationLayer->getGender();
	_slotData[_slotSelected].hairModal = _characterCreationLayer->getHairModal();
	_slotData[_slotSelected].hairColor = _characterCreationLayer->getHairColor();

	
	this->getEventDispatcher()->pauseEventListenersForTarget(_characterCreationLayer, true);
	this->getEventDispatcher()->resumeEventListenersForTarget(this, true);
	
	for (const auto& child : this->getChildren())
	{
		child->setOpacity(GLbyte(255));
	}
	
	refreshSlotData();
	_characterCreationLayer->hideDialog();
	_characterCreationLayer->setVisible(false);
}

void CharacterSelectionLayer::refreshSlotData(){

	Sprite *characterSprite;
	LabelTTF *characterName;
	for (int i = 0; i < 3; i++){

		characterSprite = (Sprite*)_spriteBatchNode->getChildByName("CharacterSprite" + to_string(i));
		//change sprite
		char gender;
		(_slotData[i].gender == kGenderFemale) ? gender = 'f' : gender = 'm';
		characterSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(_slotData[i].character + 1) + "_" + gender + "_d"));
		(_slotData[i].isEmpty) ? characterSprite->setVisible(false) : characterSprite->setVisible(true);

		characterName = (LabelTTF*)this->getChildByName("CharacterName" + to_string(i));
		characterName->setString(_slotData[i].name);
		(_slotData[i].isEmpty) ? characterName->setVisible(false) : characterName->setVisible(true);

	}
	selectSlot();
}

void CharacterSelectionLayer::setSlotData(CharacterInSlot slotData[3]){

	for (int i = 0; i < 3; i++)
	{
		_slotData[i].isEmpty = slotData[i].isEmpty;
		_slotData[i].charID = slotData[i].charID;
		_slotData[i].name = slotData[i].name;
		_slotData[i].character = slotData[i].character;
		_slotData[i].gender = slotData[i].gender;
		_slotData[i].hairModal = slotData[i].hairModal;
		_slotData[i].hairColor = slotData[i].hairColor;
		
	}

	/*Sprite *characterSelectFrame;
	Sprite *playerSprite;
	LabelTTF *playerName;
	for (int i = 0; i < 3; i++){
		characterSelectFrame = Sprite::create("hud/PlayerSelectionContainer.png");
		characterSelectFrame->setPosition(Point(VisibleRect::rightTop().x - 50, VisibleRect::rightTop().y - 50 - i*characterSelectFrame->getContentSize().height));
		characterSelectFrame->setAnchorPoint(Vec2(1, 1));
		characterSelectFrame->setTag(i);
		this->addChild(characterSelectFrame);

		if (!_slotData[i].isEmpty){
			playerSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(_slotData[i].character + 1) + "_m_d"));
			playerSprite->setAnchorPoint(Point(0.5, 0));
			playerSprite->setPosition(Vec2(VisibleRect::rightTop().x - characterSelectFrame->getContentSize().width + 25, VisibleRect::rightTop().y - 140 - i * characterSelectFrame->getContentSize().height));
			_spriteBatchNode->addChild(playerSprite);

			playerName = LabelTTF::create(_slotData[i].name, "fonts/coure.ttf", 12);
			playerName->setAnchorPoint(Point(0.5, 0));
			playerName->setPosition(Vec2(VisibleRect::rightTop().x - characterSelectFrame->getContentSize().width + 155, VisibleRect::rightTop().y - 94 - i * characterSelectFrame->getContentSize().height));
			this->addChild(playerName);
		}
	}*/
	refreshSlotData();
	
}

void CharacterSelectionLayer::onCreate(Ref* pSender)
{
	this->getEventDispatcher()->pauseEventListenersForTarget(this, true);
	//this->getEventDispatcher()->pauseEventListenersForTarget(this->getChildByTag(kMenuCreate));

	for (const auto& child : this->getChildren())
	{
		child->setOpacity(GLbyte(128));
	}

	_characterCreationLayer->setSlot(getSlotSelected());
	_characterCreationLayer->setVisible(true);
	this->getEventDispatcher()->resumeEventListenersForTarget(_characterCreationLayer,true); 
	//this->getEventDispatcher()->resumeEventListenersForTarget(_characterCreationLayer->getChildByTag(kMenuCreateCharacter));
}

void CharacterSelectionLayer::onSelect(Ref* pSender)
{
	CCLOG("on Select");
	if (connected){

		_dialog->showWait();

		clearbuffer();
		writebyte(MSG_ENTERGAME);
		writeint(_slotSelected);
		writeint(_slotData[_slotSelected].charID);
		sendmessage(serverSocket);
		CCLOG("After send MSG_ENTERGAME");
	}
}

void CharacterSelectionLayer::onDelete(Ref* pSender)
{
	_dialog->showOrdinary("Do you really want to delete \n this character?", true, CC_CALLBACK_1(CharacterSelectionLayer::_deleteCharacter, this));
	//_dialog->showImportant("Do you really want to delete \n this character?", true, CC_CALLBACK_1(CharacterSelectionLayer::testCallback, this));
}

void CharacterSelectionLayer::onBack(Ref* pSender)
{
	_dialog->showImportant("Do you really want to \n return to login screen?", true, CC_CALLBACK_1(CharacterSelectionLayer::_returnToLogin, this));
}

void CharacterSelectionLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);

	Sprite *sprite;

	for (int i = 0; i < 3; i++){

		sprite = (Sprite *)this->getChildByTag(i);
		if (sprite) {
			Rect rect = sprite->getBoundingBox();

			if (rect.containsPoint(touchLocation)) {
				_slotSelected = i;
				selectSlot();
			}

		}

	}

}

void CharacterSelectionLayer::selectSlot(){

	Sprite *sprite;
	for (int i = 0; i < 3; i++){

		sprite = (Sprite *)this->getChildByTag(i);
		if (sprite) {
			if (i != _slotSelected){
				sprite->setOpacity(GLbyte(128));
				
			}
			else{
				sprite->setOpacity(GLbyte(255));
				if (_slotData[i].isEmpty){
					this->getChildByTag(kMenuCreate)->setVisible(true);
					this->getChildByTag(kMenuSelectDelete)->setVisible(false);
				}
				else{
					this->getChildByTag(kMenuCreate)->setVisible(false);
					this->getChildByTag(kMenuSelectDelete)->setVisible(true);
				}
			}
		}

	}
}

void CharacterSelectionLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event){

	switch (keycode){
	case EventKeyboard::KeyCode::KEY_BACK:
		//show message "do you really want to return to login screen" if yes, ask server if he can return
		//_dialog->show(kDialogImportant, "Do you really want to \n return to login screen?", menu_selector(CharacterSelectionLayer::returnToLogin), true); //Important with 2 bnts
		//Director::getInstance()->end(); //for test
		_dialog->showImportant("Do you really want to \n return to login screen?", true, CC_CALLBACK_1(CharacterSelectionLayer::_returnToLogin, this));
		//gotoLogin();
		break;
	case EventKeyboard::KeyCode::KEY_MENU:
		MessageBox("message goes here", "MSG TITLE");
		break;
	}
}

void CharacterSelectionLayer::showDialogCallbackLogin(string text){
	_dialog->showImportant(text, false, CC_CALLBACK_1(CharacterSelectionLayer::_returnToLogin, this));
}

void CharacterSelectionLayer::showDialog(string str){
	_dialog->showOrdinary(str);
}

void CharacterSelectionLayer::_returnToLogin(Ref* pSender){

	if (connected){
		clearbuffer();
		writebyte(MSG_RETURNTOLOGIN);
		//writebyte(kSceneLogin);
		sendmessage(serverSocket);
	}
	gotoLogin();
}

void CharacterSelectionLayer::_deleteCharacter(Ref* pSender){

	if (connected){

		_dialog->showWait();

		clearbuffer();
		writebyte(MSG_DELETECHARACTER);
		writeint(_slotSelected);
		writeint(_slotData[_slotSelected].charID);
		sendmessage(serverSocket);
	}
}