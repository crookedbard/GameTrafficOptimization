#include "GameController.h"
#include "LoginLayer.h"
#include "LoadingLayer.h"
//#include "CharacterCreationLayer.h"
#include "CharacterSelectionLayer.h"
#include "GameLayer.h"
#include "HudLayer.h"
//#include "LoadingLayer.h"
#include "39dll.h"

#include "Players.h"
//#include "CharacterCreation.h" //need to remove later

USING_NS_CC;
using namespace std;

double GameController::serverSocket = -1;
bool GameController::connected = false;
bool GameController::loggedIn = false;
bool GameController::socketInitialized = false;
bool GameController::bufferEncrypt = false;
string GameController::bufferPassword = "";
int GameController::scene = kSceneNone;
Size GameController::visibleSize = CCSizeMake(0,0);
Point GameController::origin = Point(0,0);

thread _thread;
mutex _mutex;

LoginLayer *_loginLayer;
CharacterSelectionLayer *_characterSelectionLayer;
LoadingLayer *_loadingLayer;
GameLayer *_gameLayer;

CharacterInSlot slotData[3];

void GameController::shutdown(){

	try {
		if (connected){
			connected = false;
			if (_thread.joinable()) _thread.join();
			//_thread.join();
		}
	}
	catch (const std::system_error& /*e*/) {
		//std::cout << "Caught system_error with code " << e.code() << " meaning " << e.what() << '\n';
		CCLOG("system_error");
	}
	

	/*if (_loginLayer != nullptr) {
		((Layer *)_loginLayer)->release();
		_loginLayer = nullptr;
		CCLOG("_loginLayer->release();");
	}
	
	if (_characterSelectionLayer != nullptr) {
		((Layer *)_characterSelectionLayer)->release();
		_characterSelectionLayer = nullptr;
		CCLOG("_characterSelectionLayer->release();");
	}*/

	CC_SAFE_RELEASE(_loginLayer);
	CC_SAFE_RELEASE(_characterSelectionLayer);
	CC_SAFE_RELEASE(_loadingLayer);
}

void GameController::initialize(){
	/* initialize random seed: */
	srand(time(NULL));

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//scene = kNoneScene;

}

void GameController::gotoLogin(){

	/*if (_characterSelectionLayer != nullptr) {
		((Layer *)_characterSelectionLayer)->release();
		_characterSelectionLayer = nullptr;
		CCLOG("_characterSelectionLayer->release();");
	}*/
	CC_SAFE_RELEASE_NULL(_characterSelectionLayer);

	if (_loginLayer == nullptr) {
		_loginLayer = LoginLayer::create(); 
		_loginLayer->retain(); 
		//CCLOG("_loginLayer->retain();");
	}

	if (Director::getInstance()->getRunningScene() == NULL){
		//auto scene = LoginLayer::createScene(_loginLayer);
		//auto layer = scene->l neet to get the layer
		Director::getInstance()->runWithScene(LoginLayer::createScene(_loginLayer));
	}
	else{
		Director::getInstance()->replaceScene(LoginLayer::createScene(_loginLayer));
	}
	scene = kSceneLogin;

	
}

void GameController::gotoSplash(){
	//Director::getInstance()->replaceScene(GameLayer::createScene());
	scene = kSceneSplash;
}

void GameController::gotoLoading(){
	//push mapId
	//push characterId,name, character,gender,hairmodal,haircolor
	CCLOG("gotoLoading()");

	CC_SAFE_RELEASE_NULL(_characterSelectionLayer);

	if (_gameLayer == nullptr) {
		_gameLayer = GameLayer::create();
		_gameLayer->retain();
	}

	if (_loadingLayer == nullptr) {
		_loadingLayer = LoadingLayer::create();
		_loadingLayer->retain();
	}

	if (scene != kSceneLoading){
		Director::getInstance()->replaceScene(LoadingLayer::createScene(_loadingLayer));
		scene = kSceneLoading;
	}
	CCLOG("Before send MSG_MAPDATA");
	//CC_SAFE_RELEASE_NULL(_characterSelectionLayer);
	clearbuffer();
	writebyte(MSG_MAPDATA);
	sendmessage(serverSocket);
	CCLOG("After send MSG_MAPDATA");
}

