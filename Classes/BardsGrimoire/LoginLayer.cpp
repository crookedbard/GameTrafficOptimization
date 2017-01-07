#include "LoginLayer.h"
#include "39dll.h"

//#define MAX_LETTERS 12

USING_NS_CC;
using namespace std;

Scene* LoginLayer::createScene(cocos2d::Layer *layer)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	//auto layer
	//layer = LoginLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginLayer::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Layer::init());

		// Enable touch/click actions
		this->setTouchEnabled(true);

		// Enable keypad/keyboard actions
		this->setKeypadEnabled(true);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true; };
		listener->onTouchEnded = CC_CALLBACK_2(LoginLayer::onTouchEnded, this);
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		//Add Login background image
		int r = 1 + (rand() % 4);
		auto backgroundImage = Sprite::create("hud/LoginBackground"+to_string(r)+".png");
		backgroundImage->setPosition(Point(0, 0));
		backgroundImage->setAnchorPoint(Vec2(0, 0));
		backgroundImage->setScale(visibleSize.width / backgroundImage->getContentSize().width,
			visibleSize.height / backgroundImage->getContentSize().height);
		this->addChild(backgroundImage);

		//Add Login picture
		r = 1 + (rand() % 4);
		auto loginImage = Sprite::create("hud/LoginPicture" + to_string(r) + ".png");
		loginImage->setPosition(VisibleRect::rightBottom());
		loginImage->setAnchorPoint(Vec2(1, 0));
		this->addChild(loginImage);

		_dialog = Dialog::create();
		this->addChild(_dialog,1);

		//init private members
		_password = "";
		_newPassword = "";
		_retypePassword = "";
		_window = kLayerNone;

		bringWindow(kLayerMain);
		
		bRet = true;
	} while (0);

	return bRet;
}

void LoginLayer::onCreateAccount(Ref* pSender)
{
	bringWindow(kLayerCreateAccount);
}

void LoginLayer::onPlayGame(Ref* pSender)
{
	bringWindow(kLayerLogin);
}

void LoginLayer::onExitGame(Ref* pSender)
{
	// Exit game
	Director::getInstance()->end();
}

void LoginLayer::onEnter(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	//bringWindow(kLayerMessage, "Please wait...");
	_dialog->showWait();
	hideWindow();

	if (!connected) {
		tryConnecting(kLayerLogin);
	}
	else{
		sendLoginData();
	}

}

void LoginLayer::onOk(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);

	//bringWindow(kLayerMessage, "Please wait...");
	_dialog->showWait();
	hideWindow();

	if (!connected) {
		tryConnecting(kLayerCreateAccount);
	}
	else{
		sendRegistrationData();
	}

}

void LoginLayer::sendLoginData(){

	auto textFieldUsername = (TextFieldTTF *)this->getChildByTag(kTextFieldAccount);

	if (textFieldUsername) {
		clearbuffer();
		writebyte(MSG_LOGIN); 
		writestring(textFieldUsername->getString());
		writestring(md5string(_password));
		sendmessage(serverSocket);
	}
}

void LoginLayer::showDialog(string str){
	//_dialog->show(kDialogOrdinary, str, menu_selector(LoginLayer::hideDialog));
	hideWindow();
	_dialog->showOrdinary(str, false, CC_CALLBACK_1(LoginLayer::hideDialog,this));
}



void LoginLayer::hideDialog(Ref *sender){
	_dialog->hide();
	showWindow();
}

void LoginLayer::showDialogCallbackWindow(string str, int window){
	//_dialog->show(kDialogOrdinary, str, menu_selector(LoginLayer::hideDialog));
	hideWindow();
	_dialog->showOrdinary(str, false, CC_CALLBACK_1(LoginLayer::hideDialogBringWindow, this, window));
}

void LoginLayer::hideDialogBringWindow(Ref *sender, int window){
	_dialog->hide();
	//showWindow();
	bringWindow(window);
}

