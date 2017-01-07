#include "Dialog.h"
#include "VisibleRect.h"

USING_NS_CC;
using namespace std;

Dialog::Dialog()
{
	_dialogType = kDialogHidden;
}

Dialog::~Dialog()
{
	//CC_SAFE_RELEASE(_rideAnimation);
	//CC_SAFE_RELEASE(_floatAnimation);
}

Dialog* Dialog::create() {
	Dialog * sprite = new Dialog();
	if (sprite && sprite->initWithFile("hud/MessageWindow.png")){
		//sprite->setAnchorPoint(Point(0.5, 0.5));
		sprite->setPosition(VisibleRect::center());
		sprite->setVisible(false);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Dialog::show(int type, string text, SEL_MenuHandler firstCallBack, bool cancelBtn, SEL_MenuHandler secondCallBack){
	//this->removeAllChildren();
	_dialogType = type;
	//Disables layers event listeners and enables this messages event listener
	this->getEventDispatcher()->pauseEventListenersForTarget(this->getParent(), true);
	this->getEventDispatcher()->resumeEventListenersForTarget(this, true);
	//Add message text
	auto label = LabelTTF::create(text, "fonts/Marker Felt.ttf", 16);
	
	this->addChild(label);

	switch (_dialogType){
	case kDialogWait:
	{
		//add waiting/loading animation
		label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	}
		break;
	case kDialogOrdinary:
	{
							if (!secondCallBack && !cancelBtn){
								auto btnOk = MenuItemImage::create(
									"buttons/Ok.png",
									"buttons/OkClicked.png",
									this->getParent(),
									firstCallBack);
								btnOk->setPosition(Point(0, -30));
								auto menu = Menu::create(btnOk, NULL);
								menu->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
								this->addChild(menu);
							}
							else{
								auto btnYes = MenuItemImage::create(
									"buttons/Ok.png",
									"buttons/OkClicked.png",
									this->getParent(),
									firstCallBack);
								btnYes->setPosition(Point(-50, -30));
								auto btnNo = MenuItemImage::create(
									"buttons/Cancel.png",
									"buttons/CancelClicked.png");
								(cancelBtn) ? btnNo->setCallback(std::bind(&Dialog::hide, this)) : btnNo->setCallback([&](cocos2d::Ref *sender)
								{
									secondCallBack;
								});
								btnNo->setPosition(Point(50, -30));

								auto menu = Menu::create(btnYes, btnNo, NULL);
								menu->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
								this->addChild(menu);
							}
							label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2 + 10));
	}
		break;
	case kDialogImportant:
	{
							 auto btnYes = MenuItemImage::create(
								 "buttons/Ok.png",
								 "buttons/OkClicked.png",
								 this->getParent(),
								 firstCallBack);
							 btnYes->setPosition(Point(-50, -30));

							auto btnNo = MenuItemImage::create(
								"buttons/Cancel.png",
								"buttons/CancelClicked.png"/*,
								CC_CALLBACK_1(Dialog::hide, this)*/);

							
							/*auto test = CallFunc::create(
								this,
								callfunc_selector(Dialog::hide));
							auto btnNo2 = MenuItemImage::create(
								"buttons/Cancel.png",
								"buttons/CancelClicked.png" );
							btnNo2->setCallback(std::bind(&Dialog::hide, this));*/
							//btnNo->setCallback([&](Dialog::hide));
							(cancelBtn) ? btnNo->setCallback(std::bind(&Dialog::hide, this)) : btnNo->setTarget(this->getParent(), secondCallBack);
							//btnNo->setCallback(&secondCallBack);
								/*[&](cocos2d::Ref *sender)
							{
								//secondCallBack;
								(sender->*secondCallBack)(target);
								CCLOG("lambda callback");
							});*/
							 //auto callback = menu_selector(Dialog::hide);
							 //btnNo->setCallback(callback);//CallFunc::create(CC_CALLBACK_0(Dialog::hide, NULL)));
							 btnNo->setPosition(Point(50, -30));

							 /*auto btnEnterMini = MenuItemImage::create(
								 "buttons/EnterMini.png",
								 "buttons/EnterMiniClicked.png",
								 CC_CALLBACK_1(Dialog::hide,this));*/

							 auto menu = Menu::create(btnYes, btnNo, NULL);
							 menu->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
							 this->addChild(menu);

							 label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2 + 10));

							 for (const auto& child : this->getParent()->getChildren())
							 {
								 if (child != this) child->setOpacity(GLbyte(128));
							 }
	}
		break;
	}
	//make this message visible
	this->setVisible(true);
}

void Dialog::_animateWaiting(Node* sender){
	LabelTTF *dotLabel = (LabelTTF *)this->getChildByName("DotAnimation");
	if (dotLabel){
		string text = dotLabel->getString();
		(text.length() < 8) ? dotLabel->setString(text.append(". ")) : dotLabel->setString(" . ");
	}
}

