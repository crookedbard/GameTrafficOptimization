#include "HUDLayerOld.h"

bool HUDLayer::init()
{
	bool bRet = false;
	connected=false;

	dllInit();

    do 
    {
        CC_BREAK_IF(! Layer::init());
        this->setTouchEnabled(true);

		auto scoreLabel = LabelTTF::create("Score 12345", "Arial", 24);
        CC_BREAK_IF(! scoreLabel);
		auto timeLabel = LabelTTF::create("Time 00:00", "Arial", 24);
        CC_BREAK_IF(! timeLabel);
    
		auto winSize = Director::getInstance()->getWinSize();
		visibleSize = Director::getInstance()->getVisibleSize();
		origin = Director::getInstance()->getVisibleOrigin();

		scoreLabel->setPosition(Point(scoreLabel->getContentSize().width/2, 
			winSize.height - scoreLabel->getContentSize().height/2));
		timeLabel->setPosition(Point(winSize.width-scoreLabel->getContentSize().width/2, 
			winSize.height - scoreLabel->getContentSize().height/2));

		auto closebtn = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(HUDLayer::OnMenuClose));

		//Vector<Sprite*> wifiVec(2);

		//wifiVec.pushBack(Sprite::create("HUD/btn_wifi_off.png"));
		//wifiVec.pushBack(Sprite::create("HUD/btn_wifi_on.png"));

		//auto toggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HUDLayer::OnMenuWifi, this), Sprite::create("HUD/btn_wifi_off.png"),Sprite::create("HUD/btn_wifi_off.png"),NULL);

		// MenuItemImage 생성
		auto image1 = MenuItemImage::create("HUD/btn_wifi_off.png", "HUD/btn_wifi_on.png", NULL, NULL );
		auto image2 = MenuItemImage::create("HUD/btn_wifi_on.png", "HUD/btn_wifi_off.png", NULL, NULL );

		// MenuItemImage를 사용해 MenuItemToggle 생성
		auto toggle_wifi = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HUDLayer::OnMenuWifi, this), image1, image2, NULL);

		//auto wifibtn = MenuItemImage::create("HUD/btn_wifi_off.png", "HUD/btn_wifi_on.png", this, menu_selector(HUDLayer::OnMenuWifi));
		auto closebtnSize = closebtn->getContentSize();
		auto wifibtnSize = toggle_wifi->getContentSize();
		//btn_close->setPosition( Point(winSize.width/2-closebtnSize.width/2, winSize.height - closebtnSize.height/2) );
		//btn_wifi->setPosition( Point(winSize.width/2-closebtnSize.width/2, winSize.height - closebtnSize.height/2-100) );
		toggle_wifi->setPosition(Point(origin.x + visibleSize.width - wifibtnSize.width/2 ,
										origin.y + wifibtnSize.height/2));


		auto closebtnMenu = Menu::create(closebtn, NULL);
		auto wifibtnMenu = Menu::create(toggle_wifi, NULL);

		wifibtnMenu->setPosition(Vec2::ZERO);

		closebtnMenu->setPosition( Point(winSize.width/2-closebtnSize.width/2, winSize.height - closebtnSize.height/2));

		// add this to the layer
		this->addChild(scoreLabel, 1);
		this->addChild(timeLabel, 1);
		this->addChild(closebtnMenu, 2);
		this->addChild(wifibtnMenu, 2);
		


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		auto chatSpr = Sprite::create("woodentable.png");
#else
		auto chatSpr = Sprite::create("woodentable2.png");
#endif
		vector<string> messages_all = { "chatas" };
		chat = new ChatTableView(chatSpr, 160, 16, messages_all);
		//chat->setAnchorPoint(Point(0, 0));
		//chat->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 ));
		//chat->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		//this->addChild(chat);
		//this->addChild(chat);
		chatSpr->setAnchorPoint(Point(0, 0));

		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		chatSpr->setPosition(Point(0, 0));
#else
		chatSpr->setPosition(Point(0, chatSpr->getContentSize().height));
#endif
		this->addChild(chatSpr);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		EditBox *m_pEditName = EditBox::create(Size(100, 30), Scale9Sprite::create("EditBoxTemplate2.png"));
#else
		EditBox *m_pEditName = EditBox::create(Size(400, 30), Scale9Sprite::create("EditBoxTemplate2.png"));
