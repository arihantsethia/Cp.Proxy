#ifndef SOCKETEXCEPTION_CLASS
#define SOCKETEXCEPTION_CLASS

#include <string>

class SocketException {
private:
	std::string message;

public:
	SocketException(std::string);
	~SocketException();
	std::string description();
};

#endif
