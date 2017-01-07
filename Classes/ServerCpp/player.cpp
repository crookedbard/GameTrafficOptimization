#include "player.h"
#include "39dll.h"
#include "server.h"
#include "players.h"
#include "packets.h"
#include "maps.h"

#include "timer.h"

//Using namespace standard to resolve calls
using namespace std;


player::player(double sock, string ip_address)
{
	//New login player
	socket = sock;
	//UDPsocket = UDPsock;
	ipAddress = ip_address;
	playerID = server::newPlayerID();

	reset();
	//Get a new playerID if possible
	if(playerID == -1)
	{
		//Server is full and can't accept any incoming connections!
		clearbuffer();
		writebyte(MSG_SERVER);
		writestring("The server is full and cannot accept anymore incoming connections! Please try again at a later time. This server has been configured to handle a maximum of 255 players.");
		sendm(socket);
		server::print("New player connection rejected because server was full!");

		//Delete our instance now
		delete this;
	}
	else 
	{
		//Store important variables..
		players::list.push_back(this);

		//Print success message
		server::print("New player connection on logins spool [ID: "+server::doubleToString(playerID)+"]");
	}
}

player::~player()
{
	if (_scene == kSceneGame){
		try
		{
			server::pStmt->SqlStatement("UPDATE characters set x=" + to_string(_x) + ", y=" + to_string(_y) + ",facing=" + to_string(_facing) + " \
										WHERE charId=" + to_string(_characterID) +";"); 
		}
		catch (Kompex::SQLiteException &exception)
		{
			server::print("Exception Occured ", game::c_lightred);
			exception.Show();
			server::print("SQLite result code: " + to_string(exception.GetSqliteResultCode()), game::c_lightred);
			return;
		}
	}
	//Destructor
	server::print("Player has left the server");
	closesock(socket);
	//Remove from lists
	for (unsigned int i=0; i<players::list.size(); i++)
	{
		if(players::list[i]==this)
		{
			players::list.erase(players::list.begin()+i);
		}
	}

	//Free up player id
	server::playerID[playerID] = false;
}

/*
	Add new networking messages below here. Be sure to declare the message
	header as a constant in server.h. Don't forget to pass the this argument
	to pass a memory reference to the current player object so you can edit
	variables from the player
*/

void player::reset(){
	_userID = -1;
	_username = "";
	_password = "";
	_loggedIn = false;
	_scene = kSceneLogin;

	_characterID = -1;
	//_slot = -1;
	_name = "";
	_character = kRaceHumanWhite;
	_gender = -1;
	_hairModal = -1;
	_hairColor = -1;

	_mapID = -1;
	_x = -1;
	_y = -1;
	_facing = kFacingDownLeft;

	_state = kStateIdle;

	_previousMoveTime = Timer::GetTime();
	_currentMoveTime = Timer::GetTime();
	_deltaMoveTime = 0;
	

	pressedDL = false;
	pressedDR = false;
	pressedUL = false;
	pressedUR = false;
}

void player::act()
{
	if (_scene != kSceneGame)return;

	switch (_state){
	case kStateIdle: checkKeys();
		break;
	case kStateMove: //needs timer
		//cout << Timer::GetTime() << " - " << _previousStepTime << " = " << Timer::GetTime() - _previousStepTime<<endl;
		_currentMoveTime = Timer::GetTime();
		if (_currentMoveTime - _previousMoveTime >= static_cast<double>(0.48) - _deltaMoveTime){
			//_previousMoveTime = _currentMoveTime - (_currentMoveTime - _previousMoveTime - static_cast<double>(0.48)); 
			_deltaMoveTime = (_currentMoveTime - _previousMoveTime - static_cast<double>(0.48));
			/*"_currentMoveTime: " << _currentMoveTime << "_previousMoveTime: " << _previousMoveTime <<*/
			//if (_deltaMoveTime != 0){ cout << "_deltaMoveTime: " << _deltaMoveTime << endl; }
			//cout <<"1.  " <<_previousMoveTime << endl;
			_state = kStateIdle; 
		}
		break;
	}
}

void player::checkKeys(){

	if ((pressedDL && pressedUR) || (pressedUL && pressedDR))return; 
	if (pressedUR && pressedDR) { if (_y > 0 && Maps::findMap(_mapID)->tile[_x][_y-1].walkable){ _y--; sendMovePacket(kDirectionUpRight); } return; }
	if (pressedUL && pressedUR) { if (_x > 0 && Maps::findMap(_mapID)->tile[_x - 1][_y].walkable){ _x--; sendMovePacket(kDirectionUpLeft); } return; }
	if (pressedDL && pressedUL) { if (_y < Maps::list[_mapID]->getHeight() - 1 && Maps::findMap(_mapID)->tile[_x][_y + 1].walkable){ _y++; sendMovePacket(kDirectionDownLeft); } return; }
	if (pressedDR && pressedDL) { if (_x < Maps::list[_mapID]->getWidth() - 1 && Maps::findMap(_mapID)->tile[_x + 1][_y].walkable){ _x++; sendMovePacket(kDirectionDownRight); } return; }

	if (pressedDL)
	{
		if (_y < Maps::list[_mapID]->getHeight() - 1 /* && Maps::findMap(_mapID)->tile[_x][_y + 1].walkable*/)
		{
			if (Maps::list[_mapID]->tile[_x][_y + 1].walkable)
			{
				_y++; 
				sendMovePacket(kDirectionDownLeft);
			}
		} return;
	}
	if (pressedDR){ if (_x < Maps::list[_mapID]->getWidth() - 1 && Maps::findMap(_mapID)->tile[_x + 1][_y].walkable){ _x++; sendMovePacket(kDirectionDownRight); } return; }
	if (pressedUL){ if (_x > 0 && Maps::findMap(_mapID)->tile[_x - 1][_y].walkable){ _x--; sendMovePacket(kDirectionUpLeft); } return; }
	if (pressedUR)
	{
		if (_y > 0 && Maps::findMap(_mapID)->tile[_x][_y - 1].walkable)
		{
			_y--; sendMovePacket(kDirectionUpRight);
		} 
		return;
	}
	//cout << _x << " - " << _y << endl;
}

