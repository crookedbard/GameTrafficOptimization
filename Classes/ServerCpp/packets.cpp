#include "packets.h"
#include <sstream>
#include "39dll.h"
#include "players.h"

using namespace std;

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

void packets::msg_login(player *plr)
{
	string username;
	string password;
	
	try{
		username = readstring();
		password = readstring();
	}
	catch (exception& e){
		string str(e.what());
		server::print("Wrong packet - msg_login: " + str, game::c_lightred);
		return;
	}

	if (players::findPlayer(username) != nullptr){
		if (players::findPlayer(username)->getLoggedIn()){
			//player already loggedIn
			clearbuffer();
			writebyte(MSG_LOGIN);
			writebyte(kErrorAlreadyLoggedIn); //login error code 1 - player already loggedIn
			sendm(plr->socket);
			//also send message to leggedIn player
			//cout << "kErrorAlreadyLoggedIn" << endl;
			clearbuffer();
			writebyte(MSG_LOGIN);
			writebyte(kErrorAlreadyLoggedIn); //login error code 1 - player already loggedIn
			sendm(players::findPlayer(username)->socket);
			//cout << "kErrorAlreadyLoggedIn 2" << endl;
			server::print("Player logged out: " + username, game::c_lightred);
			players::findPlayer(username)->reset();
			return;
		}
	}

	if (server::pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM users \
											  WHERE username = '" + username + "' \
											  and password='" + password + "';") > 0){
		server::print("Player logged in: " + username, game::c_lightgreen);

		server::pStmt->Sql("SELECT userId FROM users \
						   	WHERE username = '" + username + "' \
							and password='" + password + "';");
		// process all results
		while (server::pStmt->FetchRow())
		{
			plr->setUserID(server::pStmt->GetColumnInt(0));
		}
		// do not forget to clean-up
		server::pStmt->FreeQuery();

		plr->setLoggedIn(true);
		plr->setUsername(username);
		plr->setPassword(password);
		plr->setScene(kSceneCharacterSelection);
		//cout << "ready to send MSG_CHARACTERSELECTION" << endl;
		//return;
		clearbuffer();
		writebyte(MSG_CHARACTERSELECTION); //Move to character selection screen

		int characterCount = static_cast<int>(server::pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM characters \
																   	WHERE userId = " + to_string(plr->getUserID()) + ";"));
		writeint(characterCount); //how many character he has
		server::pStmt->Sql("SELECT slot, charId, name, character, gender, hairModal, hairColor FROM characters \
						   	WHERE userId = " + to_string(plr->getUserID()) + ";");
		// process all results
		while (server::pStmt->FetchRow())
		{
			writeint(server::pStmt->GetColumnInt(0));//slot
			writeint(server::pStmt->GetColumnInt(1)); //charID
			writestring(server::pStmt->GetColumnString(2)); //name
			writeint(server::pStmt->GetColumnInt(3)); //character
			writeint(server::pStmt->GetColumnInt(4)); //gender
			writeint(server::pStmt->GetColumnInt(5)); //hairdModal
			writeint(server::pStmt->GetColumnInt(6)); //hairColor
		}
		// do not forget to clean-up
		server::pStmt->FreeQuery();
		sendm(plr->socket);
		//cout << "MSG_CHARACTERSELECTION" << endl;
	}
	else{
		//bad username or password
		clearbuffer();
		writebyte(MSG_LOGIN);
		writebyte(kErrorBadLogin); //login error code 0 - bad username or password
		sendm(plr->socket);
		//cout << "kErrorBadLogin" << endl;
	}

}

void packets::msg_register(player *plr)
{
	string username;
	string email;
	string password;

	try{
		username = readstring();
		email = readstring();
		password = readstring();
	}
	catch (exception& e){
		string str(e.what());
		server::print("Wrong packet - msg_register: " + str, game::c_lightred);
		return;
	}

	if (server::pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM users \
											  WHERE username = '" + username + "';") > 0){
		clearbuffer();
		writebyte(MSG_REGISTER);
		writebyte(kErrorUsernameAlreadyExists);
		sendm(plr->socket);

		return;
	}
	if (server::pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM users \
											  WHERE email = '" + email + "';") > 0){
		clearbuffer();
		writebyte(MSG_REGISTER);
		writebyte(kErrorEmailAlreadyInUse);
		sendm(plr->socket);

		return;
	}
		
	try
	{
		int newId = static_cast<int>(server::pStmt->SqlAggregateFuncResult("SELECT MAX(userId) FROM users;")) + 1;
		server::pStmt->SqlStatement("INSERT INTO users(userId, username, password, email) VALUES(" + to_string(newId) + " \
									,'" + username + "' \
									,'" + password + "' \
									,'" + email + "')");
	}
	catch (Kompex::SQLiteException &exception)
	{
		server::print("Exception Occured ", game::c_lightred);
		exception.Show();
		server::print("SQLite result code: " + to_string(exception.GetSqliteResultCode()), game::c_lightred);
		return;
	}

	//server::print("New player registered: " + username, game::c_lightgreen);
	//Success
	clearbuffer();
	writebyte(MSG_REGISTER);
	writebyte(kSuccess); //login error code 0 - bad username or password
	sendm(plr->socket);
}

void packets::msg_changeScene(player *plr)
{
	int scene;

	try{
		scene = static_cast<int>(readbyte());
	}
	catch (exception& e){
		string str(e.what());
		server::print("Wrong packet - msg_changeScene: " + str, game::c_lightred);
		return;
	}

	switch (scene){
	case kSceneLogin:
		switch (plr->getScene()){
		case kSceneCharacterSelection:

			clearbuffer();
			writebyte(MSG_CHANGESCENE);
			writebyte(kSceneLogin);
			sendm(plr->socket);

			//server::print("Player logged out: " + plr->getUsername(), game::c_lightred);
			plr->reset();
			break;
		case kSceneCharacterCreation:
			break;
		case kSceneGame:
			//check if not attacking for 5 seconds
			break;
		}
		break;
	case kSceneCharacterSelection:
		break;
	case kSceneCharacterCreation:
		break;
	}
}

void packets::msg_returnToLogin(player *plr)
{
	//server::print("Player logged out: " + plr->getUsername(), game::c_lightred);
	plr->reset();
}

void packets::msg_createCharacter(player *plr)
{
	int slot;
	string characterName;
	int character; //skincolor
	int gender;
	int hairModal;
	int hairColor;

	try{
		slot = readint();
		characterName = readstring();
		character = readint();
		gender = readint();
		hairModal = readint();
		hairColor = readint();
	}
	catch (exception& e){
		string str(e.what());
		server::print("Wrong packet - msg_createCharacter: " + str, game::c_lightred);
		return;
	}

	if (server::pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM characters \
											  WHERE name = '" + characterName + "';") > 0){
		clearbuffer();
		writebyte(MSG_CREATECHARACTER);
		writebyte(kErrorCharacterNameTaken);
		sendm(plr->socket);

		return;
	}

	if (server::pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM characters \
											  WHERE userId = " + to_string(plr->getUserID()) + " \
											  AND slot = " + to_string(slot) + ";") > 0){
		clearbuffer();
		writebyte(MSG_CREATECHARACTER);
		writebyte(kErrorCharacterSlotTaken);
		sendm(plr->socket);

		return;
	}

	int newId;
	try
	{
		newId = static_cast<int>(server::pStmt->SqlAggregateFuncResult("SELECT MAX(charId) FROM characters;")) + 1;
		server::pStmt->SqlStatement("INSERT INTO characters(userId, charId, slot, name, character, gender, hairModal, hairColor, mapId, x, y, facing) VALUES( \
									" + to_string(plr->getUserID()) + " \
									," + to_string(newId) + " \
									," + to_string(slot) + " \
									,'" + characterName + "' \
									," + to_string(character) + " \
									," + to_string(gender) + " \
									," + to_string(hairModal) + " \
									," + to_string(hairColor) + " \
									,0 \
									,0 \
									,0 \
									,0)");
	}
	catch (Kompex::SQLiteException &exception)
	{
		server::print("Exception Occured ", game::c_lightred);
		exception.Show();
		server::print("SQLite result code: " + to_string(exception.GetSqliteResultCode()), game::c_lightred);
		return;
	}

	//server::print("New character created: " + characterName, game::c_lightgreen);
	//Success
	clearbuffer();
	writebyte(MSG_CREATECHARACTER);
	writebyte(kSuccess);
	writeint(newId);
	sendm(plr->socket);
}

void packets::msg_deleteCharacter(player *plr)
{
	int slot;
	int charID;

	try{
		slot = readint();
		charID = readint();
	}
	catch (exception& e){
		string str(e.what());
		server::print("Wrong packet - msg_deleteCharacter: " + str, game::c_lightred);
		return;
	}

	try
	{
		server::pStmt->SqlStatement("DELETE FROM characters where userId=" + to_string(plr->getUserID()) + " \
									AND charId=" +  to_string(charID) + " \
									AND slot=" + to_string(slot) );
	}
	catch (Kompex::SQLiteException &exception)
	{
		server::print("Exception Occured ", game::c_lightred);
		exception.Show();
		server::print("SQLite result code: " + to_string(exception.GetSqliteResultCode()), game::c_lightred);
		return;
	}

	//Success
	clearbuffer();
	writebyte(MSG_DELETECHARACTER);
	writebyte(kSuccess);
	//writeint(slot);
	//writeint(charID);
	sendm(plr->socket);
}

void packets::msg_enterGame(player *plr)
{
	int slot;
	int charID;

	try{
		slot = readint();
		charID = readint();
	}
	catch (exception& e){
		string str(e.what());
		server::print("Wrong packet - msg_enterGame: " + str, game::c_lightred);
		return;
	}

	if (server::pStmt->SqlAggregateFuncResult("SELECT charId FROM characters \
						   						   	WHERE userId = " + to_string(plr->getUserID()) + " \
													AND slot = " + to_string(slot) + " \
													AND charId=" + to_string(charID) + ";") > 0){

		server::pStmt->Sql("SELECT name, character, gender, hairModal, hairColor, mapId, x, y, facing FROM characters \
						   						   	WHERE userId = " + to_string(plr->getUserID()) + " \
													AND slot = " + to_string(slot) + " \
													AND charId=" + to_string(charID) + ";");
		// process all results
		while (server::pStmt->FetchRow())
		{
			//plr->setSlot(slot);
			plr->setCharacterID(charID);
			plr->setName(server::pStmt->GetColumnString(0));
			plr->setCharacter(server::pStmt->GetColumnInt(1));
			plr->setGender(server::pStmt->GetColumnInt(2));
			plr->setHairModal(server::pStmt->GetColumnInt(3));
			plr->setHairColor(server::pStmt->GetColumnInt(4));
			plr->setMapID(server::pStmt->GetColumnInt(5));
			plr->setX(server::pStmt->GetColumnInt(6));
			plr->setY(server::pStmt->GetColumnInt(7));
			plr->setFacing(static_cast<Facing>(server::pStmt->GetColumnInt(8)));
		}
		// do not forget to clean-up
		server::pStmt->FreeQuery();

		plr->setScene(kSceneLoading);

		clearbuffer();
		writebyte(MSG_ENTERGAME);
		writebyte(kSuccess); 
		sendm(plr->socket);
		//cout << "enter game sent" << endl;
	}
	else{
		clearbuffer();
		writebyte(MSG_ENTERGAME);
		writebyte(kErrorCharacterDoesNotExist);
		sendm(plr->socket);
	}
}

void packets::msg_mapData(player *plr)
{
	//Find all players that are in game and in the same map
	vector<player*> p;
	for (const auto& child : players::list)
	{
		if (child != plr && child->getScene() == kSceneGame && child->getMapID() == plr->getMapID()){
			p.push_back(child);
		}
	}
	//cout << plr->getMapID() << endl;
	clearbuffer();
	writebyte(MSG_MAPDATA);
	writeint(plr->getMapID()); //map id, int because it will have more than 255 maps in the future
	writeint(plr->getCharacterID());//characterID
	writestring(plr->getName());//name
	writeint(plr->getCharacter());//race
	writeint(plr->getGender());//gender
	writeint(plr->getHairModal());//hairmodal
	writeint(plr->getHairColor());//haircolor
	writeint(plr->getX()); //players coords
	writeint(plr->getY()); //players coords
	writebyte(static_cast<unsigned char>(plr->getFacing())); //players facing
	//player data
	writeint(p.size()); //how many players we will send
	for (const auto& child : players::list)
	{
		writeint(child->getCharacterID());//characterID
		writestring(child->getName());//name
		writeint(child->getCharacter());//race
		writeint(child->getGender());//gender
		writeint(child->getHairModal());//hairmodal
		writeint(child->getHairColor());//haircolor
		writeint(child->getX());//x
		writeint(child->getY());//y
		writebyte(static_cast<unsigned char>(child->getFacing()));//facing
	}
	//monster data
	//item data
	sendm(plr->socket);

	//inform others about me
	clearbuffer();
	writebyte(MSG_PLAYER);
	writeint(plr->getCharacterID());//characterID
	writestring(plr->getName());//name
	writeint(plr->getCharacter());//race
	writeint(plr->getGender());//gender
	writeint(plr->getHairModal());//hairmodal
	writeint(plr->getHairColor());//haircolor
	writeint(plr->getX()); //players coords
	writeint(plr->getY()); //players coords
	writebyte(static_cast<unsigned char>(plr->getFacing())); //players facing
	sendm(plr->socket,3,plr);

	plr->setScene(kSceneGame);
}