void LoginLayer::sendRegistrationData(){

	auto textFieldAccountName = (TextFieldTTF *)this->getChildByTag(kTextFieldAccountName);
	auto textFieldEmail = (TextFieldTTF *)this->getChildByTag(kTextFieldEmail);

	if (textFieldAccountName && textFieldEmail) {
		if (_newPassword != _retypePassword) {
			//bringWindow(kLayerMessage, "Passwords do not match!", kLayerCreateAccount);
			//_dialog->show(kDialogOrdinary, "Passwords do not match!", menu_selector(LoginLayer::hideDialog));
			_dialog->showOrdinary("Passwords do not match!");
			return;
		}
		else if (count(textFieldEmail->getString().begin(), textFieldEmail->getString().end(), '@') != 1) {
			//bringWindow(kLayerMessage, "Invalid email address!", kLayerCreateAccount);
			//_dialog->show(kDialogOrdinary, "Invalid email address!", menu_selector(LoginLayer::hideDialog));
			_dialog->showOrdinary("Invalid email address!");
			return;
		}
		clearbuffer();
		writebyte(MSG_REGISTER);
		writestring(textFieldAccountName->getString());
		writestring(textFieldEmail->getString());
		writestring(md5string(_newPassword));
		sendmessage(serverSocket);
	}
}

void LoginLayer::onCancel(Ref* pSender)
{
	onTouchEnded(((Touch *)pSender), NULL);
	bringWindow(kLayerMain);
}

void LoginLayer::hideWindow(){

	Touch *touch = new Touch();
	touch->setTouchInfo(0, 0.0f, 0.0f);
	onTouchEnded(touch, NULL);

	switch (_window){
	case kLayerMain:
		showMain(false);
		break;
	case kLayerCreateAccount:
		showCreateAccount(false);
		break;
	case kLayerLogin:
		showLogin(false);
		break;
	case kLayerNone: break;
	}
}

void LoginLayer::showWindow(){

	Touch *touch = new Touch();
	touch->setTouchInfo(0, 0.0f, 0.0f);
	onTouchEnded(touch, NULL);

	switch (_window){
	case kLayerMain:
		showMain(true);
		break;
	case kLayerCreateAccount:
		showCreateAccount(true);
		break;
	case kLayerLogin:
		showLogin(true);
		break;
	case kLayerNone: break;
	}
}

void LoginLayer::bringWindow(int window/*, string msg, byte toWindow*/){
	
	//this->ccTouchEnded(this->_touchListener(), NULL);
	//Hide current window

	switch (_window){
	case kLayerMain: 
		//_mainMenu->setVisible(false);
		showMain(false);
		break;
	case kLayerCreateAccount:
		//_createAccountWindow->setVisible(false);
		//_createAccountMenu->setVisible(false);
		showCreateAccount(false);
		break;
	case kLayerLogin:
		//_loginWindow->setVisible(false);
		//_loginMenu->setVisible(false);
		showLogin(false);
		break;
	/*case kLayerMessage: 
		showMessage(false);
		break;*/
	case kLayerNone: break;
	}

	//Unhide needed window
	switch (window){
	case kLayerMain: 
		//_mainMenu->setVisible(true);
		showMain(true);
		//_previousWindow = _window;
		_window = kLayerMain; 
		break;
	case kLayerCreateAccount:
	{
		//_createAccountWindow->setVisible(true);
		//_createAccountMenu->setVisible(true);
		
		showCreateAccount(true);
		//_previousWindow = _window;
		_window = kLayerCreateAccount;
	}
		break;
	case kLayerLogin:
		//_loginWindow->setVisible(true);
		//_loginMenu->setVisible(true);
		showLogin(true);
		//_previousWindow = _window;
		_window = kLayerLogin;
		break;
	/*case kLayerMessage:
		//CCLOG("%s",msg);
		showMessage(true,msg,toWindow);
		_window = kLayerMessage;*/
		break;
	case kLayerNone: break;
	}

}

