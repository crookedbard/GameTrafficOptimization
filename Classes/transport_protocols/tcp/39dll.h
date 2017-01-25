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
double tcpconnected(double sockid);
double udpconnect(double port, double mode);
double sendmessage(double sockid, char* ip = (char *)"", double port = 0, double buffid = 0);
double receivemessage(double sockid, double len = 0, double buffid = 0);
double peekmessage(double sockid, double len, double buffid);

double setnagle(double sockid, double value);

/*
 * setformat(sockid, method, sepstr) - Messages sent over TCP are encoded in special ways. This function takes a specified socket and sets the method of transmission for the packets.
 * Argument0 = Sockid-self-explanatory, choose which socket to set to the specified method
 * Argument1 = Method- There are three ways to encode data:
 * 0-or binary format. This mode puts an extra two bytes containing integers at the beginning of each packet saying how large the packet is. This is the default.
 * 1-or Text format. This mode puts a delimiter (special character) at the end of every packet. This is usually used for text based protocols like IRC
 * 2-or no format. This mode makes it so that all packets are raw, without any extra encoding to the packets. We set GG2 sockets to this mode while receiving packets.
 * This is very important because if you receive a message that is encoded in a different method, then the game will interpret it improperly 
 * (for example if the message is encoded as 0 binary message, but you expect a 2 or raw format message, then the game will interpret the first two bytes (which detonate the size) as a part of the message, leading to errors and issues.
 * Argument3 = Sepstr-this is the value you use to note the end of a packet when you use Text format. GG2 doesn’t use this format, so it’s set to 0, but it could be set to anything as it is unused
 */
double setformat(double sockid, double mode, char* sep);

/*
 * setsync(sockid, mode) - Changes the blocking / non-blocking method of a socket. 
 * Argument0 = Socket to change.
 * Argument1 = Mode to set to. 0 = Blocking. 1 = Non-blocking. 
 */
double setsync(double sockid, double mode);

double closesock(double sockid);
double socklasterror(double sockid);
char* myhost();
double compareip(char* ip, char* mask);
double sockexit();
double sockstart();
char* hostip(char* host);
double getsocketid(double sockid);
char* lastinIP();
double lastinPort();

//Buffer Prototypes
int writebyte(double val, double buffid = 0);
int writeshort(double val, double buffid = 0);
int writeushort(double val, double buffid = 0);
int writeint(double val, double buffid = 0);
int writeuint(double val, double buffid = 0);
int writefloat(double val, double buffid = 0);
int writedouble(double val, double buffid = 0);
int writechars(char* str, double buffid = 0);
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
char* readsep(char* sep, double buffid = 0);
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
//double fileopen(char*name, double mode);
//double fileclose(double fileid);
//double filewrite(double fileid, double buffid);
//double fileread(double fileid, double bytes, double buffid);
//double filepos(double fileid);
//double filesetpos(double fileid, double pos);
//double filesize(double fileid);
double iptouint(char* ip);
char* uinttoip(double ip);

int AddBuffer(CBuffer* b);
int AddSocket(CSocket* b);
//int AddFile(HANDLE b);
double dllInit();
double dllFree();

//my custom
CBuffer* getbuffer(double buffid = 0);

#endif // __39DLL_H__
