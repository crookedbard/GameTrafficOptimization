#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"

enum {
	kDialogHidden,
	kDialogWait,
	kDialogOrdinary,
	kDialogImportant
};

class Dialog : public cocos2d::Sprite
{
public:
	Dialog();
	~Dialog();

	static Dialog* create();

	/*shows popup message, 1 of 3 types
	*type - kDialogWait: waiting message, kDialogOrdinary: normal message, kDialogImportant: fades everything out except msg
	*text - message text, default is:"Please wait"
	*firstCallBack - Menu item first button callback
	*cancelBtn - If the second btn just closes the dialog
	*secondCallBack - Menu item second button callback
	*/
	void show(int type = kDialogWait, std::string text = "Please wait", cocos2d::SEL_MenuHandler firstCallBack = NULL, bool cancelBtn = false, cocos2d::SEL_MenuHandler secondCallBack = NULL);
	void hide(/*cocos2d::Ref* pSender*/);
	void showWait(std::string text = "Please wait");
	void showOrdinary(std::string text, bool twoButtons = false, cocos2d::ccMenuCallback firstCallBack = nullptr, cocos2d::ccMenuCallback secondCallBack = nullptr);
	void showImportant(std::string text, bool twoButtons = false, cocos2d::ccMenuCallback firstCallBack = nullptr, cocos2d::ccMenuCallback secondCallBack = nullptr);
private:
	int _dialogType;
	void _animateWaiting(cocos2d::Node* sender);
};

#endif // __DIALOG_H__