void GameController::gotoCharacterSelection(){

	/*if (_loginLayer != nullptr) {
		((Layer *)_loginLayer)->release();
		_loginLayer = nullptr;
		CCLOG("_loginLayer->release();");
	}*/

	CC_SAFE_RELEASE_NULL(_loginLayer);

	if (_characterSelectionLayer == nullptr) {
		_characterSelectionLayer = CharacterSelectionLayer::create();
		_characterSelectionLayer->retain();
		//CCLOG("_characterSelectionLayer->retain();");
	}
	_characterSelectionLayer->setSlotData(slotData);

	if (scene != kSceneCharacterSelection){
		Director::getInstance()->replaceScene(CharacterSelectionLayer::createScene(_characterSelectionLayer));
		scene = kSceneCharacterSelection;
	}

	//_mutex.unlock();
}

void GameController::gotoCharacterCreation(){
	//Director::getInstance()->replaceScene(CharacterCreation::createScene()); //need to add layer
	scene = kSceneCharacterCreation;
}

void GameController::gotoGame(){
	//Director::getInstance()->replaceScene(GameLayer::createScene());
	_loadingLayer->setPercentage(1);
	CC_SAFE_RELEASE_NULL(_loadingLayer);
	//_loadingLayer->setPercentage(2);
	if (scene != kSceneGame){
		Director::getInstance()->replaceScene(GameLayer::createScene(_gameLayer));

		//auto hudLayer = HudLayer::create();
		//hudLayer->setName("HudLayer");
		//Director::getInstance()->getRunningScene()->addChild(hudLayer);
		//_loadingLayer->setPercentage(3);
		scene = kSceneGame;
	}
}

void GameController::tryConnecting(unsigned char from){

	if (!connected){

		if (!socketInitialized){
			dllInit();
			socketInitialized = true;
		}
		//byte asdasd = _loginLayer->_window;
		thread _connectionThread = thread([from](){

			//_mutex.lock();
			string ip = ipAddress;
			double portNo = port;
			//_mutex.unlock();
			
			double sock = tcpconnect(ip, portNo, 1);
			
			if (sock <= 0){
				//return; //failed to connect
				CCLOG("Failed to connect!");
				//_mutex.lock();
				//_loginLayer->bringWindow(kMessageLayer, "Failed to connect!",kLoginLayer);
				//_mutex.unlock();
				switch (from){
				case kLayerLogin:
					//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Failed to connect!", kLayerLogin));
					Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, "Failed to connect!"));
					//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(Dialog::showOrdinary, _loginLayer->getDialog(), "Failed to connect!"));//
					
					break;
				case kLayerCreateAccount:
					//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Failed to connect!", kLayerCreateAccount));
					Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, "Failed to connect!"));
					break;
				}
				

			}
			else{
				//CCLOG("Connected!");
				//_mutex.lock();
				serverSocket = sock;
				setnagle(serverSocket, false);
				connected = true;
				startListening();
				//_mutex.unlock();
				//Director::getInstance()->getScheduler()->performFunctionInCocosThread(&LoginLayer::sendLoginDetails, _loginLayer);
				
				switch (from){
				case kLayerLogin:
					Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::sendLoginData, _loginLayer));
					break;
				case kLayerCreateAccount:
					Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::sendRegistrationData, _loginLayer));
					break;
				}
				
			}
		});
		_connectionThread.detach();
	}
}

void GameController::startListening(){
	_thread = thread(GameController::packetListener);
	_thread.detach();
}