void LoginLayer::showCreateAccount(bool show){
	Node *node ;
	
	node = this->getChildByTag(kSpriteCreateAccount);
	if (!node){
		auto sprite = Sprite::create("hud/CreateAccountWindow.png");
		sprite->setPosition(VisibleRect::center());
		this->addChild(sprite);
		sprite->setTag(kSpriteCreateAccount);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kMenuCreateAccount);
	if (!node){
		auto btnOk = MenuItemImage::create(
			"buttons/Ok.png",
			"buttons/OkClicked.png",
			CC_CALLBACK_1(LoginLayer::onOk, this));
		btnOk->setAnchorPoint(Vec2(0, 0));
		btnOk->setPosition(Point(VisibleRect::center().x - 23, VisibleRect::center().y - 104));

		auto btnCancel = MenuItemImage::create(
			"buttons/Cancel.png",
			"buttons/CancelClicked.png",
			CC_CALLBACK_1(LoginLayer::onCancel, this));
		btnCancel->setAnchorPoint(Vec2(0, 0));
		btnCancel->setPosition(Point(VisibleRect::center().x + 70, VisibleRect::center().y - 104));

		auto menu = Menu::create(btnOk, btnCancel, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu);
		menu->setTag(kMenuCreateAccount);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kTextFieldAccountName);
	if (!node){
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("",/* Size(130, 14), TextHAlignment::CENTER,*/
			"Arial", 12);
		addChild(textField);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x + 22, VisibleRect::center().y + 43));
		textField->setDelegate(this);
		textField->setTag(kTextFieldAccountName);

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		//m_pTextFieldAction->retain();
		auto cursorLabel = /*Label::createWithTTF("|", "fonts/micross.ttf", 12);*/LabelTTF::create("|", "fonts/micross.ttf", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorAccountName);
		cursorLabel->runAction(blink);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kTextFieldEmail);
	if (!node){
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("",/* Size(130, 14), TextHAlignment::CENTER,*/
			"Arial", 12);
		addChild(textField);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x + 22, VisibleRect::center().y + 13));
		textField->setDelegate(this);
		textField->setTag(kTextFieldEmail);

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		//m_pTextFieldAction->retain();
		auto cursorLabel = /*Label::createWithTTF("|", "fonts/micross.ttf", 12); */LabelTTF::create("|", "fonts/micross.ttf", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorEmail);
		cursorLabel->runAction(blink);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kTextFieldNewPassword);
	if (!node){
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("",/* Size(130, 14), TextHAlignment::CENTER,*/
			"Arial", 12);
		textField->setTag(kTextFieldNewPassword);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x + 22, VisibleRect::center().y - 17));
		textField->setDelegate(this);
		addChild(textField);

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		//m_pTextFieldAction->retain();
		auto cursorLabel = /*Label::createWithTTF("|", "fonts/micross.ttf", 12); */LabelTTF::create("|", "fonts/micross.ttf", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorNewPassword);
		cursorLabel->runAction(blink);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kTextFieldRetypePassword);
	if (!node){
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("",/* Size(130, 14), TextHAlignment::CENTER,*/
			"Arial", 12);
		textField->setTag(kTextFieldRetypePassword);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x + 22, VisibleRect::center().y - 47));
		textField->setDelegate(this);
		addChild(textField);

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		//m_pTextFieldAction->retain();
		auto cursorLabel = /*Label::createWithTTF("|", "fonts/micross.ttf", 12); */LabelTTF::create("|", "fonts/micross.ttf", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorRetypePassword);
		cursorLabel->runAction(blink);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}
}

