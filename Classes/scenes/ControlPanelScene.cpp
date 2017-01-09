//
//  ControlPanelScene.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-08.
//
//

#include "ControlPanelScene.hpp"
#include "VisibleRect.h"
#include <exception>
#include "39dll.h"

USING_NS_CC;
using namespace cocos2d::ui;

const std::string SETTINGS_FILENAME = "Settings.txt";
const std::string FONT_NAME = "fonts/arial.ttf";
const int FONT_SIZE = 15;
const int FONT_SIZE_MESSAGE = 14;
const int DEPTH_HUD = 1;
const int MAX_MESSAGE_COUNT = 20;

Scene* ControlPanelScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ControlPanelScene::create();
    scene->addChild(layer);
    return scene;
}

bool ControlPanelScene::init()
{
    if (!Layer::init()) return false;
    if (!readSettings()) return false;
    if (!loadPanel()) return false;
    
    //LayerColor::initWithColor(cocos2d::Color4B(0,0,0,0));
    
    //    auto visibleSize = Director::getInstance()->getVisibleSize();
    //    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto panel = Panel::create()
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(ControlPanelScene::menuCloseCallback, this));
    
    closeItem->setPosition(VisibleRect::rightBottom().x - closeItem->getContentSize().width/2 ,
                           VisibleRect::rightBottom().y + closeItem->getContentSize().height/2);
    
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, DEPTH_HUD);
    
    //    auto labelIpAddress = Label::createWithTTF(_ipAddress.c_str(), FONT_NAME.c_str(), FONT_SIZE);
    //    labelIpAddress->setPosition(VisibleRect::center().x - labelIpAddress->getWidth(), VisibleRect::center().y + labelIpAddress->getHeight());
    //    this->addChild(labelIpAddress, DEPTH_HUD);
    
    return true;
}

