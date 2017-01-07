#ifndef __LOGINLAYER_H_
#define __LOGINLAYER_H_

#include "GameController.h"

enum {
	kLayerNone,
	kLayerMain,
	kLayerCreateAccount,
	kLayerLogin/*,
	kLayerMessage*/
};

enum {
	kTextFieldAccountName,
	kTextFieldEmail,
	kTextFieldNewPassword,
	kTextFieldRetypePassword,
	kTextFieldAccount,
	kTextFieldPassword,
	kCursorAccountName,
	kCursorEmail,
	kCursorNewPassword,
	kCursorRetypePassword,
	kCursorAccount,
	kCursorPassword,
	kSpriteCreateAccount,
	kSpriteLogin,
	kSpriteMessage,
	kMenuCreateAccount,
	kMenuLogin,
	kMenuMain,
	kMenuMessage,
	kLabelMessage
};

class LoginLayer : public cocos2d::Layer, public cocos2d::TextFieldDelegate, public GameController
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::Scene* createScene(cocos2d::Layer *layer); //dis wont work, wont change and return

	// functions for click even handling
	void onCreateAccount(cocos2d::Ref* pSender);
	void onPlayGame(cocos2d::Ref* pSender);
	void onExitGame(cocos2d::Ref* pSender);
	void onEnter(cocos2d::Ref* pSender);
	void onOk(cocos2d::Ref* pSender);
	void onCancel(cocos2d::Ref* pSender);
	
	//brings specific sprite/menus
	void bringWindow(int window/*, std::string msg = " ", byte toWindow = kLayerNone*/);
	void hideWindow();
	void showWindow();
	void showCreateAccount(bool show);
	void showLogin(bool show);
	void showMain(bool show);
	//void showMessage(bool show, std::string msg = " ", byte toWindow = kLayerNone);
	
	//Dialog methods
	void hideDialog(cocos2d::Ref *sender); //callback = removes dialog
	void showDialog(std::string str);
	void showDialogCallbackWindow(std::string str, int window);
	void hideDialogBringWindow(cocos2d::Ref *sender, int window);
	//packet methods
	void sendLoginData();
	void sendRegistrationData();

	//Message tools
	//void setMessage(std::string msg, byte window = kNone);

	//Handle key events
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	CREATE_FUNC(LoginLayer);

	//TextFieldttf delegate begin
	virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF * sender, const char * text, size_t nLen);
	virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF * sender, const char * delText, size_t nLen);
	virtual bool onVisit(cocos2d::TextFieldTTF * sender, cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);

	//virtual bool onDraw(cocos2d::TextFieldTTF * sender);
	//virtual bool onUpdate();
	//virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF * sender, const char * delText, size_t nLen) override;
	//virtual void deleteBackward() override;
	//virtual bool onTextFieldAttachWithIME(cocos2d::TextFieldTTF * pSender){ return false; };
	//virtual bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF * pSender){ return false; };
	//virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF * pSender, const char * text, int nLen){ return false; };
	//virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF * pSender, const char * delText, int nLen){ return false; };
	//virtual bool onDraw(cocos2d::TextFieldTTF * pSender){return false;};
	//byte _window; 
	

	//CC_SYNTHESIZE(Dialog*, _dialog, Dialog);

private:
	int _window; 
	std::string _password;
	std::string _newPassword;
	std::string _retypePassword;
	Dialog *_dialog;
	//byte _previousWindow;
	//std::string _message;
	//cocos2d::Menu* _mainMenu;
	//cocos2d::Menu* _createAccountMenu;
	//cocos2d::Sprite* _createAccountWindow;
	//cocos2d::Menu* _loginMenu;
	//cocos2d::Sprite* _loginWindow;
	//cocos2d::Menu* _messageMenu;
	//cocos2d::Sprite* _messageWindow;
};



#endif // __LOGINLAYER_H_
