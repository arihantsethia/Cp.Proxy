#include <string>
#include "socket_exception.h"

SocketException::SocketException(std::string s){
	throw s;
}

SocketException::~SocketException(){

}