#include "CharacterCreationLayer.h"
#include "39dll.h"

USING_NS_CC;
using namespace std;

/*enum {
	kTextFieldPlayerName,
	kCursorPlayerName,
	kSpriteGender,
	kSpriteHairModal,
	kSpriteHairColor,
	kSpriteSkinColor,
	kSpritePlayer
};*/
/*Scene* CharacterCreationLayer::createScene(cocos2d::Layer *layer)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	//auto layer = CharacterSelectionLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}*/

bool CharacterCreationLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		// Enable touch/click actions
		//this->setTouchEnabled(true);

		// Enable keypad/keyboard actions
		//this->setKeypadEnabled(true);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true; };
		listener->onTouchEnded = CC_CALLBACK_2(CharacterCreationLayer::onTouchEnded, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		auto playerCreationWindow = Sprite::create("hud/PlayerCreationWindow.png");
		//charCreationImage->setAnchorPoint(Vec2(0.5, 0.5));
		playerCreationWindow->setPosition(VisibleRect::center());
		this->addChild(playerCreationWindow);
		
		auto spriteFrameCache = SpriteFrameCache::getInstance();
		spriteFrameCache->addSpriteFramesWithFile("buttons/SmallButtons.plist", "buttons/SmallButtons.png");
		_spriteBatchNode = SpriteBatchNode::create("buttons/SmallButtons.png");
		this->addChild(_spriteBatchNode);

		auto btnToggle = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggle) + ".png"));
		auto btnToggleClicked = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggleClicked) + ".png"));
		
		auto btnToggleGender = MenuItemSprite::create(
			btnToggle,
			btnToggleClicked,
			CC_CALLBACK_1(CharacterCreationLayer::toggleGender, this));
		btnToggleGender->setAnchorPoint(Vec2(0, 0));
		btnToggleGender->setPosition(Point(VisibleRect::center().x + 14, VisibleRect::center().y + 16));

		auto btnToggle2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggle) + ".png"));
		auto btnToggleClicked2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggleClicked) + ".png"));
		
		auto btnToggleHairModal = MenuItemSprite::create(
			btnToggle2,
			btnToggleClicked2,
			CC_CALLBACK_1(CharacterCreationLayer::toggleHairModal, this));
		btnToggleHairModal->setAnchorPoint(Vec2(0, 0));
		btnToggleHairModal->setPosition(Point(VisibleRect::center().x + 14, VisibleRect::center().y - 11));
		
		auto btnToggle3 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggle) + ".png"));
		auto btnToggleClicked3 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggleClicked) + ".png"));

		auto btnToggleHairColor = MenuItemSprite::create(
			btnToggle3,
			btnToggleClicked3,
			CC_CALLBACK_1(CharacterCreationLayer::toggleHairColor, this));
		btnToggleHairColor->setAnchorPoint(Vec2(0, 0));
		btnToggleHairColor->setPosition(Point(VisibleRect::center().x + 14, VisibleRect::center().y - 38));

		auto btnToggle4 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggle) + ".png"));
		auto btnToggleClicked4 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnToggleClicked) + ".png"));

		auto btnToggleSkinColor = MenuItemSprite::create(
			btnToggle4,
			btnToggleClicked4,
			CC_CALLBACK_1(CharacterCreationLayer::toggleSkinColor, this));
		btnToggleSkinColor->setAnchorPoint(Vec2(0, 0));
		btnToggleSkinColor->setPosition(Point(VisibleRect::center().x + 14, VisibleRect::center().y - 65));

		auto btnOk = MenuItemImage::create(
			"buttons/Ok.png",
			"buttons/OkClicked.png",
			CC_CALLBACK_1(CharacterCreationLayer::onCreate, this));
		btnOk->setAnchorPoint(Vec2(0, 0));
		btnOk->setPosition(Point(VisibleRect::center().x - 23, VisibleRect::center().y - 104));

		auto btnCancel = MenuItemImage::create(
			"buttons/Cancel.png",
			"buttons/CancelClicked.png",
			CC_CALLBACK_1(CharacterCreationLayer::onCancel, this));
		btnCancel->setAnchorPoint(Vec2(0, 0));
		btnCancel->setPosition(Point(VisibleRect::center().x + 70, VisibleRect::center().y - 104));

		auto menu = Menu::create(btnToggleGender, btnToggleHairModal,btnToggleHairColor
			, btnToggleSkinColor, btnOk, btnCancel, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu);
		menu->setTag(kMenuCreateCharacter);

		//textField new player name
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("","Arial", 12);
		addChild(textField);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x - 97, VisibleRect::center().y + 47));
		textField->setDelegate(this);
		textField->setTag(kTextFieldPlayerName);

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		auto cursorLabel = LabelTTF::create("|", "Arial", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorPlayerName);
		cursorLabel->runAction(blink);

		//set 5 sprites
		auto sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnFemale) + ".png"));
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Point(VisibleRect::center().x - 10, VisibleRect::center().y + 17));
		this->addChild(sprite);
		sprite->setTag(kSpriteGender);

		sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnHair1) + ".png"));
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Point(VisibleRect::center().x - 10, VisibleRect::center().y - 10));
		this->addChild(sprite);
		sprite->setTag(kSpriteHairModal);

		sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnBrown) + ".png"));
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Point(VisibleRect::center().x - 10, VisibleRect::center().y - 37));
		this->addChild(sprite);
		sprite->setTag(kSpriteHairColor);

		sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(kBtnSkinWhite) + ".png"));
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Point(VisibleRect::center().x - 10, VisibleRect::center().y - 64));
		this->addChild(sprite);
		sprite->setTag(kSpriteSkinColor);

		sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_1_f_d"));
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Point(VisibleRect::center().x + 94, VisibleRect::center().y - 35));
		this->addChild(sprite);
		sprite->setTag(kSpritePlayer);

		_dialog = Dialog::create();
		this->addChild(_dialog);

		//init private members
		_slot = 0;
		_characterName = "";
		_gender = kGenderFemale;
		_hairModal = kBtnHair1 - 20;
		_hairColor = kBtnBrown;
		_skinColor = kBtnSkinWhite - 42;

		bRet = true;
	} while (0);

	return bRet;
}

