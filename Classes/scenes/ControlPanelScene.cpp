//
//  ControlPanelScene.cpp
//  GameTrafficOptimization
//
//  Created by Dainius Kreivys on 2017-01-08.
//
//

#include "ControlPanelScene.hpp"
#include <exception>
//problem with paths
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "utils/VisibleRect.h"
#include "utils/ScreenLog.hpp"
#include "transport_protocols/tcp/39dll.h"
#include "controllers/PacketController.h"
#include "payload_compression/huffman/HuffmanCompression.hpp"
#include "payload_compression/lz4stream/Lz4Compression.hpp"
//#include "header_compression/rohc/RohcCompression.hpp" //we dont have libs for it.
#else
#include "39dll.h"
#include "PacketController.h"
#include "VisibleRect.h"
#include "ScreenLog.hpp"
#include "HuffmanCompression.hpp"
#include "Lz4Compression.hpp"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
#include "RohcCompression.hpp" //doesnt work on Iphone 5 and on emulator 5. but it works on emu >=6
#endif
#endif

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

	//    auto visibleSize = Director::getInstance()->getVisibleSize();
	//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(ControlPanelScene::menuCloseCallback, this));

	closeItem->setPosition(VisibleRect::rightBottom().x - closeItem->getContentSize().width / 2,
						   VisibleRect::rightBottom().y + closeItem->getContentSize().height / 2);


	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, DEPTH_HUD);
	this->scheduleUpdate();

	return true;
}

void ControlPanelScene::update(float dt)
{
	if (_isTcpServer)
	{
		auto incomingSocket = tcpaccept(_tcpSocket, 1);
		if (incomingSocket > 0)
		{
			std::string ip(lastinIP());
			std::string sok = doubleToString(incomingSocket);
			//addMessage("Trying to connect " + ip + ", socket: " + sok);
			g_screenLog->log( LL_INFO, "Someones connecting: %s socket: %s", ip.c_str(), sok.c_str() );

			auto exists = false;
			for (auto i : _connections)
				{
					if (i.first == lastinIP())
					{
						exists = true;
						break;
					}
				}
			if (!exists)
			{
				_connections.push_back(std::make_pair(lastinIP(), incomingSocket));
			}
			else closesock(incomingSocket);
		}

		//for (auto i : _connections)
		for (unsigned int i = 0; i < _connections.size(); i++)
			{
				auto size = receivemessage(_connections[i].second);
				if (size > 0)
				{
					//addMessage("Packet received. Size: " + doubleToString(size));
					g_screenLog->log( LL_INFO, "Packet received. Size: %s", doubleToString(size).c_str());
					//handle packet payload
					PacketController::read();
				}
				if (size == 0)
				{
					//addMessage("Connection ended: " + _connections[i].first);
					g_screenLog->log( LL_WARNING, "Connection ended: %s", _connections[i].first.c_str());
					_connections.erase(_connections.begin() + i);
				}
			}
	}

	if (_isTcpClient)
	{
		auto size = receivemessage(_tcpSocket);
		if (size > 0)
		{
			//addMessage("Packet received. Size: " + doubleToString(size));
			g_screenLog->log( LL_INFO, "Packet received. Size: %s", doubleToString(size).c_str());
			//handle packet payload
			PacketController::read();
		}
		else if (size == 0)
		{
			//addMessage("Connection to server ended.");
			g_screenLog->log( LL_WARNING, "Connection to server ended.");
			onButtonTcpConnect(nullptr, Widget::TouchEventType::ENDED);
			//closesock(_tcpSocket);
		}
	}
}