void LoginLayer::showLogin(bool show){
	Node *node;

	node = this->getChildByTag(kSpriteLogin);
	if (!node){
		auto sprite = Sprite::create("hud/LoginWindow.png");
		sprite->setPosition(VisibleRect::center());
		this->addChild(sprite);
		sprite->setTag(kSpriteLogin);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kMenuLogin);
	if (!node){
		auto btnEnterMini = MenuItemImage::create(
			"buttons/EnterMini.png",
			"buttons/EnterMiniClicked.png",
			CC_CALLBACK_1(LoginLayer::onEnter, this));
		btnEnterMini->setAnchorPoint(Vec2(0, 0));
		btnEnterMini->setPosition(Point(VisibleRect::center().x + 18, VisibleRect::center().y - 60));

		auto btnCancelMini = MenuItemImage::create(
			"buttons/CancelMini.png",
			"buttons/CancelMiniClicked.png",
			CC_CALLBACK_1(LoginLayer::onCancel, this));
		btnCancelMini->setAnchorPoint(Vec2(0, 0));
		btnCancelMini->setPosition(Point(VisibleRect::center().x + 78, VisibleRect::center().y - 60));

		auto menu = Menu::create(btnEnterMini, btnCancelMini, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu);
		menu->setTag(kMenuLogin);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	node = this->getChildByTag(kTextFieldAccount);
	if (!node){
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("",	"Arial", 12);
		this->addChild(textField);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x - 114, VisibleRect::center().y - 10));
		textField->setDelegate(this);
		textField->setTag(kTextFieldAccount);
		textField->setString("test"); //remove later in release build

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		auto cursorLabel = /*Label::createWithTTF("|", "fonts/micross.ttf", 12); */LabelTTF::create("|", "fonts/micross.ttf", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorAccount);
		cursorLabel->runAction(blink);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}
	
	node = this->getChildByTag(kTextFieldPassword);
	if (!node){
		auto textField = TextFieldTTF::textFieldWithPlaceHolder("", "Arial", 12);

		addChild(textField);
		textField->setAnchorPoint(Vec2::ZERO);
		textField->setPosition(Point(VisibleRect::center().x - 114, VisibleRect::center().y - 54));
		textField->setDelegate(this);
		textField->setTag(kTextFieldPassword);
		textField->setString("****"); //remove later in release build
		_password = "test";

		auto blink = RepeatForever::create((ActionInterval*)Sequence::create(FadeOut::create(0.25), FadeIn::create(0.25), 0));
		auto cursorLabel = /*Label::createWithTTF("|", "fonts/micross.ttf", 12); */LabelTTF::create("|", "fonts/micross.ttf", 12);
		cursorLabel->setVisible(false);
		cursorLabel->setAnchorPoint(Vec2::ZERO);
		cursorLabel->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		this->addChild(cursorLabel);
		cursorLabel->setTag(kCursorPassword);
		cursorLabel->runAction(blink);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

}