#endif
		m_pEditName->setAnchorPoint(Point(0, 0));
		m_pEditName->setPosition(Point(origin.x + visibleSize.width / 2 - m_pEditName->getContentSize().width / 2, origin.y /*+ visibleSize.height*/ + m_pEditName->getContentSize().height));
		m_pEditName->setFontColor(ccWHITE);
		m_pEditName->setPlaceHolder("Tekstas:");
		m_pEditName->setMaxLength(256);
		m_pEditName->setReturnType(EditBox::KeyboardReturnType::DONE);
		m_pEditName->setDelegate(this);
		//m_pEditName->setOnEnterCallback(&HUDLayer::editBoxReturn);
		this->addChild(m_pEditName);
		/*TableView* pTableView = TableView::create(this, VisibleRect::getVisibleRect().size, NULL);//VisibleRect::getVisibleRect().size
		pTableView->setDirection(ScrollView::Direction::VERTICAL);

		pTableView->setDelegate(this);
		pTableView->setPosition(Point(chatSpr->getContentSize().width*0.04, chatSpr->getContentSize().height*0.14));
		
		pTableView->setViewSize(Size(chatSpr->getContentSize().width*0.94, chatSpr->getContentSize().height*0.80));
		pTableView->setContentSize(Size(chatSpr->getContentSize().width*0.94, chatSpr->getContentSize().height*0.80));
		this->addChild(pTableView);
		pTableView->reloadData();*/

		/*TableView* tableView = TableView::create(this, VisibleRect::getVisibleRect().size, NULL);
		tableView->setDirection(ScrollView::Direction::VERTICAL);
		tableView->setPosition(Vec2(0, winSize.height / 2 - 120));
		tableView->setDelegate(this);
		tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		this->addChild(tableView,1,1);
		tableView->reloadData();*/

        bRet = true;
    } while (0);

    return bRet;
}

void HUDLayer::editBoxReturn(EditBox* editBox)
{

	string text = editBox->getText();

	if (connected)
	{
		//char *txtChar = new char[text.size() + 1];
		//txtChar[text.size()] = 0;
		//memcpy(txtChar, text.c_str(), text.size());
		
		clearbuffer();
		writebyte(50);
		writestring(text);
		sendmessage(serverSocket);
	}

	//messages_all.push_back(text);
	

}


void HUDLayer::OnMenuClose(Ref* pSender)
{
	Scene* nextScene = MainMenu::createScene();
	Director::getInstance()->replaceScene(nextScene);	
}

void HUDLayer::OnMenuWifi(Ref* pSender)
{
	//auto delay = DelayTime::create(2.5f);
	/*FadeOut* fadeOutAction = FadeOut::create(2.5f);
	RemoveSelf* removeSelfAction = RemoveSelf::create();
	auto sequence = Sequence::create(fadeOutAction,removeSelfAction,NULL);
	auto disconnectedLabel = LabelTTF::create("Disconnected", "Arial", 24);
	auto connectedLabel = LabelTTF::create("Connected", "Arial", 24);*/

	auto toggleItem = dynamic_cast<MenuItemToggle*>(pSender);
    if (toggleItem->getSelectedIndex() == 0 ) {
		toggleItem->setSelectedIndex(1);
        // Turn Off Wifi 
		if (connected)
		{
			//CCLOG("Disconnected!");
			//closesock(serverSocket);
			//sockexit();
			//dllFree();
			connected = false;
			//t1.join();
			_thread.join();
			displayMsg("Disconnected!");
			toggleItem->setSelectedIndex(0);
		}
			
    }
    else if ( toggleItem->getSelectedIndex() == 1 ) {
		toggleItem->setSelectedIndex(0);
        // Turn On Wifi
		//TcpConnection::beginListening();
		/////////////////////
	// Define the protocol packet
		if (!connected)
		{
			serverSocket = tcpconnect("127.0.0.1", 1337, 1);//78.57.167.11//192.168.1.193//127.0.0.1//78.56.33.195
			CCLOG("Server socket: %f",serverSocket);
			if (serverSocket <= 0){
				return;
			}
			else{
				CCLOG("Connected!");
				setnagle(serverSocket, false);
				connected = true;
				displayMsg("Connected!");
				toggleItem->setSelectedIndex(1);

				clearbuffer();
				writebyte(1); //login
				writebyte(HelloWorld::_character);

				//string str = HelloWorld::_name;
				//char *txtChar = new char[str.size() + 1];
				//txtChar[str.size()] = 0;
				//memcpy(txtChar, str.c_str(), str.size());

				writestring(HelloWorld::_name);
				sendmessage(serverSocket);

				//thread t1(&HUDLayer::packetListener, this);
				//thread t1(&HUDLayer::packetListener, this);
				_thread = thread(&HUDLayer::packetListener,this);
				//t1.detach();
				//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_2(HUDLayer::packetListener, this));
			}
		}
		
		
    }

	/*switch(connected)
	{
		case 1:
			
			/*CC_BREAK_IF(! disconnectedLabel);
			disconnectedLabel->setPosition(Point(origin.x + visibleSize.width - disconnectedLabel->getContentSize().width/2, 
													origin.y + disconnectedLabel->getContentSize().height/2));
			
			disconnectedLabel->runAction(sequence);
			this->addChild(disconnectedLabel, 1);*/
		/*break;
		case 0:
			
			/*CC_BREAK_IF(! connectedLabel);
			connectedLabel->setPosition(Point(origin.x + visibleSize.width - connectedLabel->getContentSize().width/2, 
												origin.y + connectedLabel->getContentSize().height/2));

			connectedLabel->runAction(sequence);
			this->addChild(connectedLabel, 1);*/
		/*break;
	}*/

}