void GameController::packetListener(){

	_mutex.lock();
	bool con = connected;
	_mutex.unlock();

	double size;
	//CCLOG("serverSocket:  %f", serverSocket);
	//size = receivemessage(serverSocket);
	//CCLOG("SIZE>0:  %f", size);
	//return;
	while (con){
		//CCLOG("In thread!");

		size = receivemessage(serverSocket); //Each player in the login list has the following command(s) executed
		//CCLOG("size:  %f", size);
		//CCLOG("size: %f",size);
		if (size > 0)
		{
			//CCLOG("SIZE>0:  %f",size);
			//server::print("handle: ", game::c_blue);
			//CCLOG("packetHandler");
			//_mutex.lock();
			GameController::packetHandler(); //This client has data on the buffer
			//_mutex.unlock();
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

	//CCLOG("Disconnected!");
	//_mutex.lock();
	closesock(serverSocket);
	//_mutex.unlock();
}

void GameController::packetHandler(){

	//Handle incoming messages
	if (bufferEncrypt)
	{
	//Only decrypt if we're using an encryption password
	//bufferencrypt(bufferPassword);
	}

	unsigned char msg = readbyte();
	//CCLOG("Msg ID: %d", msg);
	//Message switches
	switch (msg)
	{
	case MSG_LOGIN:
		//CCLOG("MSG_LOGIN");
		{
			unsigned char errorCode = readbyte();
			switch (errorCode){
			case kErrorBadLogin:
				//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Wrong username or password!", kLayerLogin));
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, "Wrong username or password!"));
				break;
			case kErrorAlreadyLoggedIn:
				if (loggedIn){
					//end game go back to Login screen, someone is trying to steal acc.. or dual clienting..
					loggedIn = false;
					//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Someones already logged in \n with this username!", kLayerLogin));
					switch (scene){
					case kSceneCharacterSelection: Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(CharacterSelectionLayer::showDialogCallbackLogin, _characterSelectionLayer, "Someone is trying to log in \n with this username!"));
						break;
					case kSceneGame: //Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, ""Someone is trying to log in \n with this username!"));
						break;
					}
				}
				else{
					//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Someones already logged in \n with this username!", kLayerLogin));
					Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, "Someone already logged in \n with this username!"));
				}
				break;
			}

		}
		break;
	case MSG_REGISTER:
	{
		unsigned char response;

		try{
			response = readbyte();
		}
		catch (exception& /*e*/){
			//string str(e.what());
			CCLOG("ERROR");//server::print("Wrong packet - msg_changeScene: " + str, game::c_lightred);
			break;
		}

		switch (response){
		case kSuccess:
			//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Successfully registered!", kLayerMain));
			Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialogCallbackWindow, _loginLayer, "Successfully registered!", kLayerMain));
			break;
		case kErrorUsernameAlreadyExists:
			//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Username already exists!", kLayerCreateAccount));
			Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, "Username already exists!"));
			break;
		case kErrorEmailAlreadyInUse:
			//Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::bringWindow, _loginLayer, kLayerMessage, "Email address is already in use!", kLayerCreateAccount));
			Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(LoginLayer::showDialog, _loginLayer, "Email address is already in use!"));
			break;
		}
	}
		break;
	case MSG_CHARACTERSELECTION:
	{
		CCLOG("MSG_CHARACTERSELECTION");
		//_mutex.lock();
		
			int hasCharacters = readint();
			int slot = 0;

			for (int i = 0; i < 3; i++)
			{
				slotData[i].isEmpty = true;
				slotData[i].charID = -1;
				slotData[i].name = "";
				slotData[i].character = 0;
				slotData[i].gender = 0;
				slotData[i].hairModal = 0;
				slotData[i].hairColor = 0;
			}

			for (int i = 0; i < hasCharacters; i++)
			{
				slot = readint();
				slotData[slot].isEmpty = false;
				slotData[slot].charID = readint();
				slotData[slot].name = readstring();
				slotData[slot].character = readint();
				slotData[slot].gender = readint();
				slotData[slot].hairModal = readint();
				slotData[slot].hairColor = readint();
			}
		
		Director::getInstance()->getScheduler()->performFunctionInCocosThread(gotoCharacterSelection);
		break;
	}
	case MSG_CHANGESCENE:
	{
		unsigned char scene;
		try{
			scene = readbyte();
		}
		catch (exception& /*e*/){
			//CC_UNUSED_PARAM(e);
			//string str(e.what());
			CCLOG("ERROR");//server::print("Wrong packet - msg_changeScene: " + str, game::c_lightred);
			break;
		}

		switch (scene){
		case kSceneLogin:
			loggedIn = false;
			Director::getInstance()->getScheduler()->performFunctionInCocosThread(gotoLogin);
			break;
		case kSceneCharacterSelection:
			Director::getInstance()->getScheduler()->performFunctionInCocosThread(gotoCharacterSelection);
			break;
		case kSceneCharacterCreation:
			Director::getInstance()->getScheduler()->performFunctionInCocosThread(gotoCharacterCreation);
			break;
		}
	}
		break;
	case MSG_CREATECHARACTER:
		{
			unsigned char  response;
			int charID;
			try{
				response = readbyte();
				charID = readint();
			}
			catch (exception&){
				CCLOG("exception MSG_CREATECHARACTER");
				break;
			}

			switch (response){
			case kSuccess:
				//Create char
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(CharacterSelectionLayer::addCharacter, _characterSelectionLayer, charID));
				break;
			case kErrorCharacterNameTaken:
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(CharacterCreationLayer::showDialog, _characterSelectionLayer->getCharacterCreationLayer(), "Character name already taken!"));
				break;
			case kErrorCharacterSlotTaken:
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(CharacterCreationLayer::showDialog, _characterSelectionLayer->getCharacterCreationLayer(), "Character slot already taken!"));
				break;
			default: break;
			}
		}
		break;
	case MSG_DELETECHARACTER:
		{
			int response;
			try{
				response = static_cast<int>(readbyte());
			}
			catch (exception&){
				CCLOG("exception MSG_DELETECHARACTER");
				break;
			}

			switch (response){
			case kSuccess:
				//Delete char
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(CharacterSelectionLayer::deleteCharacter, _characterSelectionLayer));
				break;
			default: break;
			}
		}
		break;
	case MSG_ENTERGAME:
	{
			unsigned char response;
			try{
				response = readbyte();
			}
			catch (exception&){
				CCLOG("exception MSG_ENTERGAME");
				break;
			}
			CCLOG("Got MSG_ENTERGAME: %d",response);
			switch (response){
			case kSuccess:
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(gotoLoading);
				break;
			case kErrorCharacterDoesNotExist:
				Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(CharacterSelectionLayer::showDialog, _characterSelectionLayer, "Character does not exist!"));
				break;
			}
		
		break;
	}
	case MSG_MAPDATA:	
	{
		int mapID;
		int playerCount;
		int id;
		char* name;
		int race;
		int gender;
		int hairModal;
		int hairColor;
		unsigned char facing;
		int x;
		int y;
		try{
			mapID = readint();
			id = readint();
			name = readstring();  
			race = readint();
			gender = readint();
			hairModal = readint();
			hairColor = readint();
			x = readint();
			y = readint();
			facing = readbyte();
			playerCount = readint();
		}
		catch (exception&){
			CCLOG("exception MSG_MAPDATA part1");
			break;
		}
		
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([mapID, race, gender, facing, id, name, hairModal, hairColor,x,y]()
		{
			_gameLayer->loadMap(mapID);
			_loadingLayer->setPercentage(30);
			_gameLayer->setPlayer(player::create(static_cast<Race>(race), static_cast<Gender>(gender), static_cast<Facing>(facing)));
			_gameLayer->getPlayer()->initPlayer(id, name, hairModal, hairColor, x, y);
			_gameLayer->addChild(_gameLayer->getPlayer());
			_gameLayer->setViewPointPlrCenter();
			_loadingLayer->setPercentage(50);
		});

		for (int i = 0; i < playerCount; i++){
			try{
				_loadingLayer->setPercentage(11);
				id = readint();
				name = readstring();
				race = readint();
				gender = readint();
				hairModal = readint();
				hairColor = readint();
				x = readint();
				y = readint();
				facing = readbyte();
			}
			catch (exception&){
				CCLOG("exception MSG_MAPDATA part2");
				break;
			}

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([race, gender, facing, id, name, hairModal, hairColor, x, y]()
			{
				_gameLayer->addPlayer(static_cast<Race>(race), static_cast<Gender>(gender), static_cast<Facing>(facing), id, name, hairModal, hairColor, x, y);
			});

		}

		//Director::getInstance()->getScheduler()->performFunctionInCocosThread(gotoGame);
		//Director::getInstance()->getScheduler()->performFunctionInCocosThread(GameController::gotoGame);
		
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()
		{
			_loadingLayer->setPercentage(100);
			gotoGame();
		});

		break;
	}

	case MSG_PLAYER:
	{
		int id;
		char* name;
		int race;
		int gender;
		int hairModal;
		int hairColor;
		unsigned char facing;
		int x;
		int y;
		try{
			id = readint();
			name = readstring();
			race = readint();
			gender = readint();
			hairModal = readint();
			hairColor = readint();
			x = readint();
			y = readint();
			facing = readbyte();
		}
		catch (exception&){
			CCLOG("exception MSG_MAPDATA part1");
			break;
		}

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([race, gender, facing, id, name, hairModal, hairColor, x, y]()
		{
			_gameLayer->addPlayer(static_cast<Race>(race), static_cast<Gender>(gender), static_cast<Facing>(facing), id, name, hairModal, hairColor, x, y);
		});

		break;
	}

	case MSG_PLRMOVEDL:
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()
		{
			_gameLayer->getPlayer()->steps.push_back(kDirectionDownLeft);
		});
		break;

	case MSG_PLRMOVEDR:
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()
		{
			_gameLayer->getPlayer()->steps.push_back(kDirectionDownRight);
		});
		break;

	case MSG_PLRMOVEUL:
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()
		{
			_gameLayer->getPlayer()->steps.push_back(kDirectionUpLeft);
		});
		break;

	case MSG_PLRMOVEUR:
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()
		{
			_gameLayer->getPlayer()->steps.push_back(kDirectionUpRight);
		});
		break;

	case MSG_OPLRMOVEDL:
	{
		int id = readint();
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([id]()
		{
			players::findPlayerById(id)->steps.push_back(kDirectionDownLeft);
		});
		break;
	}
	case MSG_OPLRMOVEDR:
	{
		int id = readint();
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([id]()
		{
			players::findPlayerById(id)->steps.push_back(kDirectionDownRight);
		});
		break;
	}
	case MSG_OPLRMOVEUL:
	{
		int id = readint();
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([id]()
		{
			players::findPlayerById(id)->steps.push_back(kDirectionUpLeft);
		});
		break;
	}
	case MSG_OPLRMOVEUR:
	{	
		int id = readint();
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([id]()
		{
			players::findPlayerById(id)->steps.push_back(kDirectionUpRight);
		});
		break;
	}
	/*case MSG_PLRSTOP:
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()
		{
			_gameLayer->getHudLayer()->pressedDL = false;
			_gameLayer->getHudLayer()->pressedDR = false;
			_gameLayer->getHudLayer()->pressedUL = false;
			_gameLayer->getHudLayer()->pressedUR = false;
		});
		break;*/
	/*case MSG_OTHERPLAYER:
		CCLOG("MSG_OTHERPLAYER");
		break;
	case MSG_TEXT:
		CCLOG("MSG_TEXT");*/
		/*Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]
		{
		chat->addMsg(readstring());
		});*/
		break;
	default:
		CCLOG("DEFAULT!!!");
		clearbuffer();
		break;
	}

}

void GameController::testingReadstring(){

	/*_loadingLayer->setPercentage(1);

		int mapID2;
		int playerCount2;
		int id2;
		//char* name2;
		int race2;
		int gender2;
		int hairModal2;
		int hairColor2;
		unsigned char facing2;
		int x2;
		int y2;
	_loadingLayer->setPercentage(2);
		mapID2 = readint();
	_loadingLayer->setPercentage(3);
		id2 = readint();
		_loadingLayer->setPercentage(4);*/
		/*char* name2 = readstring();  //error
		_loadingLayer->setPercentage(5);
		race2 = readint();
		_loadingLayer->setPercentage(6);
		gender2 = readint();
		_loadingLayer->setPercentage(7);
		hairModal2 = readint();
		hairColor2 = readint();
		x2 = readint();
		y2 = readint();
		facing2 = readbyte();
		playerCount2 = readint();
		_loadingLayer->setPercentage(8);
		clearbuffer();
		_loadingLayer->setPercentage(9);*/

}
