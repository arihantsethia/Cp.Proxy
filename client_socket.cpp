#include "client_socket.h"

ClientSocket::ClientSocket(){

}

ClientSocket::ClientSocket(int ip, int port){
	if(!Socket::create()){
		SocketException("Client socket could not be created.\n");
	}
	if(!Socket::connect(ip,port)){
		SocketException("Socket couldn't connect to the host.");
	}
}

ClientSocket::~ClientSocket(){

}

ClientSocket ClientSocket::operator << (std::string s){

		std::cout<<s<<std::endl;
	if(!Socket::send(s)){
		SocketException("Couldn't write to socket.\n");
	}
	return *this;
}

ClientSocket ClientSocket::operator >> (std::string s){
	if(!Socket::recv(s)){
		SocketException("Couldn't read from socket.\n");
	}
	return *this;
}

int ClientSocket::fd(){
	return Socket::fd();
}

void ClientSocket::close(){
	if(!Socket::close()){
		SocketException("Couldn't close the client socket.");
	}
}
