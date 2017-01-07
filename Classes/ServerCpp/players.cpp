#include "players.h"
#include "game.h"
#include "39dll.h"
#include "server.h"
//Declare namespace standard below
using namespace std;

double players::acceptSocket;
vector<player*> players::list;
players* players::plrs; //plr_timerio pointeris plrs
//int players::players_online;

void players::listen()
{
	//Handle login packets
	acceptSocket = tcpaccept(server::socket, 1);

	//Handle any possible new incoming connections
	if(acceptSocket > 0)
	{
		string s(lastinIP());
		string sok = server::doubleToString(acceptSocket);
		server::print("Trying to connect "+ s+" and: "+ sok, game::c_green);

		bool already_exist = false;
		for (unsigned int i = 0; i < list.size(); i++)
		{
			if (list[i]->ipAddress == lastinIP()) already_exist = true;
		}

		if (!already_exist) { new player(acceptSocket, lastinIP()); }
		else { closesock(acceptSocket); }
	}

	//Handle existing players below
	for(unsigned int i = 0; i <list.size(); i++)
	{
		
		double size;

			size = receivemessage(list[i]->socket); //Each player in the login list has the following command(s) executed

		if(size > 0)
		{
			//server::print("handle: ", game::c_blue);
			list[i]->handle(); //This client has data on the buffer
		}
		list[i]->act();
		/*else*/ if(size == 0)
		{
			server::print("Connection ended: "+ list[i]->ipAddress, game::c_red);
			delete list[i]; //This player has lost connection
		}
	}
}

player* players::findPlayer(int id)
{
	//Find a given player with playerid
	for(unsigned int i=0; i<list.size(); i++)
	{
		if(list[i]->playerID == id)
		{
			return list[i];
		}
	}

	//Else nothing found
	return nullptr;
}

player* players::findPlayer(string username)
{
	//Find a given player with playerid
	for (unsigned int i = 0; i<list.size(); i++)
	{
		if (list[i]->getUsername() == username)
		{
			return list[i];
		}
	}

	//Else nothing found
	return nullptr;
}

void players::initialize()
{
	//players::plrs = new players();
	//players_online=0;
	
}
