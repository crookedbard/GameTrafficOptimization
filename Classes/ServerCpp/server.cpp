#include "server.h"
#include <sstream>
//#include <ostream>
#include <fstream>

#include "39dll.h"
#include "players.h"
#include "game.h"
//Using namespace standard to resolve calls
using namespace std;

double server::port;
double server::socket;
//double server::UDPsocket;

int server::method;

bool server::playerID[255];
bool server::bufferEncrypt;

char *server::bufferPassword;

string server::messageOfTheDay;

//sqlite3* server::db = nullptr;

Kompex::SQLiteDatabase* server::pDatabase;
Kompex::SQLiteStatement* server::pStmt;

void sendm(double sock, int method, player* plr)
{
	//Send message with 39dll
	if(server::bufferEncrypt)
	{
		//Only encrypt if there was an initial password set
		bufferencrypt(server::bufferPassword);
	}

	switch(method)
	{
		case 1:
			//Used for private messages
			//Send message to specific SOCKET
			sendmessage(sock);
		break;

		case 2:
			//Used to send map movement, local chat and so on
			//Send message to all InGame SameMap players including me
			for (const auto& child : players::list)
			{
				if (child->getScene() == kSceneGame)
				{
					if (child->getMapID() == plr->getMapID())sendmessage(child->socket);
				}
			}
			break;

		case 3:
			//Send message to all InGame SameMap players except me
			for (const auto& child : players::list)
			{
				if (child->getScene() == kSceneGame && child != plr && child->getMapID() == plr->getMapID())
				{
					sendmessage(child->socket);
				}
			}
			break;
		case 5:
			//Send message to all logged in players INCLUDING me
			for(unsigned int i=0; i<players::list.size(); i++)
			{
				sendmessage(players::list[i]->socket);
				/*if(players::list[i]->loggedIn)
				{
					sendmessage(players::list[i]->socket);
				}*/
			}
		break;


		case 9:
			//Send message to all 
			for(unsigned int i=0; i<players::list.size(); i++)
			{
				cout<<"PLR SOCK: "<<players::list[i]->socket<<endl;
				sendmessage(players::list[i]->socket);
			}
		break;
	}
}

bool fexists(const char *filename)
{
	//Returns whether or not a file exists
	ifstream ifile(filename);
	return ifile.is_open();
}

void server::print(string message, int color)
{
	//Print message to console
	char timeStr [9];
	_strtime_s( timeStr );

	string time = timeStr;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
	cout << time << ": " << message << endl;

	//Reset console color
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)game::c_lightgrey);

	//Log message(s) in an external log file
	
	/*
		You should append all printed console messages to an external log file
		(*.txt) and create a new *.txt file for every day. That is beyond the
		scope of this tutorial, but is not hard to do if you understand the
		structure of this server
	*/
}

int server::newPlayerID()
{
	//Generate new playerid
	for (unsigned int id = 0; id < 255; id++)
	{
		if (playerID[id] == false)
		{
			playerID[id] = true;
			return id;
		}
	}

	//Else, no player id available
	return -1;
}

string server::doubleToString(double val)
{
	//Convert double value to string
	ostringstream strs;
	strs << val;
	return strs.str();
}

float server::rand_FloatRange(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

void server::initialize(double prt, int mtd, unsigned int plrLimit, string buffPass)
{
	//Initialize server
	port = prt;
	method = mtd;

	if(buffPass != "")
	{
		bufferEncrypt = true;
		bufferPassword = new char[buffPass.size()+1];
		strcpy_s(bufferPassword, sizeof(buffPass)+1, buffPass.c_str());
	}
	else
	{
		bufferEncrypt = false;
	}

	//Sqlite
	//sqlite3 *db;

	/*int rc;

	rc = sqlite3_open("BardsGrimoireDB.db", &db);

	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stderr, "Opened database successfully\n");
	}

	// Create SQL statement 
	char *zErrMsg = 0;
	//int  rc;
	char *sql;

	sql = "SELECT * from USERS";
	//sql = "INSERT INTO USERS (userId,username,password,email) values (2,'Jonas','Jonas','Jonas@one.lt')";
	// Execute SQL statement 
	const char* data = "Callback function called";
	rc = sqlite3_exec(db, sql, callback, (void*)data /, &zErrMsg);


	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Querry successfully executed.\n");
	}
	*/
	try
	{
		// create and open database
		pDatabase = new Kompex::SQLiteDatabase("BardsGrimoireDB.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
		// create statement instance for sql queries/statements
		pStmt = new Kompex::SQLiteStatement(pDatabase);

		/*pStmt->SqlStatement("INSERT INTO users (userId, username, password, email) \
							                     VALUES (1, 'test', '"+md5string("test")+"', 'test@bardsgrimoire.com')");*/
		print("Opened database successfully. SQLite version: " + to_string(pDatabase->GetLibVersionNumber()), game::c_yellow);
		//cout << "SQLite version: " << pDatabase->GetLibVersionNumber() << endl;
	}
	catch (Kompex::SQLiteException &exception)
	{
		//cerr << "\nException Occured" << endl;
		print("Exception Occured ", game::c_lightred);
		exception.Show();
		//cerr << "SQLite result code: " << exception.GetSqliteResultCode() << endl;
		print("SQLite result code: " + to_string(exception.GetSqliteResultCode()), game::c_lightred);
	}
	//Open listening socket
	dllInit();
	socket = tcplisten(port, 2, 1);
	
	if(socket <= 0)
	{
		print("Listening socket could not be opened on port "+doubleToString(port), game::c_lightred);
		closesock(socket);
		dllFree();
		system("PAUSE");
		exit(0);
	}
	else
	{
		setnagle(socket, false);
		print("Server listening for incoming connections on port "+doubleToString(port), game::c_yellow);
	}

}

/*int server::callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}*/

int server::callback(void *data, int argc, char **argv, char **azColName){
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}