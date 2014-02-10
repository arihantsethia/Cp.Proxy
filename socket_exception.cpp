#include <string>

SocketException::SocketException(std::string s){
	throw s;
}

SocketException::~SocketException(){

}