#ifndef CpProxy_H
#define CpProxy_H

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <csignal>
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "server_socket.h"
#include "client_socket.h"
#include "socket_exception.h"

class CpProxy{
private:
	ServerSocket *server_socket;
	ClientSocket *client_socket;

	void connect(ServerSocket *, ClientSocket *);

public:
	CpProxy();
	~CpProxy();
};

#endif