void CharacterCreationLayer::toggleGender(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	auto iconSprite = (Sprite *)this->getChildByTag(kSpriteGender);
	auto playerSprite = (Sprite *)this->getChildByTag(kSpritePlayer);
	if (!iconSprite || !playerSprite) return;

	int iconNo;
	char playerGender;
	(_gender == kGenderFemale) ? _gender = kGenderMale : _gender = kGenderFemale;
	(_gender == kGenderFemale) ? iconNo = kBtnFemale : iconNo = kBtnMale;
	(_gender == kGenderFemale) ? playerGender = 'f' : playerGender = 'm';
	iconSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(iconNo) + ".png"));
	playerSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(_skinColor + 1) + "_" + playerGender + "_d"));
}

void CharacterCreationLayer::toggleHairModal(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	auto iconSprite = (Sprite *)this->getChildByTag(kSpriteHairModal);
	//auto playerSprite = (Sprite *)this->getChildByTag(kSpritePlayer);
	if (!iconSprite /*|| !playerSprite*/) return;

	(_hairModal == kBtnHair20 - 20) ? _hairModal = kBtnHair1 - 20 : _hairModal++;

	iconSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(_hairModal+20) + ".png"));
	//playerSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(_skinColor + 1) + "_" + playerGender + "_d"));
}

void CharacterCreationLayer::toggleHairColor(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	auto iconSprite = (Sprite *)this->getChildByTag(kSpriteHairColor);
	//auto playerSprite = (Sprite *)this->getChildByTag(kSpritePlayer);
	if (!iconSprite /*|| !playerSprite*/) return;

	(_hairColor == kBtnBlack) ? _hairColor = kBtnBrown : _hairColor++;

	iconSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(_hairColor) + ".png"));
	//playerSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(_skinColor + 1) + "_" + playerGender + "_d"));
}

void CharacterCreationLayer::toggleSkinColor(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	auto iconSprite = (Sprite *)this->getChildByTag(kSpriteSkinColor);
	auto playerSprite = (Sprite *)this->getChildByTag(kSpritePlayer);
	if (!iconSprite || !playerSprite) return;

	(_skinColor == kBtnSkinPanda - 42) ? _skinColor = kBtnSkinWhite - 42 : _skinColor++;

	char playerGender;
	(_gender == kGenderFemale) ? playerGender = 'f' : playerGender = 'm';

	iconSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("btn_" + to_string(_skinColor + 42) + ".png"));
	playerSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("char_" + to_string(_skinColor + 1) + "_" + playerGender + "_d"));
}

