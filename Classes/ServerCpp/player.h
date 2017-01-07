#ifndef __PLAYER_H__
#define __PLAYER_H__

//Additional include dependancies
#include <iostream>
#include "game.h"
//#include <string>

enum Scene {
	kSceneNone,
	kSceneSplash,
	kSceneLoading,
	kSceneLogin,
	kSceneCharacterSelection,
	kSceneCharacterCreation,
	kSceneGame
};

enum Race {
	kRaceHumanWhite,
	kRaceHumanBlack,
	kRaceHumanYellow,
	kRaceHumanGreen,
	kRaceSkeleton,
	kRacePanda,
	kRaceFish
};

/*enum Facing{
	kFacingDownLeft,
	kFacingDownRight,
	kFacingUpLeft,
	kFacingUpRight
};*/

class player
{
	public:

		double socket;		
		std::string ipAddress;
		int playerID;
		//bool loggedIn;
		//std::string username;
		//std::string password;
		bool pressedDL;
		bool pressedDR;
		bool pressedUL;
		bool pressedUR;

		player(double sock, std::string ip_address);
		~player();

		void handle();
		void reset();
		void act();
		void checkKeys();
		void sendMovePacket(Direction direction);
		/*public:
        double Sock;
        bool InUse;
        std::string name;
        int x;
        int y;
		int direction;*/
		CC_SYNTHESIZE(bool, _loggedIn, LoggedIn);
		//CC_SYNTHESIZE(bool, _inGame, InGame);
		CC_SYNTHESIZE(int, _scene, Scene);
		CC_SYNTHESIZE(int, _userID, UserID);
		CC_SYNTHESIZE(std::string, _username, Username);
		CC_SYNTHESIZE(std::string, _password, Password);

		CC_SYNTHESIZE(int, _characterID, CharacterID);
		//CC_SYNTHESIZE(int, _slot, Slot);
		CC_SYNTHESIZE(std::string, _name, Name);
		CC_SYNTHESIZE(int, _character, Character);
		CC_SYNTHESIZE(int, _gender, Gender);
		CC_SYNTHESIZE(int, _hairModal, HairModal);
		CC_SYNTHESIZE(int, _hairColor, HairColor);
		CC_SYNTHESIZE(int, _mapID, MapID);
		CC_SYNTHESIZE(int, _x, X);
		CC_SYNTHESIZE(int, _y, Y);
		CC_SYNTHESIZE(Facing, _facing, Facing);
		
		CC_SYNTHESIZE(State, _state, State);
private:
	double _previousMoveTime;
	double _deltaMoveTime;
	double _currentMoveTime;
};

#endif // __PLAYER_H__