bool ControlPanelScene::loadPanel()
{
	try
	{
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
		//		auto labelIpAddress = Label::createWithTTF(_ipAddress + ":" + doubleToString(_port), FONT_NAME, FONT_SIZE);
		//		labelIpAddress->setPosition(Vec2(10.0f, scrollView->getInnerContainerSize().height - labelIpAddress->getContentSize().height ));
		//		labelIpAddress->setAnchorPoint(Vec2::ZERO);
		//		scrollView->addChild(labelIpAddress);

		auto _editName = ui::EditBox::create(Size(150.0f, 25.0f), ui::Scale9Sprite::create());
		_editName->setPosition(Vec2(7.0f, scrollView->getInnerContainerSize().height - 25.0f));
		_editName->setFontName(FONT_NAME.c_str());
		_editName->setFontSize(FONT_SIZE);
		_editName->setFontColor(Color3B::WHITE);
		_editName->setPlaceHolder("IP address:");
		_editName->setPlaceholderFontColor(Color3B::WHITE);
		_editName->setMaxLength(20);
		_editName->setAnchorPoint(Vec2::ZERO);
		_editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		_editName->setText((_ipAddress + ":" + doubleToString(_port)).c_str());
		//_editName->setDelegate(this);
		//EbDelegate dbd; // = EbDelegate::EbDelegate();
		_editName->setDelegate(this);
		scrollView->addChild(_editName);

		//        auto labelIp = Label::createWithTTF(_editName->getText(), FONT_NAME, FONT_SIZE);
		//        auto labelPort = Label::createWithTTF(":" + doubleToString(_port), FONT_NAME, FONT_SIZE);
		//        labelPort->setPosition(Vec2(10.0f + labelIp->getContentSize().width, scrollView->getInnerContainerSize().height - labelPort->getContentSize().height - 4.0f));
		//        labelPort->setAnchorPoint(Vec2::ZERO);
		//        scrollView->addChild(labelPort);

		_labelConnectionStatus = Label::createWithTTF("Disconnected", FONT_NAME, FONT_SIZE);
		_labelConnectionStatus->setColor(Color3B(255.0f, 0.0f, 0.0f));
		_labelConnectionStatus->setPosition(Vec2(innerWidth - 150.0f, scrollView->getInnerContainerSize().height - _labelConnectionStatus->getContentSize().height - 4.0f));
		_labelConnectionStatus->setAnchorPoint(Vec2::ZERO);
		scrollView->addChild(_labelConnectionStatus);

		//line 1. Transoport protocols
		auto line = 1.0f;
		auto labelTransportProtocols = Label::createWithTTF("Transport protocols", FONT_NAME, FONT_SIZE);
		labelTransportProtocols->setPosition(Vec2(12.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		labelTransportProtocols->setAnchorPoint(Vec2(0.0f, 0.5f));
		labelTransportProtocols->setColor(Color3B(255.0f, 255.0f, 0.0f));
		scrollView->addChild(labelTransportProtocols);

		//line 2. TCP
		line += 1.0f;
		auto labelTcp = Label::createWithTTF("TCP", FONT_NAME, FONT_SIZE);
		labelTcp->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		//labelTcp->setAnchorPoint(Vec2(1.0f,0.5f));
		scrollView->addChild(labelTcp);

		_buttonTcpListen = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonTcpListen->setScale9Enabled(true);
		_buttonTcpListen->setTitleText("Listen");
		_buttonTcpListen->setContentSize(Size(100.0f, 25.0f));
		_buttonTcpListen->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonTcpListen);
		_buttonTcpListen->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTcpListen, this));

		_buttonTcpConnect = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonTcpConnect->setScale9Enabled(true);
		_buttonTcpConnect->setTitleText("Connect");
		_buttonTcpConnect->setContentSize(Size(100.0f, 25.0f));
		_buttonTcpConnect->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonTcpConnect);
		_buttonTcpConnect->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTcpConnect, this));

		//line 3. SCTP
		line += 1.0f;
		auto labelSctp = Label::createWithTTF("SCTP", FONT_NAME, FONT_SIZE);
		labelSctp->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		//labelSctp->setAnchorPoint(Vec2(1.0f,0.5f));
		scrollView->addChild(labelSctp);

		_buttonSctpListen = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonSctpListen->setScale9Enabled(true);
		_buttonSctpListen->setTitleText("Listen");
		_buttonSctpListen->setContentSize(Size(100.0f, 25.0f));
		_buttonSctpListen->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonSctpListen);
		_buttonSctpListen->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSctpListen, this));

		_buttonSctpConnect = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonSctpConnect->setScale9Enabled(true);
		_buttonSctpConnect->setTitleText("Connect");
		_buttonSctpConnect->setContentSize(Size(100.0f, 25.0f));
		_buttonSctpConnect->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonSctpConnect);
		_buttonSctpConnect->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSctpConnect, this));

		//line 4. Header compression
		line += 1.0f;
		auto labelHeaderCompression = Label::createWithTTF("Header compression", FONT_NAME, FONT_SIZE);
		labelHeaderCompression->setPosition(Vec2(12.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		labelHeaderCompression->setAnchorPoint(Vec2(0.0f, 0.5f));
		labelHeaderCompression->setColor(Color3B(255.0f, 255.0f, 0.0f));
		scrollView->addChild(labelHeaderCompression);

		//line 5. ROHC
		line += 1.0f;
		auto labelRohc = Label::createWithTTF("ROHC", FONT_NAME, FONT_SIZE);
		labelRohc->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		//labelRohc->setAnchorPoint(Vec2(1.0f,0.5f));
		scrollView->addChild(labelRohc);

		_checkboxRohc = CheckBox::create();
		_checkboxRohc->setScale(1.4f);
		_checkboxRohc->setTouchEnabled(true);
		_checkboxRohc->loadTextures("cocosui/check_box_normal.png",
									"cocosui/check_box_normal_press.png",
									"cocosui/check_box_active.png",
									"cocosui/check_box_normal_disable.png",
									"cocosui/check_box_active_disable.png");
		_checkboxRohc->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		_checkboxRohc->addEventListener(CC_CALLBACK_2(ControlPanelScene::selectedEventRohc,this));
		scrollView->addChild(_checkboxRohc);

		_buttonTestRohc = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonTestRohc->setScale9Enabled(true);
		_buttonTestRohc->setTitleText("Perform test");
		_buttonTestRohc->setContentSize(Size(100.0f, 25.0f));
		_buttonTestRohc->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonTestRohc);
		_buttonTestRohc->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTestRohc, this));
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		_buttonTestRohc->setEnabled(false);
#endif
		_checkboxRohc->setEnabled(false); //not implemented yet
		
		//line 6. Payload compression
		line += 1.0f;
		auto labelPayloadCompression = Label::createWithTTF("Payload compression", FONT_NAME, FONT_SIZE);
		labelPayloadCompression->setPosition(Vec2(12.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		labelPayloadCompression->setAnchorPoint(Vec2(0.0f, 0.5f));
		labelPayloadCompression->setColor(Color3B(255.0f, 255.0f, 0.0f));
		scrollView->addChild(labelPayloadCompression);

		//line 7. LZ4
		line += 1.0f;
		auto labelLz = Label::createWithTTF("LZ4", FONT_NAME, FONT_SIZE);
		labelLz->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		//labelLz->setAnchorPoint(Vec2(1.0f,0.5f));
		scrollView->addChild(labelLz);

		_checkboxLz = CheckBox::create("cocosui/check_box_normal.png",
									   "cocosui/check_box_normal_press.png",
									   "cocosui/check_box_active.png",
									   "cocosui/check_box_normal_disable.png",
									   "cocosui/check_box_active_disable.png");
		_checkboxLz->setScale(1.4f);
		_checkboxLz->setTouchEnabled(true);
		_checkboxLz->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		_checkboxLz->addEventListener(CC_CALLBACK_2(ControlPanelScene::selectedEventLz,this));
		scrollView->addChild(_checkboxLz);

		_buttonTestLz = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonTestLz->setScale9Enabled(true);
		_buttonTestLz->setTitleText("Perform test");
		_buttonTestLz->setContentSize(Size(100.0f, 25.0f));
		_buttonTestLz->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonTestLz);
		_buttonTestLz->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTestLz, this));

		//line 8. Huffman
		line += 1.0f;
		auto labelHuffman = Label::createWithTTF("Huffman", FONT_NAME, FONT_SIZE);
		labelHuffman->setPosition(Vec2(innerWidth / 2.0f - 110.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		//labelHuffman->setAnchorPoint(Vec2(1.0f,0.5f));
		scrollView->addChild(labelHuffman);

		_checkboxHuffman = CheckBox::create("cocosui/check_box_normal.png",
											"cocosui/check_box_normal_press.png",
											"cocosui/check_box_active.png",
											"cocosui/check_box_normal_disable.png",
											"cocosui/check_box_active_disable.png");
		_checkboxHuffman->setTouchEnabled(true);
		_checkboxHuffman->setScale(1.4f);
		_checkboxHuffman->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		_checkboxHuffman->addEventListener(CC_CALLBACK_2(ControlPanelScene::selectedEventHuffman,this));
		scrollView->addChild(_checkboxHuffman);

		_buttonTestHuffman = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		_buttonTestHuffman->setScale9Enabled(true);
		_buttonTestHuffman->setTitleText("Perform test");
		_buttonTestHuffman->setContentSize(Size(100.0f, 25.0f));
		_buttonTestHuffman->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(_buttonTestHuffman);
		_buttonTestHuffman->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonTestHuffman, this));

		//line 9. Send Packets
		line += 1.0f;
		auto labelSendPackets = Label::createWithTTF("Send packets", FONT_NAME, FONT_SIZE);
		labelSendPackets->setPosition(Vec2(12.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		labelSendPackets->setAnchorPoint(Vec2(0.0f, 0.5f));
		labelSendPackets->setColor(Color3B(255.0f, 255.0f, 0.0f));
		scrollView->addChild(labelSendPackets);

		//line 10. Button Packet line 1
		line += 1.0f;
		auto buttonPacketIntFew = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		buttonPacketIntFew->setScale9Enabled(true);
		buttonPacketIntFew->setTitleText("Few ints");
		buttonPacketIntFew->setContentSize(Size(100.0f, 25.0f));
		buttonPacketIntFew->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(buttonPacketIntFew);
		buttonPacketIntFew->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendFewInts, this));

		auto buttonPacketStringFew = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		buttonPacketStringFew->setScale9Enabled(true);
		buttonPacketStringFew->setTitleText("Few strings");
		buttonPacketStringFew->setContentSize(Size(100.0f, 25.0f));
		buttonPacketStringFew->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(buttonPacketStringFew);
		buttonPacketStringFew->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendFewStrings, this));

		//line 11. Button Packet line 2
		line += 1.0f;
		auto buttonPacketIntMany = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		buttonPacketIntMany->setScale9Enabled(true);
		buttonPacketIntMany->setTitleText("Many ints");
		buttonPacketIntMany->setContentSize(Size(100.0f, 25.0f));
		buttonPacketIntMany->setPosition(Vec2(innerWidth / 2.0f - 15.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(buttonPacketIntMany);
		buttonPacketIntMany->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendManyInts, this));

		auto buttonPacketStringMany = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
		buttonPacketStringMany->setScale9Enabled(true);
		buttonPacketStringMany->setTitleText("Many strings");
		buttonPacketStringMany->setContentSize(Size(100.0f, 25.0f));
		buttonPacketStringMany->setPosition(Vec2(innerWidth / 2.0f + 90.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		scrollView->addChild(buttonPacketStringMany);
		buttonPacketStringMany->addTouchEventListener(CC_CALLBACK_2(ControlPanelScene::onButtonSendManyStrigs, this));

		//line 12. TextField
		line += 1.0f;
		//_messages = { "chatas", "testas", "testas", "testas", "testas", "testas" };
		_textMessages = Text::create("", FONT_NAME, FONT_SIZE_MESSAGE);
		_textMessages->ignoreContentAdaptWithSize(false);
		_textMessages->setContentSize(Size(innerWidth - 4.0f, 300.0f));
		_textMessages->setTextHorizontalAlignment(TextHAlignment::LEFT);
		_textMessages->setPosition(Vec2(2.0f, scrollView->getInnerContainerSize().height - LINE_HEIGHT * line));
		_textMessages->setAnchorPoint(Vec2(0.0f, 1.0f));
		scrollView->addChild(_textMessages);

		//addMessage("Result area");

		auto host = myhost();
		if (host != NULL)
		{
			//addMessage(host);
			g_screenLog->log( LL_INFO, "Host: %s", host);
			auto hostIp = hostip(host);
			if (hostIp != NULL) g_screenLog->log( LL_INFO, "Host ip: %s", hostIp);//addMessage(hostIp);
		}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		auto ips = CTools::getIPAddresses();
		for(auto i : ips)
		{
			//addMessage(i);
			g_screenLog->log( LL_INFO, "\%s", i.c_str());
		}
#endif
	}
	catch (std::exception& e)
	{
		std::string ex = "Standard exception: ";
		ex += e.what();
		ex += "\n";
		//printf("%s", ex.c_str());
		g_screenLog->log( LL_ERROR, "%s", ex.c_str());
		return false;
	}
	return true;
}

