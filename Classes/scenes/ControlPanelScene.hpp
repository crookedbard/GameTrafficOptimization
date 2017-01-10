//
//  ControlPanelScene.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-08.
//
//

#ifndef ControlPanelScene_hpp
#define ControlPanelScene_hpp

#include "cocos2d.h"
//#include "extensions\cocos-ext.h"
#include "cocos-ext.h"
//#include "ui\CocosGUI.h"
#include "CocosGUI.h"
#include <sstream>
#include <iomanip>

class ControlPanelScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);
	void onButtonTcpListen(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonTcpConnect(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSctpListen(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSctpConnect(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSendFewInts(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSendFewStrings(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSendManyInts(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSendManyStrigs(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void selectedEventRohc(Ref* pSender,cocos2d::ui::CheckBox::EventType type);
	void selectedEventLz(Ref* pSender,cocos2d::ui::CheckBox::EventType type);
	void selectedEventHuffman(Ref* pSender,cocos2d::ui::CheckBox::EventType type);
	bool readSettings();
	bool loadPanel();
	void addMessage(std::string msg);
	std::string doubleToString(double dbl);
	void update(float dt);
	
	CREATE_FUNC(ControlPanelScene);
private:
	bool _isTcpServer;
	bool _isTcpClient;
	bool _isSctpServer;
	bool _isSctpClient;
	std::string _ipAddress;
	double _port;
	double _tcpSocket;
	std::vector<std::pair<std::string, double>> _connections;

	//hud
	std::vector<std::string> _messages;
	cocos2d::Label *_labelConnectionStatus;
	cocos2d::ui::Button *_buttonTcpListen;
	cocos2d::ui::Button *_buttonTcpConnect;
	cocos2d::ui::Button *_buttonSctpListen;
	cocos2d::ui::Button *_buttonSctpConnect;
	cocos2d::ui::CheckBox *_checkboxRohc;
	cocos2d::ui::CheckBox *_checkboxLz;
	cocos2d::ui::CheckBox *_checkboxHuffman;
	cocos2d::ui::Text *_textMessages;
};

#endif /* ControlPanelScene_hpp */
