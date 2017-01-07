#ifndef __PACKETS_H__
#define __PACKETS_H__

//Additional include dependancies
#include <iostream>
#include "server.h"
#include "player.h"

class player;

//Custom class
class packets
{
	public:
		//Bard's Grimoire
		
		static void msg_login(player* plr);
		static void msg_register(player* plr);
		static void msg_changeScene(player* plr);
		static void msg_returnToLogin(player* plr);
		static void msg_createCharacter(player* plr);
		static void msg_deleteCharacter(player* plr);
		static void msg_enterGame(player* plr);
		static void msg_mapData(player* plr);
		/*static void msg_plrMoveDL(player* plr);
		static void msg_plrMoveDR(player* plr);
		static void msg_plrMoveUL(player* plr);
		static void msg_plrMoveUR(player* plr);
		static void msg_plrStopDL(player* plr);
		static void msg_plrStopDR(player* plr);
		static void msg_plrStopUL(player* plr);
		static void msg_plrStopUR(player* plr);*/
		//static void msg_plrStop(player* plr);
		//static void msg_text(player* plr);
		//static void msg_login_old(player* plr);
		//static void msg_default(player* plr);
private:
	//static void msg_plrMove(player* plr);
};
//Include Protection
#endif // __PACKETS_H__