void LoginLayer::showMain(bool show){
	Node *node;

	node = this->getChildByTag(kMenuMain);
	if (!node){
		auto btnCreateAccount = MenuItemImage::create(
			"buttons/CreateAccount.png",
			"buttons/CreateAccountClicked.png",
			CC_CALLBACK_1(LoginLayer::onCreateAccount, this));

		auto btnPlayGame = MenuItemImage::create(
			"buttons/PlayGame.png",
			"buttons/PlayGameClicked.png",
			CC_CALLBACK_1(LoginLayer::onPlayGame, this));

		auto btnExitGame = MenuItemImage::create(
			"buttons/ExitGame.png",
			"buttons/ExitGameClicked.png",
			CC_CALLBACK_1(LoginLayer::onExitGame, this));

		auto menu = Menu::create(btnCreateAccount, btnPlayGame, btnExitGame, NULL);
		menu->alignItemsVertically();
		this->addChild(menu);
		menu->setTag(kMenuMain);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

}
/*
void LoginLayer::showMessage(bool show, string msg , byte toWindow){
	Node *node;

	node = this->getChildByTag(kSpriteMessage);
	if (!node){
		auto sprite = Sprite::create("hud/MessageWindow.png");
		sprite->setPosition(VisibleRect::center());
		this->addChild(sprite);
		sprite->setTag(kSpriteMessage);
	}
	else{
		(show) ? node->setVisible(true) : node->setVisible(false);
	}

	Label *labelNode = (Label *)this->getChildByTag(kLabelMessage);
	if (!labelNode && show && msg != ""){
		auto label = Label::create(msg, "Arial", 12);
		label->setPosition(VisibleRect::center());
		this->addChild(label);
		label->setTag(kLabelMessage);
		//(show) ? label->setVisible(true) : label->setVisible(false);
	}
	else if (labelNode && show && msg!=""){
		labelNode->setString(msg);
		labelNode->setVisible(true);
	}
	else if (labelNode && !show){
		labelNode->setVisible(false);
	}

	Menu *menuOld = (Menu *)this->getChildByTag(kMenuMessage);
	if (!menuOld && show && toWindow != kLayerNone){
		auto btnOk = MenuItemImage::create(
			"buttons/Ok.png",
			"buttons/OkClicked.png",
			CC_CALLBACK_0(LoginLayer::bringWindow, this, toWindow,"",kLayerNone));
		btnOk->setPosition(Point(VisibleRect::center().x, VisibleRect::center().y - 35));
		auto menu = Menu::create(btnOk, NULL);
		menu->setPosition(Point::ZERO);
		this->addChild(menu);
		menu->setTag(kMenuMessage);
		//(show) ? menu->setVisible(true) : menu->setVisible(false);
	}
	else if (menuOld && show && toWindow != kLayerNone){
		//menuOld->setVisible(true);
		//node->setCall
		menuOld->removeAllChildren();

		auto btnOk = MenuItemImage::create(
			"buttons/Ok.png",
			"buttons/OkClicked.png",
			CC_CALLBACK_0(LoginLayer::bringWindow, this, toWindow, "", kLayerNone));
		btnOk->setPosition(Point(VisibleRect::center().x, VisibleRect::center().y - 35));

		menuOld->addChild(btnOk);
		menuOld->setVisible(true);
	}
	else if (menuOld && !show){
		menuOld->setVisible(false);
	}

}*/

/*void LoginLayer::onNewGame(Ref* pSender)
{
	// Start the game 
	//Director::getInstance()->replaceScene(CharacterCreation::createScene());
}


void LoginLayer::onOptions(Ref* pSender)
{
	tryConnecting();
}


void LoginLayer::onQuit(Ref* pSender)
{
	// Exit game
	Director::getInstance()->end();
}*/

bool LoginLayer::onVisit(TextFieldTTF * sender, Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	switch (_window){
	case kLayerMain: break;
	case kLayerCreateAccount:
	{
		TextFieldTTF *textField = (TextFieldTTF *)this->getChildByTag(kTextFieldAccountName);
		LabelTTF *cursor = (LabelTTF *)this->getChildByTag(kCursorAccountName);
		if (textField && cursor) {
			cursor->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		}

		textField = (TextFieldTTF *)this->getChildByTag(kTextFieldEmail);
		cursor = (LabelTTF *)this->getChildByTag(kCursorEmail);
		if (textField && cursor) {
			cursor->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		}

		textField = (TextFieldTTF *)this->getChildByTag(kTextFieldNewPassword);
		cursor = (LabelTTF *)this->getChildByTag(kCursorNewPassword);
		if (textField && cursor) {
			cursor->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		}

		textField = (TextFieldTTF *)this->getChildByTag(kTextFieldRetypePassword);
		cursor = (LabelTTF *)this->getChildByTag(kCursorRetypePassword);
		if (textField && cursor) {
			cursor->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		}
	}
		break;
	case kLayerLogin:
	{
		TextFieldTTF *textField = (TextFieldTTF *)this->getChildByTag(kTextFieldAccount);
		LabelTTF *cursor = (LabelTTF *)this->getChildByTag(kCursorAccount);
		if (textField && cursor) {
			cursor->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		}

		textField = (TextFieldTTF *)this->getChildByTag(kTextFieldPassword);
		cursor = (LabelTTF *)this->getChildByTag(kCursorPassword);
		if (textField && cursor) {
			cursor->setPosition(Point(textField->getPosition().x + textField->getContentSize().width, textField->getPosition().y));
		}
	}
		break;
	/*case kLayerMessage:break;*/
	case kLayerNone:break;
	}

	CC_UNUSED_PARAM(sender);
	return false;
}

bool LoginLayer::onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen)
{
	if (sender->getString().length() + nLen < MAX_LETTERS){
		if (sender->getTag() == kTextFieldPassword){
			_password.append(text);
			string hiddenPassword = sender->getString();
			for (unsigned int i = 0; i < nLen; i++){
				hiddenPassword.append("*");
			}
			sender->setString(hiddenPassword);
			return true;
		}
		else if (sender->getTag() == kTextFieldNewPassword){
			_newPassword.append(text);
			string hiddenPassword = sender->getString();
			for (unsigned int i = 0; i < nLen; i++){
				hiddenPassword.append("*");
			}
			sender->setString(hiddenPassword);
			return true;
		}
		else if (sender->getTag() == kTextFieldRetypePassword){
			_retypePassword.append(text);
			string hiddenPassword = sender->getString();
			for (unsigned int i = 0; i < nLen; i++){
				hiddenPassword.append("*");
			}
			sender->setString(hiddenPassword);
			return true;
		}
		return false;//append
	}
	
	return true; 

}

