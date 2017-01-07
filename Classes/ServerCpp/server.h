#ifndef __SERVER_H__
#define __SERVER_H__
//#define _WIN32_WINDOWS

//Resolve additional dependancies
#include <iostream>
//#include <string>
//#include <sstream>
//#include <ostream>
//#include <Windows.h>
//#include <Psapi.h>
//#include <fstream>
//#include "dirent.h"
//#include <direct.h>
//#include <vector>
//#include <stdio.h>
//#include <cstdlib>



//#include "ini.h"

#include "player.h"
//#include "sqlite/sqlite3.h"
#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"
#include <exception>

//#include "monster.h"
//#include "monsters.h"
//#include "item_on_ground.h"
//#include "items.h"



//Server function constants
#define SQLITE_LOGIN 1
#define MYSQL_LOGIN 2
#define LOCAL_LOGIN 3

//SERVER_* Constants
#define SERVER_INFO 1
#define SERVER_ERROR 2
#define SERVER_LOAD 3

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

//struct sqlite3;
//typedef struct sqlite3 sqlite3;
//Anything that's global goes in this "static" class
class server
{
	public:
		static void print(std::string message, int color = 7);
		static void initialize(double prt, int mtd, unsigned int plrLimit, std::string buffPass = "");

		static std::string doubleToString(double val);
		static float rand_FloatRange(float a, float b);
		static int newPlayerID();

		//Global variables
		static double socket;
		static double port;

		static int method;

		static char* bufferPassword;

		static bool playerID[255];

		static bool bufferEncrypt;

		static std::string messageOfTheDay;

		//static struct sqlite3 *db;
		//static sqlite3 *db;// = NULL;
		static Kompex::SQLiteDatabase *pDatabase;
		static Kompex::SQLiteStatement *pStmt;

		static int callback(void *NotUsed, int argc, char **argv, char **azColName);
};

//Important networking functions and other functions as well
void sendm(double sock, int method = 1, player* plr = NULL);
bool fexists(const char *filename);

//Include protection
#endif // __SERVER_H__