void HUDLayer::packetListener(){

	_mutex.lock();
	bool con = connected;
	_mutex.unlock();


	while (con){
		//CCLOG("In thread!");

		double size;

		size = receivemessage(serverSocket); //Each player in the login list has the following command(s) executed
		//CCLOG("size: %f",size);
		if (size > 0)
		{
			//server::print("handle: ", game::c_blue);
			CCLOG("packetHandler");
			_mutex.lock();
			//HUDLayer::packetHandler(); //This client has data on the buffer
			_mutex.unlock();
		}
		else if (size == 0)
		{
			CCLOG("Connection to server ended, exiting thread.");
			connected = false;
			break;
		}

		_mutex.lock();
		con = connected;
		_mutex.unlock();

		Sleep(1);
	}

	_mutex.lock();
	//CCLOG("Disconnected!");
	closesock(serverSocket);
	_mutex.unlock();
}
/*
void HUDLayer::packetHandler(){



	CCLOG("inside packetHandler");
	int messageID = static_cast<int>(readbyte());
	//double messageID = (readbyte());
	CCLOG("Msg ID: %d",messageID);
	//Message switches
	switch (messageID)
	{
	case MSG_TEXT:
		CCLOG("MSG_TEXT");
		
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]
		{
			//HUDLayer::displayMsg(readstring());
			chat->addMsg(readstring());
		});
		break;

	case MSG_LOGIN:
		//Login message
		//packets::msg_login(this);
		CCLOG("MSG_LOGIN");
		break;

	case MSG_OTHERPLAYER:
		//Login message
		//packets::msg_login(this);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		int _character = readbyte();
		string _name = readstring();
#endif
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]
		{
			//HUDLayer::displayMsg(readstring());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			HelloWorld::createOtherPlayer(_character, _name);
#else
			int _character = readbyte();
			string _name = readstring();
			HelloWorld::createOtherPlayer(_character, _name);
#endif
		});
		CCLOG("MSG_OTHERPLAYER");
		break;


	}

}*/

void HUDLayer::displayMsg(string msg){
	FadeOut* fadeOutAction = FadeOut::create(4.5f);
	RemoveSelf* removeSelfAction = RemoveSelf::create();
	auto sequence = Sequence::create(fadeOutAction, removeSelfAction, NULL);
	auto label = LabelTTF::create(msg, "Arial", 24);

	label->setPosition(Point(origin.x + visibleSize.width - label->getContentSize().width / 2,
		origin.y + label->getContentSize().height / 2 + 30));

	label->runAction(sequence);
	this->addChild(label, 1);
}
//thread t1(packetListener);

/*void HUDLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size HUDLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	//if (idx == 2) {
	//	return Size(100, 100);
	//
	return Size(160, 20);
}

TableViewCell* HUDLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	auto string = String::createWithFormat("%ld", idx);
	TableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = TableViewCell::create();//new CustomTableViewCell();
		//cell->autorelease();
		auto sprite = Sprite::create("chatline.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		cell->addChild(sprite);

		auto label = Label::createWithSystemFont(string->getCString(), "Helvetica", 18.0);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		//label->setColor(ccRED);
		cell->addChild(label);
	}
	else
	{
		auto label = (Label*)cell->getChildByTag(123);
		label->setString(string->getCString());
	}


	return cell;
}

ssize_t HUDLayer::numberOfCellsInTableView(TableView *table)
{
	return 20;
}*/