bool LoginLayer::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen)
{
	/*if (sender->getTag() == kTextFieldPassword){
		_password.erase(_password.end() - nLen, _password.end());
	}
	else if (sender->getTag() == kTextFieldNewPassword){
		_newPassword.erase(_newPassword.end() - nLen, _newPassword.end());
	}
	else if (sender->getTag() == kTextFieldRetypePassword){
		_retypePassword.erase(_retypePassword.end() - nLen, _retypePassword.end());
	}*/

	switch (sender->getTag()){
	case kTextFieldPassword: _password.erase(_password.end() - nLen, _password.end());
		break;
	case kTextFieldNewPassword: _newPassword.erase(_newPassword.end() - nLen, _newPassword.end());
		break;
	case kTextFieldRetypePassword: _retypePassword.erase(_retypePassword.end() - nLen, _retypePassword.end());
		break;
	default: break;
	}
	//CC_UNUSED_PARAM(sender);
	CC_UNUSED_PARAM(delText);
	//CC_UNUSED_PARAM(nLen);
	return false;
}

void LoginLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);

	switch (_window){
	case kLayerMain: break;
	case kLayerCreateAccount:
	{
		TextFieldTTF *textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldAccountName);
		LabelTTF *cursor = (LabelTTF *)this->getChildByTag(kCursorAccountName);
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
		textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldEmail);
		cursor = (LabelTTF *)this->getChildByTag(kCursorEmail);
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
		textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldNewPassword);
		cursor = (LabelTTF *)this->getChildByTag(kCursorNewPassword);
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
		textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldRetypePassword);
		cursor = (LabelTTF *)this->getChildByTag(kCursorRetypePassword);
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
	}
		break;
	case kLayerLogin:
	{
		TextFieldTTF *textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldAccount);
		LabelTTF *cursor = (LabelTTF *)this->getChildByTag(kCursorAccount);
		if (textfield && cursor) {
			//Rect rect = textfield->getBoundingBox();
			Rect rect = Rect::Rect(textfield->getPositionX(), textfield->getPositionY(), 111, 14);
			if (rect.containsPoint(touchLocation)) {
				textfield->attachWithIME();
				cursor->setVisible(true);
			}
			else{
				textfield->detachWithIME();
				cursor->setVisible(false);
			}
		}
		textfield = (TextFieldTTF *)this->getChildByTag(kTextFieldPassword);
		cursor = (LabelTTF *)this->getChildByTag(kCursorPassword);
		if (textfield && cursor) {
			//Rect rect = textfield->getBoundingBox();
			Rect rect = Rect::Rect(textfield->getPositionX(), textfield->getPositionY(), 111, 14);
			if (rect.containsPoint(touchLocation)) {
				textfield->attachWithIME();
				cursor->setVisible(true);
			}
			else{
				textfield->detachWithIME();
				cursor->setVisible(false);
			}
		}
	}
		break;
	/*case kLayerMessage:break;*/
	case kLayerNone:break;
	}

}


void LoginLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event *event){

	switch (keycode){
	case EventKeyboard::KeyCode::KEY_BACK:
	{
		Touch *touch = new Touch();
		touch->setTouchInfo(0, 0.0f, 0.0f);
		onTouchEnded(touch, NULL);
	}
		switch (_window){
		case kLayerMain: 
			Director::getInstance()->end();
			break;
		case kLayerCreateAccount:
			bringWindow(kLayerMain);
			break;
		case kLayerLogin:
			bringWindow(kLayerMain);
			break;
		/*case kLayerMessage:
			bringWindow(kLayerMain);*/
			break;
		}

		break;
	case EventKeyboard::KeyCode::KEY_MENU:
	{
		Touch *touch = new Touch();
		touch->setTouchInfo(0, 0.0f, 0.0f);
		onTouchEnded(touch, NULL);
	
		//MessageBox("message goes here", "MSG TITLE");
		gotoLoading();
		break;
	}
	case EventKeyboard::KeyCode::KEY_GRAVE:
		gotoCharacterSelection();
		//changeScene();
		break;
	}
}