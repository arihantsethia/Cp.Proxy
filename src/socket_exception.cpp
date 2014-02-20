#include <string>
#include "socket_exception.h"

SocketException::SocketException(std::string s){
	message = s;
}

SocketException::~SocketException(){

}

std::string SocketException::description(){
	return message;
}