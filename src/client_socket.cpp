// Class file of Client Socket 

#include "client_socket.h"

ClientSocket::ClientSocket(){

}

ClientSocket::ClientSocket(int ip, int port){
	if(!Socket::create()){
		throw SocketException(strerror(errno));
	}
	if(!Socket::connect(ip,port)){
		throw SocketException(strerror(errno));
	}
}

ClientSocket::~ClientSocket(){
	Socket::close();
}

ClientSocket& ClientSocket::operator << (std::string& s){
	if(Socket::send(s)==-1){
		throw SocketException(strerror(errno));
	}
	return *this;
}

ClientSocket& ClientSocket::operator >> (std::string& s){
	if(Socket::recv(s)==-1){
		throw SocketException(strerror(errno));
	}
	return *this;
}

int ClientSocket::fd(){
	return Socket::fd();
}

void ClientSocket::close(){
	if(!Socket::close()){
		throw SocketException(strerror(errno));
	}
}