void player::sendMovePacket(Direction direction){
	//cout << direction << endl;
	_state = kStateMove;

	clearbuffer();
	switch (direction){
	case kDirectionDownLeft: _facing = kFacingDownLeft; writebyte(MSG_PLRMOVEDL);
		break;
	case kDirectionDownRight: _facing = kFacingDownRight; writebyte(MSG_PLRMOVEDR);
		break;
	case kDirectionUpLeft: _facing = kFacingUpLeft; writebyte(MSG_PLRMOVEUL);
		break;
	case kDirectionUpRight: _facing = kFacingUpRight; writebyte(MSG_PLRMOVEUR);
		break;
	}
	sendm(socket);

	_previousMoveTime = Timer::GetTime();
	
	clearbuffer();
	switch (direction){
	case kDirectionDownLeft: _facing = kFacingDownLeft; writebyte(MSG_OPLRMOVEDL);
		break;
	case kDirectionDownRight: _facing = kFacingDownRight; writebyte(MSG_OPLRMOVEDR);
		break;
	case kDirectionUpLeft: _facing = kFacingUpLeft; writebyte(MSG_OPLRMOVEUL);
		break;
	case kDirectionUpRight: _facing = kFacingUpRight; writebyte(MSG_OPLRMOVEUR);
		break;
	}
	writeint(_characterID);
	sendm(socket,3,this);
}

void player::handle()
{

	//Handle incoming messages
	if(server::bufferEncrypt)
	{
		//Only decrypt if we're using an encryption password
		bufferencrypt(server::bufferPassword);
	}

	int messageID = static_cast<int>(readbyte());
	//cout<<"Msg ID: "<< messageID << endl;

	//Message switches
	switch(messageID)
	{
		case MSG_LOGIN:
			if (_scene != kSceneLogin)  break;
			packets::msg_login(this);
		break;

		case MSG_REGISTER:
			if (_scene != kSceneLogin)  break;
			packets::msg_register(this);
		break;

		case MSG_CHANGESCENE:
			packets::msg_changeScene(this);
			break;

		case MSG_RETURNTOLOGIN:
			if (_scene != kSceneCharacterSelection)  break;
			packets::msg_returnToLogin(this);
			break;

		case MSG_CREATECHARACTER:
			if (_scene != kSceneCharacterSelection)  break;
			packets::msg_createCharacter(this);
			break;

		case MSG_DELETECHARACTER:
			if (_scene != kSceneCharacterSelection)  break;
			packets::msg_deleteCharacter(this);
			break;

		case MSG_ENTERGAME:
			if (_scene != kSceneCharacterSelection)  break;
			packets::msg_enterGame(this);
			break;

		case MSG_MAPDATA:
			if (_scene != kSceneLoading)  break;
			packets::msg_mapData(this);
			break;

		case MSG_PLRMOVEDL:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrMoveDL(this);
			pressedDL = true;
			//cout << "MOVE DL" << endl;
			break;

		case MSG_PLRMOVEDR:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrMoveDR(this);
			pressedDR = true;
			break;

		case MSG_PLRMOVEUL:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrMoveUL(this);
			pressedUL = true;
			break;

		case MSG_PLRMOVEUR:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrMoveUR(this);
			pressedUR = true;
			break;

		case MSG_PLRSTOPDL:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrStopDL(this);
			pressedDL = false;
			//cout << "STOP DL" << endl;
			break;

		case MSG_PLRSTOPDR:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrStopDR(this);
			pressedDR = false;
			break;

		case MSG_PLRSTOPUL:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrStopUL(this);
			pressedUL = false;
			break;

		case MSG_PLRSTOPUR:
			if (_scene != kSceneGame)  break;
			//packets::msg_plrStopUR(this);
			pressedUR = false;
			break;
		/*case MSG_PLRSTOP:
			if (_scene != kSceneGame)  break;
			packets::msg_plrStop(this);
			break;*/

		default:
			cout << "UNKNOWN PACKET:" << endl;
			cout << "Player: id - " << _characterID << ", Name - " << _name << ",msgId - " << messageID << endl;
			//packets::msg_default(this);
		break;
	}

	//if (_scene==kSceneGame) act();//for test
}