void ControlPanelScene::editBoxReturn(cocos2d::extension::EditBox* editBox)
{
	const char* str = editBox->getText();

	std::stringstream ss(str);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(ss, segment, ':'))
	{
		seglist.push_back(segment);
	}
	if (seglist.size() < 2) return;
	auto trimStr = ss.str();
	_ipAddress = seglist[0];
	_port = std::stod(seglist[1]);

	//addMessage("New ip: " + _ipAddress + " port: " + doubleToString(_port));
	g_screenLog->log( LL_INFO, "New ip: %s:%s", _ipAddress.c_str(), doubleToString(_port).c_str());
}

void ControlPanelScene::onButtonTcpListen(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (!_isTcpServer)
		{
			dllInit();
			_tcpSocket = tcplisten(_port, 2, 1);

			if (socket <= 0)
			{
				//addMessage("Listening socket could not be opened on port " + doubleToString(_port));
				g_screenLog->log( LL_ERROR, "Listening socket could not be opened on port %s", doubleToString(_port).c_str());
				closesock(_tcpSocket);
				return;
			}
			else
			{
				setformat(_tcpSocket, 2, 0);
				setnagle(_tcpSocket, false);
				//setsync(_tcpSocket, 1);
				//addMessage("Server listening on port " + doubleToString(_port));
				g_screenLog->log( LL_INFO, "Server listening on port %s", doubleToString(_port).c_str());
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
		if (!_isTcpClient)
		{
			dllInit();
			_tcpSocket = tcpconnect(_ipAddress, _port, 1);

			if (_tcpSocket <= 0)
			{
				//addMessage("Failed to connect!");
				g_screenLog->log( LL_ERROR, "Failed to connect");
				closesock(_tcpSocket);
				return;
			}
			else
			{
				//setformat(_tcpSocket, 2, 0);
				setnagle(_tcpSocket, false);
				//setsync(_tcpSocket, 1);
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
			_isTcpClient = false;
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
		if (!_isSctpServer)
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
		if (!_isSctpClient)
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

void ControlPanelScene::onButtonTestRohc(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		//addMessage("Testing ROHC!");
		g_screenLog->log( LL_INFO, "Testing ROHC:");
		//RohcCompression::compress((char*)"payload data");
		RohcCompression::performTest();
#endif
	}
}

void ControlPanelScene::onButtonTestLz(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		g_screenLog->log( LL_INFO, "Testing LZ4:" );
		
		auto buffer = "Testing Lz4!Testing Lz4!ABCZXNCMBZXCHGUIQiuwiqwjebnbyzixcuzyxciuzyxcuiyzxuciyzuxciyTesting Lz4!";
		std::string cstr2 = Lz4Compression::encode(buffer);
		std::string dstr2 = Lz4Compression::decode(cstr2);

		g_screenLog->log( LL_DEBUG, "Dempressed string = %s", dstr2.c_str());
		g_screenLog->log( LL_DEBUG, "Compressed length = %d", cstr2.length());
		g_screenLog->log( LL_DEBUG, "Dempressed length = %d", dstr2.length());
		
		//addMessage(dstr2);
		//addMessage(doubleToString(cstr2.length()));
		//addMessage(doubleToString(dstr2.length()));
		//#endif
	}
}

void ControlPanelScene::onButtonTestHuffman(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		//addMessage("Testing Huffman!");
		g_screenLog->log( LL_INFO, "Testing Huffman:" );
		/*char *testBuffer = (char*)"Huff";
		auto result = HuffmanCompression::encode(testBuffer);
		//addMessage(result);
		auto result2 = HuffmanCompression::decode(result);
		addMessage(result2);*/
	}
}

void ControlPanelScene::onButtonSendFewInts(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED && (_isTcpClient || _isTcpServer))
	{
		if (_lz) PacketController::write(MSG_FEW_INT_LZ);
		else if (_huffman) PacketController::write(MSG_FEW_INT_HUFFMAN);
		else PacketController::write(MSG_FEW_INT);

		double /*sizet,*/ payloadSize;
		if (_isTcpClient)
		{
			sendmessage(_tcpSocket);
		}
		if (_isTcpServer)
		{
			for (auto i : _connections)
				{
					sendmessage(i.second);
				}
		}
		payloadSize = buffsize();
		g_screenLog->log( LL_INFO, "Sending packet with size: %s", doubleToString(payloadSize).c_str());
		//addMessage("Sending packet with size: " + doubleToString(payloadSize) + " " + doubleToString(sizet));
		//auto buff = getBuffer();
		//std::string s(buff->data);
		//addMessage("buff data: " + s);
		//addMessage("buff size: " + buff->BuffSize);
	}
}

void ControlPanelScene::onButtonSendFewStrings(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED && (_isTcpClient || _isTcpServer))
	{
		//addMessage("SendFewStrings");
		if (_lz) PacketController::write(MSG_FEW_STRING_LZ);
		else if (_huffman) PacketController::write(MSG_FEW_STRING_HUFFMAN);
		else PacketController::write(MSG_FEW_STRING);

		double payloadSize;
		if (_isTcpClient)
		{
			//sizet = 
			sendmessage(_tcpSocket);
		}
		if (_isTcpServer)
		{
			for (auto i : _connections)
				{
					//sizet = 
					sendmessage(i.second);
				}
		}
		payloadSize = buffsize();
		//addMessage("Sending packet with size: " + doubleToString(payloadSize));// +" " + doubleToString(sizet));
		g_screenLog->log( LL_INFO, "Sending packet with size: %s", doubleToString(payloadSize).c_str());
	}
}