void Dialog::showWait(string text){

	if (_dialogType != kDialogHidden) this->removeAllChildren();
	_dialogType = kDialogWait;

	this->getEventDispatcher()->pauseEventListenersForTarget(this->getParent(), true);
	//this->getEventDispatcher()->resumeEventListenersForTarget(this, true);

	auto label = LabelTTF::create(text, "fonts/Marker Felt.ttf", 16);
	label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(label);


	//animation
	auto dotLabel = LabelTTF::create(" . ", "fonts/Marker Felt.ttf", 16);
	dotLabel->setName("DotAnimation");
	dotLabel->setAnchorPoint(Vec2(0,0.5));
	dotLabel->setPosition(Point(this->getContentSize().width / 2 + label->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(dotLabel);
	label->runAction(RepeatForever::create(CCSequence::create(DelayTime::create(0.2), CCCallFuncN::create(this, callfuncN_selector(Dialog::_animateWaiting)), NULL)));

	this->setVisible(true);
}

void Dialog::showOrdinary(string text, bool twoButtons, ccMenuCallback firstCallBack, ccMenuCallback secondCallBack){

	if(_dialogType != kDialogHidden) this->removeAllChildren();
	_dialogType = kDialogOrdinary;

	this->getEventDispatcher()->pauseEventListenersForTarget(this->getParent(),true);
	this->getEventDispatcher()->resumeEventListenersForTarget(this, true);

	auto label = LabelTTF::create(text, "fonts/Marker Felt.ttf", 16);
	label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2 + 10));
	this->addChild(label);

	auto menu = Menu::create();
	menu->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(menu);

	auto btnOne = MenuItemImage::create(
		"buttons/Ok.png",
		"buttons/OkClicked.png");

	if (firstCallBack == nullptr){
		btnOne->setCallback(std::bind(&Dialog::hide, this));
	}
	else{
		btnOne->setCallback(firstCallBack);
	}

	btnOne->setPosition(Point(0, -30));
	menu->addChild(btnOne);

	if (twoButtons){
		btnOne->setNormalImage(Sprite::create("buttons/Ok.png")); //should be Yes button
		btnOne->setSelectedImage(Sprite::create("buttons/OkClicked.png")); //should be Yes button clicked
		btnOne->setPosition(Point(-50, -30));

		auto btnTwo = MenuItemImage::create(
			"buttons/Cancel.png",
			"buttons/CancelClicked.png");

		if (secondCallBack == nullptr){
			btnTwo->setCallback(std::bind(&Dialog::hide, this));
		}
		else{
			btnTwo->setCallback(secondCallBack);
		}

		btnTwo->setPosition(Point(50, -30));
		menu->addChild(btnTwo);
	}

	this->setVisible(true);
}

void Dialog::showImportant(string text, bool twoButtons, ccMenuCallback firstCallBack, ccMenuCallback secondCallBack){

	if (_dialogType != kDialogHidden) this->removeAllChildren();
	_dialogType = kDialogImportant;

	this->getEventDispatcher()->pauseEventListenersForTarget(this->getParent(), true);
	this->getEventDispatcher()->resumeEventListenersForTarget(this, true);

	for (const auto& child : this->getParent()->getChildren())
	{
		if (child != this) child->setOpacity(GLbyte(128));
	}

	auto label = LabelTTF::create(text, "fonts/Marker Felt.ttf", 16);
	label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2 + 10));
	this->addChild(label);

	auto menu = Menu::create();
	menu->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(menu);

	auto btnOne = MenuItemImage::create(
		"buttons/Ok.png",
		"buttons/OkClicked.png");

	if (!firstCallBack){
		btnOne->setCallback(std::bind(&Dialog::hide, this));
	}
	else{
		btnOne->setCallback(firstCallBack);
	}

	btnOne->setPosition(Point(0, -30));
	menu->addChild(btnOne);

	if (twoButtons){
		btnOne->setNormalImage(Sprite::create("buttons/Ok.png")); //should be Yes button
		btnOne->setSelectedImage(Sprite::create("buttons/OkClicked.png")); //should be Yes button clicked
		btnOne->setPosition(Point(-50, -30));

		auto btnTwo = MenuItemImage::create(
			"buttons/Cancel.png",
			"buttons/CancelClicked.png");

		if (!secondCallBack){
			btnTwo->setCallback(std::bind(&Dialog::hide, this));
		}
		else{
			btnTwo->setCallback(secondCallBack);
		}

		btnTwo->setPosition(Point(50, -30));
		menu->addChild(btnTwo);
	}

	this->setVisible(true);
}

void Dialog::hide(/*Ref* pSender*/){

	if (_dialogType == kDialogHidden)return;
	if (_dialogType == kDialogWait) this->getChildByName("DotAnimation")->stopAllActions();

	this->getEventDispatcher()->pauseEventListenersForTarget(this, true);
	this->getEventDispatcher()->resumeEventListenersForTarget(this->getParent(), true);
	this->removeAllChildren();

	if (_dialogType == kDialogImportant){
		for (const auto& child : this->getParent()->getChildren())
		{
			if (child != this) child->setOpacity(GLbyte(255));
		}
	}
	
	this->setVisible(false);

	_dialogType = kDialogHidden;
}

