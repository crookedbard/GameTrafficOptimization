#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

//Message constants
#define MSG_SERVER 0 //unused
#define MSG_LOGIN 1
#define MSG_REGISTER 2
#define MSG_CHANGESCENE 3 //request change scene (mainly when pressed esc key)
#define MSG_PLAYER 4
#define MSG_CHARACTERSELECTION 5
#define MSG_RETURNTOLOGIN 6
#define MSG_CREATECHARACTER 7
#define MSG_DELETECHARACTER 8
#define MSG_ENTERGAME 9
#define MSG_MAPDATA 10
#define MSG_PLRMOVEDL 11
#define MSG_PLRMOVEDR 12
#define MSG_PLRMOVEUL 13
#define MSG_PLRMOVEUR 14
#define MSG_PLRSTOPDL 15
#define MSG_PLRSTOPDR 16
#define MSG_PLRSTOPUL 17
#define MSG_PLRSTOPUR 18
#define MSG_OPLRMOVEDL 19
#define MSG_OPLRMOVEDR 20
#define MSG_OPLRMOVEUL 21
#define MSG_OPLRMOVEUR 22
#define MSG_MOBMOVEDL 23
#define MSG_MOBMOVEDR 24
#define MSG_MOBMOVEUL 25
#define MSG_MOBMOVEUR 26

//#define MSG_TEXT 50 //for test

#include "cocos2d.h"
#include "GameConfig.h"
#include "Utils.h"
#include "VisibleRect.h"
#include "Dialog.h"
//#include "Timer.h"

enum {
	kSceneNone,
	kSceneSplash,
	kSceneLoading,
	kSceneLogin,
	kSceneCharacterSelection,
	kSceneCharacterCreation,
	kSceneGame
};

enum Errors{
	kSuccess,
	kErrorBadLogin,
	kErrorAlreadyLoggedIn,
	kErrorUsernameAlreadyExists,
	kErrorEmailAlreadyInUse,
	kErrorCharacterNameTaken,
	kErrorCharacterSlotTaken,
	kErrorCharacterDoesNotExist
};

enum {
	kCharHumanWhite,
	kCharHumanBlack,
	kCharHumanYellow,
	kCharHumanGreen,
	kCharSkeleton,
	kCharPanda,
	kCharFish
};

class GameController
{
public:

	static void initialize();
	static cocos2d::Size visibleSize;
	static cocos2d::Point origin;
	//static cocos2d::Layer *_loginLayer;
	//static cocos2d::Layer *_characterSelectionLayer;
	//static std::string ipAddress;
	//static const double port = 1337;
	static double serverSocket;
	static bool bufferEncrypt;
	static std::string bufferPassword;
	static bool socketInitialized;
	static bool connected;
	static bool loggedIn;
	static int scene;
	
	static void gotoSplash();
	static void gotoLoading();
	static void gotoLogin(); 
	static void gotoCharacterSelection(); //loading - get data from server, existing chars
	static void gotoCharacterCreation(); 
	static void gotoGame(); //loading - get map data
	static void changeMap(); //loading - get map data
	static void shutdown();

	static void tryConnecting(unsigned char from);
	static void startListening();
	static void packetListener();
	static void packetHandler();

	static void testingReadstring();
private:
	
};

#endif // __GAMECONTROLLER_H__