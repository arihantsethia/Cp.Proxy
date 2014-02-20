#include "server_socket.h"

ServerSocket::ServerSocket(){
	if(!Socket::create()){
		SocketException("Server socket could not be created\n");
	}
}

ServerSocket::ServerSocket(int port){
	if(!Socket::create()){
		SocketException("Server socket could not be created.\n");
	}

	if(!Socket::bind(port)){
		SocketException("Socket couldn't bind to the port.\n");
	}

	if(!Socket::listen()){
		SocketException("Server socket listen on the port.\n");
	}
}

ServerSocket::~ServerSocket(){

}

ServerSocket& ServerSocket::operator << (std::string& s){
	if(!Socket::send(s)){
		SocketException("Couldn't write to socket.\n");
	}
	return *this;
}

ServerSocket& ServerSocket::operator >> (std::string& s) {
	if(!Socket::recv(s)){
		SocketException("Couldn't read from socket.\n");
	}
	return *this;
}

int ServerSocket::fd(){
	return Socket::fd();
}

void ServerSocket::accept(ServerSocket& _socket){
	if(!Socket::accept(_socket)){
		SocketException("Couldn't accept socket.\n");
	}
}

void  ServerSocket::close(){
	if(!Socket::close()){
		SocketException("Couldn't close the server socket");
	}
}