void ControlPanelScene::onButtonSendManyInts(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		//addMessage("Sending many ints: 123, 456, 789 ...");
		
	}
}

void ControlPanelScene::onButtonSendManyStrigs(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED && (_isTcpClient || _isTcpServer))
	{
		//addMessage("SendManyStrings");
		if (_lz) PacketController::write(MSG_MANY_STRING_LZ);
		else if (_huffman) PacketController::write(MSG_MANY_STRING_HUFFMAN);
		else PacketController::write(MSG_MANY_STRING);

		double payloadSize;
		if (_isTcpClient)
		{
			sendmessage(_tcpSocket);
		}
		if (_isTcpServer)
		{
			for (auto i : _connections)
				{
					sendmessage(i.second);
				}
		}
		payloadSize = buffsize();
		//addMessage("Sending packet with size: " + doubleToString(payloadSize));
		g_screenLog->log( LL_INFO, "Sending packet with size: %s", doubleToString(payloadSize).c_str());
	}
}

void ControlPanelScene::selectedEventRohc(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_rohc = true;
		break;

	case CheckBox::EventType::UNSELECTED:
		_rohc = false;
		break;

	default:
		break;
	}
}

void ControlPanelScene::selectedEventLz(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_lz = true;
		break;

	case CheckBox::EventType::UNSELECTED:
		_lz = false;
		break;

	default:
		break;
	}
}

