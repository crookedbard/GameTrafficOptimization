#include "main.h"
#include "maps.h"
#include "players.h"

int main()
{

	server::print("\n\
                          ___  ___  ___ ___ _____   __\n\
  BGSERV Version 0.0.1  | _ \ / __\/ __| __| _ \\ \\ / /  http://bardsgrimoire.com/\n\
========================| _ <|(_)\\__ \\ _||   /\\ ` /===========================\n\
                        |___/\___/|___/___|_|_\\ \\_/    dainiusu@live.com\n\
\n\
BG Version Support: .01 .01\n\
\n", game::c_lightmagenta);

    double port = 1337;

	server::initialize(port, SQLITE_LOGIN, 255, "");
	Maps::init();
	//players::initialize();
	server::print("Done initialization, server is ready to accept packets.", game::c_yellow);

	while(true)
	{
		players::listen(); //Handle incoming packets
		//Prevent CPU from taking 100% cycles
		Sleep(1);
	}

	//All paths must return value
	//if (server::db != nullptr){ sqlite3_close(server::db); server::db = nullptr; }

	if (server::pDatabase	!= nullptr)	delete server::pDatabase;
	if (server::pStmt		!= nullptr)	delete server::pStmt;

	closesock(server::socket);
	dllFree();
    return 0;

}
