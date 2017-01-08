#ifndef __39DLL_H__
#define __39DLL_H__

#include "list.h"
#include "buffer.h"
#include "tools.h"
#include "socket.h"
#include <iostream>

//Sockets
double tcpconnect(std::string ip, double port, double mode);
double tcplisten(double port, double max, double mode);
double tcpaccept(double sockid, double mode);
char* tcpip(double sockid);
double setnagle(double sockid, double value);
double tcpconnected(double sockid);
double udpconnect(double port, double mode);
double sendmessage(double sockid, char*ip = "", double port = 0, double buffid = 0);
double receivemessage(double sockid, double len = 0, double buffid = 0);
double peekmessage(double sockid, double len, double buffid);
double setformat(double sockid, double mode, char*sep);
char* lastinIP();
double lastinPort();
double setsync(double sockid, double mode);
double closesock(double sockid);
double socklasterror(double sockid);
char* myhost();
double compareip(char*ip, char*mask);
double sockexit();
double sockstart();
char* hostip(char*host);
double getsocketid(double sockid);

//Buffer Prototypes
int writebyte(double val, double buffid = 0);
int writeshort(double val, double buffid = 0);
int writeushort(double val, double buffid = 0);
int writeint(double val, double buffid = 0);
int writeuint(double val, double buffid = 0);
int writefloat(double val, double buffid = 0);
int writedouble(double val, double buffid = 0);
int writechars(char*str, double buffid = 0);
int writestring(std::string str, double buffid = 0);
unsigned char readbyte(double buffid = 0);
short readshort(double buffid = 0);
unsigned short readushort(double buffid = 0);
int readint(double buffid = 0);
unsigned int readuint(double buffid = 0);
float readfloat(double buffid = 0);
double readdouble(double buffid = 0);
char* readchars(double len, double buffid = 0);
char* readstring(double buffid = 0);
char* readsep(char*sep, double buffid = 0);
double getpos(double pos, double buffid = 0);
double clearbuffer(double buffid = 0);
double buffsize(double buffid = 0);
double setpos(double pos, double buffid = 0);
double bytesleft(double buffid = 0);
double createbuffer();
double freebuffer(double buffid = 0);
double copybuffer(double destinationid, double sourceid);
double copybuffer2(double destinationid, double start, double len, double sourceid);

//tools
//char* getmacaddress(); //doesnt work
std::string md5string(std::string str);
char* md5buffer(double buffid); 
double bufferencrypt(std::string pass, double buffid = 0);
double adler32(double buffid);
double bufferexists(double buffid);
double netconnected();
double fileopen(char*name, double mode);
double fileclose(double fileid);
double filewrite(double fileid, double buffid);
double fileread(double fileid, double bytes, double buffid);
double filepos(double fileid);
double filesetpos(double fileid, double pos);
double filesize(double fileid);
double iptouint(char*ip);
char* uinttoip(double ip);

int AddBuffer(CBuffer*b);
int AddSocket(CSocket*b);
//int AddFile(HANDLE b);
double dllInit();
double dllFree();

//my custom
CBuffer* getBuffer(double buffid);

#endif // __39DLL_H__