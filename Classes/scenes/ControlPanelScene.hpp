//
//  ControlPanelScene.hpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-08.
//
//

#ifndef ControlPanelScene_hpp
#define ControlPanelScene_hpp

#include <sstream>
#include <iomanip>
#include "cocos2d.h"
#include "cocos-ext.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "ui\CocosGUI.h"
#else
#include "CocosGUI.h"
#endif
//#include "extensions\cocos-ext.h"




class ControlPanelScene : public cocos2d::Layer, public cocos2d::extension::EditBoxDelegate
{
public:
	static cocos2d::Scene* createScene();

	bool init() override;

	void menuCloseCallback(cocos2d::Ref* pSender);
	void onButtonTcpListen(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonTcpConnect(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSctpListen(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonSctpConnect(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void onButtonTestRohc(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonTestLz(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void onButtonTestHuffman(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
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
	void update(float dt) override;
	
	CREATE_FUNC(ControlPanelScene);
private:
	bool _isTcpServer;
	bool _isTcpClient;
	bool _isSctpServer;
	bool _isSctpClient;
    bool _rohc;
    bool _lz;
    bool _huffman;
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
    cocos2d::ui::Button *_buttonTestRohc;
    cocos2d::ui::Button *_buttonTestLz;
    cocos2d::ui::Button *_buttonTestHuffman;
	cocos2d::ui::CheckBox *_checkboxRohc;
	cocos2d::ui::CheckBox *_checkboxLz;
	cocos2d::ui::CheckBox *_checkboxHuffman;
	cocos2d::ui::Text *_textMessages;

	void editBoxReturn( cocos2d::extension::EditBox* editBox ) override;
//    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
//    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
//    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
};

//class EbDelegate : public cocos2d::extension::EditBoxDelegate
//{
//public:
//    virtual void editBoxReturn( cocos2d::extension::EditBox* editBox );
//    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
//    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
//    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
//private:
//    //virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
//    //virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
//    //virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
//    //virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
//};
#endif /* ControlPanelScene_hpp */