bool ControlPanelScene::loadPanel()
{
    try{
    // Create the scrollview by vertical
        auto scrollView = ui::ScrollView::create();
        scrollView->setContentSize(Size(VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height));
        Size backgroundSize = this->getContentSize();
        scrollView->setPosition(Vec2(VisibleRect::leftTop().x, VisibleRect::rightBottom().y));
        this->addChild(scrollView);
        
        scrollView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        scrollView->setBackGroundColor(Color3B(128, 128, 128));
        
        float innerWidth = scrollView->getContentSize().width;
        float innerHeight = scrollView->getContentSize().height;
        
        scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
        const float LINE_HEIGHT = 30.0f;
        //line 1. Connection info
        auto labelIpAddress = Label::createWithTTF(_ipAddress + ":" + doubleToString(_port), FONT_NAME, FONT_SIZE);
        labelIpAddress->setPosition(Vec2(10, scrollView->getInnerContainerSize().height - labelIpAddress->getContentSize().height ));
        labelIpAddress->setAnchorPoint(Vec2::ZERO);
        scrollView->addChild(labelIpAddress);

        _labelConnectionStatus = Label::createWithTTF("Disconnected", FONT_NAME, FONT_SIZE);
        _labelConnectionStatus->setColor(Color3B(255, 0, 0));
        _labelConnectionStatus->setPosition(Vec2(innerWidth - 180.0f, scrollView->getInnerContainerSize().height - _labelConnectionStatus->getContentSize().height ));
        _labelConnectionStatus->setAnchorPoint(Vec2::ZERO);
        scrollView->addChild(_labelConnectionStatus);
        
        //line 2. TCP
        auto labelTcp = Label::createWithTTF("TCP", FONT_NAME, FONT_SIZE);
        labelTcp->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 2.0f ));
        scrollView->addChild(labelTcp);
        
        _buttonTcpListen = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        _buttonTcpListen->setScale9Enabled(true);
        _buttonTcpListen->setTitleText("Listen");
        _buttonTcpListen->setContentSize(Size(100.0f, 25.0f));
        _buttonTcpListen->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 2.0f));
        scrollView->addChild(_buttonTcpListen);
        _buttonTcpListen->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTcpListen, this));
        
        _buttonTcpConnect = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        _buttonTcpConnect->setScale9Enabled(true);
        _buttonTcpConnect->setTitleText("Connect");
        _buttonTcpConnect->setContentSize(Size(100.0f, 25.0f));
        _buttonTcpConnect->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 2.0f));
        scrollView->addChild(_buttonTcpConnect);
        _buttonTcpConnect->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTcpConnect, this));
        
        //line 3. SCTP
        auto labelSctp = Label::createWithTTF("SCTP", FONT_NAME, FONT_SIZE);
        labelSctp->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 3.0f ));
        scrollView->addChild(labelSctp);
        
        _buttonSctpListen = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        _buttonSctpListen->setScale9Enabled(true);
        _buttonSctpListen->setTitleText("Listen");
        _buttonSctpListen->setContentSize(Size(100.0f, 25.0f));
        _buttonSctpListen->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 3.0f));
        scrollView->addChild(_buttonSctpListen);
        _buttonSctpListen->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSctpListen, this));
        
        _buttonSctpConnect = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        _buttonSctpConnect->setScale9Enabled(true);
        _buttonSctpConnect->setTitleText("Connect");
        _buttonSctpConnect->setContentSize(Size(100.0f, 25.0f));
        _buttonSctpConnect->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 3.0f));
        scrollView->addChild(_buttonSctpConnect);
        _buttonSctpConnect->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSctpConnect, this));
        
        //line 4. ROHC
        auto labelRohc = Label::createWithTTF("ROHC", FONT_NAME, FONT_SIZE);
        labelRohc->setPosition(Vec2(innerWidth / 2.0f - 30.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 4.0f ));
        scrollView->addChild(labelRohc);
        
        _checkboxRohc = CheckBox::create();
        _checkboxRohc->setScale(1.4f);
        _checkboxRohc->setTouchEnabled(true);
        _checkboxRohc->loadTextures("cocosui/check_box_normal.png",
                               "cocosui/check_box_normal_press.png",
                               "cocosui/check_box_active.png",
                               "cocosui/check_box_normal_disable.png",
                               "cocosui/check_box_active_disable.png");
        _checkboxRohc->setPosition(Vec2(innerWidth / 2.0f +30.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 4.0f ));
        _checkboxRohc->addEventListener(CC_CALLBACK_2(ControlPanelScene::selectedEventRohc,this));
        scrollView->addChild(_checkboxRohc);
        
        //line 5. LZ4
        auto labelLz = Label::createWithTTF("LZ4", FONT_NAME, FONT_SIZE);
        labelLz->setPosition(Vec2(innerWidth / 2.0f - 30.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 5.0f ));
        scrollView->addChild(labelLz);
        
        _checkboxLz = CheckBox::create("cocosui/check_box_normal.png",
                                   "cocosui/check_box_normal_press.png",
                                   "cocosui/check_box_active.png",
                                   "cocosui/check_box_normal_disable.png",
                                   "cocosui/check_box_active_disable.png");
        _checkboxLz->setScale(1.4f);
        _checkboxLz->setTouchEnabled(true);
        _checkboxLz->setPosition(Vec2(innerWidth / 2.0f + 30.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 5.0f ));
        _checkboxLz->addEventListener(CC_CALLBACK_2(ControlPanelScene::selectedEventLz,this));
        scrollView->addChild(_checkboxLz);
        
        //line 6. Huffman
        auto labelHuffman = Label::createWithTTF("Huffman", FONT_NAME, FONT_SIZE);
        labelHuffman->setPosition(Vec2(innerWidth / 2.0f - 30.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 6.0f ));
        scrollView->addChild(labelHuffman);
        
        _checkboxHuffman = CheckBox::create("cocosui/check_box_normal.png",
                                       "cocosui/check_box_normal_press.png",
                                       "cocosui/check_box_active.png",
                                       "cocosui/check_box_normal_disable.png",
                                       "cocosui/check_box_active_disable.png");
        _checkboxHuffman->setTouchEnabled(true);
        _checkboxHuffman->setScale(1.4f);
        _checkboxHuffman->setPosition(Vec2(innerWidth / 2.0f + 30.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 6.0f ));
        _checkboxHuffman->addEventListener(CC_CALLBACK_2(ControlPanelScene::selectedEventHuffman,this));
        scrollView->addChild(_checkboxHuffman);
        
        //line 7. Send Packets
        auto labelSendPackets = Label::createWithTTF("Send packets", FONT_NAME, FONT_SIZE);
        labelSendPackets->setPosition(Vec2(innerWidth / 2.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 7.0f ));
        scrollView->addChild(labelSendPackets);
        
        //line 8. Button Packet line 1
        auto buttonPacketIntFew = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        buttonPacketIntFew->setScale9Enabled(true);
        buttonPacketIntFew->setTitleText("Few ints");
        buttonPacketIntFew->setContentSize(Size(100.0f, 25.0f));
        buttonPacketIntFew->setPosition(Vec2(innerWidth / 2.0f - 60.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 8.0f));
        scrollView->addChild(buttonPacketIntFew);
        buttonPacketIntFew->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendFewInts, this));
        
        auto buttonPacketStringFew = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        buttonPacketStringFew->setScale9Enabled(true);
        buttonPacketStringFew->setTitleText("Few strings");
        buttonPacketStringFew->setContentSize(Size(100.0f, 25.0f));
        buttonPacketStringFew->setPosition(Vec2(innerWidth / 2.0f + 50.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 8.0f));
        scrollView->addChild(buttonPacketStringFew);
        buttonPacketStringFew->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendFewStrings, this));
        
        //line 9. Button Packet line 2
        auto buttonPacketIntMany = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        buttonPacketIntMany->setScale9Enabled(true);
        buttonPacketIntMany->setTitleText("Many ints");
        buttonPacketIntMany->setContentSize(Size(100.0f, 25.0f));
        buttonPacketIntMany->setPosition(Vec2(innerWidth / 2.0f - 60.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 9.0f));
        scrollView->addChild(buttonPacketIntMany);
        buttonPacketIntMany->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendManyInts, this));
        
        auto buttonPacketStringMany = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
        buttonPacketStringMany->setScale9Enabled(true);
        buttonPacketStringMany->setTitleText("Many strings");
        buttonPacketStringMany->setContentSize(Size(100.0f, 25.0f));
        buttonPacketStringMany->setPosition(Vec2(innerWidth / 2.0f + 50.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 9.0f));
        scrollView->addChild(buttonPacketStringMany);
        buttonPacketStringMany->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendManyStrigs, this));
        
        //line 10. TextField
        //_messages = { "chatas", "testas", "testas", "testas", "testas", "testas" };
        _textMessages = Text::create("",FONT_NAME, FONT_SIZE_MESSAGE );
        _textMessages->ignoreContentAdaptWithSize(false);
        _textMessages->setContentSize(Size(innerWidth - 4.0f, 300.0f));
        _textMessages->setTextHorizontalAlignment(TextHAlignment::LEFT);
        _textMessages->setPosition(Vec2(2.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * 10.0f));
        _textMessages->setAnchorPoint(Vec2(0.0f, 1.0f));
        scrollView->addChild(_textMessages);
        
        addMessage("Result area");
    }
    catch(std::exception& e){
        std::string ex ="Standard exception: " ;
        ex += e.what() ;
        ex += "\n";
        printf("%s", ex.c_str());
        return false;
    }
    return true;
}