void ControlPanelScene::selectedEventHuffman(Ref* pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_huffman = true;
		break;

	case CheckBox::EventType::UNSELECTED:
		_huffman = false;
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
	_rohc = false;
	_lz = false;
	_huffman = false;
	auto sharedFileUtils = FileUtils::getInstance();
	const int settingsCount = 2; //ipAddress and port

	bool exist = sharedFileUtils->isFileExist(SETTINGS_FILENAME);
	if (!exist) return false;


	std::string str = sharedFileUtils->getStringFromFile(SETTINGS_FILENAME);

	if (str.empty()) return false;
	std::stringstream ss(str);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(ss, segment, '\n'))
	{
		seglist.push_back(segment);
	}
	if (seglist.size() < settingsCount) return false;

	_ipAddress = seglist[0];
	std::string::size_type pos = 0u;
	while ((pos = _ipAddress.find("\r", pos)) != std::string::npos)
	{
		_ipAddress.replace(pos, 2, "");
		pos += 0;
	}
	_port = std::stod(seglist[1]);

	return true;
}

void ControlPanelScene::addMessage(std::string msg)
{
	if (_messages.size() > MAX_MESSAGE_COUNT) _messages.erase(_messages.begin());
	_messages.push_back(msg);

	std::string fullMsg = "";
	for (auto i = _messages.size() - 1; i > 0; i--)
		{
			fullMsg += _messages[i] + "\n";
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
