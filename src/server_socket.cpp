// Class file of Server Socket 

#include "server_socket.h"

ServerSocket::ServerSocket(){
	if(!Socket::create()){
		throw SocketException(strerror(errno));
	}
}

ServerSocket::ServerSocket(int port){
	if(!Socket::create()){
		throw SocketException(strerror(errno));
	}

	if(!Socket::bind(port)){
		throw SocketException(strerror(errno));
	}

	if(!Socket::listen()){
		throw SocketException(strerror(errno));
	}
}

ServerSocket::~ServerSocket(){

}

ServerSocket& ServerSocket::operator << (std::string& s){
	if(Socket::send(s)==-1){
		throw SocketException(strerror(errno));
	}
	return *this;
}

ServerSocket& ServerSocket::operator >> (std::string& s) {
	if(Socket::recv(s)==-1){
		throw SocketException(strerror(errno));
	}
	return *this;
}

int ServerSocket::fd(){
	return Socket::fd();
}

void ServerSocket::accept(ServerSocket& _socket){
	if(!Socket::accept(_socket)){
		throw SocketException(strerror(errno));
	}
}

void  ServerSocket::close(){
	if(!Socket::close()){
		throw SocketException(strerror(errno));
	}
}