void ControlPanelScene::onButtonTcpListen(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(!_isTcpServer)
        {
            dllInit();
            _tcpSocket = tcplisten(_port, 2, 1);
            
            if(socket <= 0)
            {
                addMessage("Listening socket could not be opened on port " + doubleToString(_port));
                closesock(_tcpSocket);
                return;
            }
            else
            {
                setnagle(_tcpSocket, false);
                addMessage("Server listening for incoming connections on port " + doubleToString(_port));
            }
            
            _isTcpServer = true;
            _labelConnectionStatus->setString("TCP server listening");
            _labelConnectionStatus->setColor(Color3B(0, 255, 0));
            _buttonTcpListen->setTitleText("Stop listen");
            _buttonTcpConnect->setEnabled(false);
            _buttonTcpConnect->setBright(false);
            _buttonSctpListen->setEnabled(false);
            _buttonSctpListen->setBright(false);
            _buttonSctpConnect->setEnabled(false);
            _buttonSctpConnect->setBright(false);
        }
        else
        {
            _isTcpServer = false;
            _labelConnectionStatus->setString("Disconnected");
            _labelConnectionStatus->setColor(Color3B(255, 0, 0));
            _buttonTcpListen->setTitleText("Listen");
            _buttonTcpConnect->setEnabled(true);
            _buttonTcpConnect->setBright(true);
            _buttonSctpListen->setEnabled(true);
            _buttonSctpListen->setBright(true);
            _buttonSctpConnect->setEnabled(true);
            _buttonSctpConnect->setBright(true);
            
            closesock(_tcpSocket);
        }
    }
}

