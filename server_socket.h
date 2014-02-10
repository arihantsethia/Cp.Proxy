#ifndef CLIENTSOCKET_CLASS
#define CLIENTSOCKET_CLASS

#include "socket.h"

class ServerSocket : private Socket{
public:
	ServerSocket();
	ServerSocket(int port);
	~ServerSocket();
	ServerSocket operator << (std::string);
	ServerSocket operator >> (std::string);
	void accept();
	void close();
	int fd();
};

#endif