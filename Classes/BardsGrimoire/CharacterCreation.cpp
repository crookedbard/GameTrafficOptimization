#include "CharacterCreation.h"



Scene* CharacterCreation::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = CharacterCreation::create();

    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CharacterCreation::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		visibleSize = Director::getInstance()->getVisibleSize();
		origin = Director::getInstance()->getVisibleOrigin();

		// Enable touch/click actions
		this->setTouchEnabled(true);

		// Enable keypad/keyboard actions
		this->setKeypadEnabled(true);

		//Scene label
		auto sceneLabel = LabelTTF::create("Character Creation", "Arial", 24);
		sceneLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - sceneLabel->getContentSize().height));
		this->addChild(sceneLabel);

		//Name label
		auto nameLabel = LabelTTF::create("Name: ", "Arial", 24);
		nameLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - nameLabel->getContentSize().width / 2 - 100,
			origin.y + visibleSize.height / 2 + nameLabel->getContentSize().height + 100));
		this->addChild(nameLabel);

		//Character label
		auto characterLabel = LabelTTF::create("Character: ", "Arial", 24);
		characterLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - characterLabel->getContentSize().width / 2 - 100,
			origin.y + visibleSize.height / 2 ));
		this->addChild(characterLabel);

		m_pEditName = EditBox::create(Size(100, 24), Scale9Sprite::create("EditBoxTemplate2.png"));
		m_pEditName->setAnchorPoint(Point(0, 0));
		m_pEditName->setPosition(Point(origin.x + visibleSize.width / 2 - m_pEditName->getContentSize().width / 2,
			origin.y + visibleSize.height / 2 + m_pEditName->getContentSize().height + 95));
		m_pEditName->setFontColor(ccWHITE);
		//m_pEditName->setPlaceHolder("enter name");
		m_pEditName->setText("");
		m_pEditName->setMaxLength(12);
		m_pEditName->setReturnType(EditBox::KeyboardReturnType::DONE);
		m_pEditName->setDelegate(this);
		//m_pEditName->setFontSize(23);
		this->addChild(m_pEditName);

		//sprite character box
		auto sprite = Sprite::create("CharacterBox.png");
		sprite->setPosition(Vec2(origin.x + visibleSize.width / 2 , 
			origin.y + visibleSize.height / 2));
		this->addChild(sprite, 0); 

		//player sprite
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("characters.plist", "characters.png");

		auto spriteBatchNode = SpriteBatchNode::create("characters.png");
		addChild(spriteBatchNode);

		//Vector<SpriteFrame*> animFrames(3);
		_character = 1;
		auto frame1 = cache->getSpriteFrameByName("char_1_m_d");
		auto frame2 = cache->getSpriteFrameByName("char_2_m_d");
		auto frame3 = cache->getSpriteFrameByName("char_3_m_d");
		auto frame4 = cache->getSpriteFrameByName("char_4_m_d");
		auto frame5 = cache->getSpriteFrameByName("char_5_m_d");
		auto frame6 = cache->getSpriteFrameByName("char_6_m_d");
		auto frame7 = cache->getSpriteFrameByName("char_7_m_d");
		_animFrames.pushBack(frame1);
		_animFrames.pushBack(frame2);
		_animFrames.pushBack(frame3);
		_animFrames.pushBack(frame4);
		_animFrames.pushBack(frame5);
		_animFrames.pushBack(frame6);
		_animFrames.pushBack(frame7);

		_sprPlr = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_1_m_d"));
		_sprPlr->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height / 2));
		spriteBatchNode->addChild(_sprPlr);
		//addChild(spriteBatchNode);

		//button left arrow - menu item
		auto sprLeftArrow = Sprite::create("btnLeftArrow.png");
		auto opacity = GLubyte(200); //255max opacity
		sprLeftArrow->setOpacity(opacity);

		auto btnLeftArrowItem = MenuItemImage::create(
			"btnLeftArrow.png",
			"btnLeftArrow.png",
			CC_CALLBACK_1(CharacterCreation::leftArrowReleased, this));
		btnLeftArrowItem->setSelectedImage(sprLeftArrow);
		btnLeftArrowItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + btnLeftArrowItem->getContentSize().width / 2 - 90,
			origin.y + visibleSize.height / 2 ));

		//button right arrow - menu item
		auto sprRightArrow = Sprite::create("btnRightArrow.png");
		//auto opacity = GLubyte(200); //255max opacity
		sprRightArrow->setOpacity(opacity);

		auto btnRightArrowItem = MenuItemImage::create(
			"btnRightArrow.png",
			"btnRightArrow.png",
			CC_CALLBACK_1(CharacterCreation::rightArrowReleased, this));
		btnRightArrowItem->setSelectedImage(sprRightArrow);
		btnRightArrowItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - btnRightArrowItem->getContentSize().width / 2 + 180 - 90,
			origin.y + visibleSize.height / 2 ));

		//label create - menu item
		auto createLabelItem = MenuItemFont::create("Create", this, menu_selector(CharacterCreation::onCreate));
		createLabelItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + createLabelItem->getContentSize().height));

		//menu
		auto menu = Menu::create(createLabelItem, btnLeftArrowItem, btnRightArrowItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu);

		bRet = true;
	} while (0);

	return bRet;
}

void CharacterCreation::leftArrowReleased(Ref* pSender){
	if (_character == 1){ _character = 7; }
	else{
		_character--;
	}
	_sprPlr->setSpriteFrame(_animFrames.at(_character-1));
}

void CharacterCreation::rightArrowReleased(Ref* pSender){
	if (_character == 7){ _character = 1; }
	else{
		_character++;
	}
	_sprPlr->setSpriteFrame(_animFrames.at(_character-1));
}

void CharacterCreation::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event){

	switch(keycode){
		case EventKeyboard::KeyCode::KEY_BACK: 
			Director::getInstance()->replaceScene(MainMenu::createScene());
			break;
		case EventKeyboard::KeyCode::KEY_MENU : 

			MessageBox("message goes here","MSG TITLE");
			break;
		case EventKeyboard::KeyCode::KEY_SPACE : 

			MessageBox("message goes here","MSG TITLE");
			break;
	}
}

void CharacterCreation::onCreate(Ref* pSender)
{
	//CCLOG("vardas: %s",m_pEditName->getText());
	string str = m_pEditName->getText();

	if (!str.empty()){
		// Start the game 
		//CCLOG("testas");
		Director::getInstance()->replaceScene(HelloWorld::createScene(_character,str));
	}
	else{
		MessageBox("Please enter player name!", "Forgot to enter");
	}
}

void CharacterCreation::editBoxReturn(EditBox* editBox)
{

}

void CharacterCreation::menuCloseCallback(Ref* pSender)
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