void ControlPanelScene::onButtonTcpConnect(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(!_isTcpClient)
        {
            dllInit();
            _tcpSocket = tcpconnect(_ipAddress, _port, 1);
            
            if (_tcpSocket <= 0){
                addMessage("Failed to connect!");
                closesock(_tcpSocket);
                return;
            }else{
                setnagle(_tcpSocket, false);
            }
            _isTcpClient = true;
            _labelConnectionStatus->setString("TCP client connected");
            _labelConnectionStatus->setColor(Color3B(0, 255, 0));
            _buttonTcpConnect->setTitleText("Disconnect");
            _buttonTcpListen->setEnabled(false);
            _buttonTcpListen->setBright(false);
            _buttonSctpListen->setEnabled(false);
            _buttonSctpListen->setBright(false);
            _buttonSctpConnect->setEnabled(false);
            _buttonSctpConnect->setBright(false);
        }
        else
        {
            _isTcpClient= false;
            _labelConnectionStatus->setString("Disconnected");
            _labelConnectionStatus->setColor(Color3B(255, 0, 0));
            _buttonTcpConnect->setTitleText("Connect");
            _buttonTcpConnect->setEnabled(true);
            _buttonTcpListen->setBright(true);
            _buttonSctpListen->setEnabled(true);
            _buttonSctpListen->setBright(true);
            _buttonSctpConnect->setEnabled(true);
            _buttonSctpConnect->setBright(true);
            
            closesock(_tcpSocket);
        }
    }
}

void ControlPanelScene::onButtonSctpListen(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(!_isSctpServer)
        {
            _isSctpServer = true;
            _labelConnectionStatus->setString("SCTP server listening");
            _labelConnectionStatus->setColor(Color3B(0, 255, 0));
            _buttonSctpListen->setTitleText("Stop listen");
            _buttonTcpConnect->setEnabled(false);
            _buttonTcpConnect->setBright(false);
            _buttonTcpListen->setEnabled(false);
            _buttonTcpListen->setBright(false);
            _buttonSctpConnect->setEnabled(false);
            _buttonSctpConnect->setBright(false);
            _checkboxRohc->setEnabled(false);
            _checkboxRohc->setBright(false);
        }
        else
        {
            _isSctpServer = false;
            _labelConnectionStatus->setString("Disconnected");
            _labelConnectionStatus->setColor(Color3B(255, 0, 0));
            _buttonSctpListen->setTitleText("Listen");
            _buttonTcpConnect->setEnabled(true);
            _buttonTcpConnect->setBright(true);
            _buttonTcpListen->setEnabled(true);
            _buttonTcpListen->setBright(true);
            _buttonSctpConnect->setEnabled(true);
            _buttonSctpConnect->setBright(true);
            _checkboxRohc->setEnabled(true);
            _checkboxRohc->setBright(true);
        }
    }
}