void CharacterCreationLayer::onCreate(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	if (connected){
		clearbuffer();
		writebyte(MSG_CREATECHARACTER);
		writeint(getSlot());
		writestring(getCharacterName());
		writeint(getSkinColor());
		writeint(getGender());
		writeint(getHairModal());
		writeint(getHairColor());
		sendmessage(serverSocket);
		_dialog->showWait();
	}
}

void CharacterCreationLayer::onCancel(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	
	this->getEventDispatcher()->pauseEventListenersForTarget(this,true); //->setEnabled(false);
	//this->getEventDispatcher()->pauseEventListenersForTarget(this->getChildByTag(kMenuCreateCharacter));

	for (const auto& child : this->getScene()->getChildByName("CharacterSelectionLayer")->getChildren())
	{
		child->setOpacity(GLbyte(255));
	}
	//this->getScene()->getChildByName("CharacterSelectionLayer")->selectSlot();
	//this->getScene()->getChildByName("CharacterSelectionLayer")->setTouchEnabled(false);
	//this->setKeypadEnabled(false);
	this->getEventDispatcher()->resumeEventListenersForTarget(this->getScene()->getChildByName("CharacterSelectionLayer"),true); //->setEnabled(true);
	//this->getEventDispatcher()->resumeEventListenersForTarget(this->getScene()->getChildByName("CharacterSelectionLayer")->getChildByTag(kMenuCreate));
	this->setVisible(false);
}

bool CharacterCreationLayer::onVisit(TextFieldTTF * sender, Renderer *renderer, const Mat4 &transform, uint32_t flags)
{

	//TextFieldTTF *textField = (TextFieldTTF *)this->getChildByTag(kTextFieldPlayerName);
	LabelTTF *cursor = (LabelTTF *)this->getChildByTag(kCursorPlayerName);
	if (/*textField && */cursor) {
		cursor->setPosition(Point(sender->getPosition().x + sender->getContentSize().width, sender->getPosition().y));
	}

	CC_UNUSED_PARAM(renderer);
	CC_UNUSED_PARAM(transform);
	CC_UNUSED_PARAM(flags);
	return false;
}

bool CharacterCreationLayer::onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen)
{
	if (sender->getString().length() + nLen < MAX_LETTERS){
		_characterName.append(text);
		return false;//append
	}
	return true;
}

bool CharacterCreationLayer::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen)
{
	_characterName.erase(_characterName.end() - nLen, _characterName.end());

	CC_UNUSED_PARAM(sender);
	CC_UNUSED_PARAM(delText);
	//CC_UNUSED_PARAM(nLen);
	return false;
}

void CharacterCreationLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);

	TextFieldTTF *textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldPlayerName);
	LabelTTF *cursor = (LabelTTF *)this->getChildByTag(kCursorPlayerName);
	if (textfield && cursor) {
		//Rect rect = textfield->getBoundingBox();
		Rect rect = Rect::Rect(textfield->getPositionX(), textfield->getPositionY(), 130, 14);
		if (rect.containsPoint(touchLocation)) {
			textfield->attachWithIME();
			cursor->setVisible(true);
		}
		else{
			textfield->detachWithIME();
			cursor->setVisible(false);
		}
	}

	CC_UNUSED_PARAM(unused_event);
}
/*void CharacterCreationLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event){

	switch (keycode){
	case EventKeyboard::KeyCode::KEY_BACK:
		break;
	case EventKeyboard::KeyCode::KEY_MENU:
		MessageBox("message goes here", "MSG TITLE");
		break;
	}
}*/

void CharacterCreationLayer::hideDialog(){
	_dialog->hide();
}

void CharacterCreationLayer::showDialog(string str){
	_dialog->showOrdinary(str);
}

/*void CharacterCreationLayer::_createCharacter(Ref* pSender){

	if (connected){
		clearbuffer();
		writebyte(MSG_CREATECHARACTER);
		writebyte(getSlot());
		writestring(getCharacterName());
		writeint(getSkinColor());
		writeint(getGender());
		writeint(getHairModal());
		writeint(getHairColor());
		sendmessage(serverSocket);
		_dialog->showWait();
	}
}*/