void ControlPanelScene::onButtonSctpConnect(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        if(!_isSctpClient)
        {
            _isSctpClient = true;
            _labelConnectionStatus->setString("SCTP client connected");
            _labelConnectionStatus->setColor(Color3B(0, 255, 0));
            _buttonSctpConnect->setTitleText("Disconnect");
            _buttonTcpConnect->setEnabled(false);
            _buttonTcpConnect->setBright(false);
            _buttonSctpListen->setEnabled(false);
            _buttonSctpListen->setBright(false);
            _buttonTcpListen->setEnabled(false);
            _buttonTcpListen->setBright(false);
            _checkboxRohc->setEnabled(false);
            _checkboxRohc->setBright(false);
        }
        else
        {
            _isSctpClient = false;
            _labelConnectionStatus->setString("Disconnected");
            _labelConnectionStatus->setColor(Color3B(255, 0, 0));
            _buttonSctpConnect->setTitleText("Listen");
            _buttonTcpConnect->setEnabled(true);
            _buttonTcpConnect->setBright(true);
            _buttonSctpListen->setEnabled(true);
            _buttonSctpListen->setBright(true);
            _buttonTcpListen->setEnabled(true);
            _buttonTcpListen->setBright(true);
            _checkboxRohc->setEnabled(true);
            _checkboxRohc->setBright(true);
        }
    }
}

void ControlPanelScene::onButtonSendFewInts(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        addMessage("Sending few ints: 123, 456");
    }
}

void ControlPanelScene::onButtonSendFewStrings(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        addMessage("Sending few strigs: Hello World!");
    }
}

void ControlPanelScene::onButtonSendManyInts(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        addMessage("Sending many ints: 123, 456, 789 ...");
    }
}

void ControlPanelScene::onButtonSendManyStrigs(Ref* pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        addMessage("Sending many strigs: Hello World! ...");
    }
}

void ControlPanelScene::selectedEventRohc(Ref* pSender,CheckBox::EventType type)
{
    switch (type)
    {
        case CheckBox::EventType::SELECTED:

            break;
            
        case CheckBox::EventType::UNSELECTED:

            break;
            
        default:
            break;
    }
}

void ControlPanelScene::selectedEventLz(Ref* pSender,CheckBox::EventType type)
{
    switch (type)
    {
        case CheckBox::EventType::SELECTED:
            
            break;
            
        case CheckBox::EventType::UNSELECTED:
            
            break;
            
        default:
            break;
    }
}

void ControlPanelScene::selectedEventHuffman(Ref* pSender,CheckBox::EventType type)
{
    switch (type)
    {
        case CheckBox::EventType::SELECTED:
            
            break;
            
        case CheckBox::EventType::UNSELECTED:
            
            break;
            
        default:
            break;
    }
}

bool ControlPanelScene::readSettings()
{
    _isTcpServer = false;
    _isTcpClient = false;
    _isSctpServer = false;
    _isSctpClient = false;
    auto sharedFileUtils = FileUtils::getInstance();
    const int settingsCount = 2; //ipAddress and port
    
    bool exist = sharedFileUtils->isFileExist(SETTINGS_FILENAME);
    if(!exist) return false;
    
    
    std::string str = sharedFileUtils->getStringFromFile(SETTINGS_FILENAME);
    
    if(str.empty()) return false;
    std::stringstream ss(str);
    std::string segment;
    std::vector<std::string> seglist;
    
    while(std::getline(ss, segment, '\n'))
    {
        seglist.push_back(segment);
    }
    if(seglist.size() < settingsCount) return false;
    
    _ipAddress = seglist[0];
    _port = std::stod(seglist[1]);
    
    return true;
}

void ControlPanelScene::addMessage(std::string msg)
{
    if(_messages.size() > MAX_MESSAGE_COUNT) _messages.erase(_messages.begin());
    _messages.push_back(msg);
    
    std::string fullMsg = "";
    for (std::vector<std::string>::iterator i = _messages.end(); i-- != _messages.begin(); ) {
        fullMsg += *i  + "\n";
    }
    _textMessages->setString(fullMsg);
}

std::string ControlPanelScene::doubleToString(double dbl)
{
    std::ostringstream strs;
    strs << dbl;
    return strs.str();
}

void ControlPanelScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
