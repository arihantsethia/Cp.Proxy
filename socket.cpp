#include "socket.h"

Socket::Socket(){
	memset(&_addr, 0, sizeof _addr);
	_sockfd = -1;
}

Socket::~Socket(){
	if(is_valid()){
		close();
	}
}

bool Socket::is_valid(){
	return _sockfd!=-1;
}

int Socket::fd(){
	return _sockfd;
}

void Socket::fd(int _fd){
	_sockfd = _fd;
}

bool Socket::create(){
	_sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if(!is_valid()) {
		return false;
	}

	int yes = 1;
	
	if (setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
		return false;
	}

	return true;
}

bool Socket::bind(int port){
	if(!is_valid()){
		return false;
	}

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	memset(_addr.sin_zero, '\0', sizeof _addr.sin_zero);

	if(::bind(_sockfd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1){
		return false;
	}

	return true;
}

bool Socket::connect(int ip, int port){
	if(!is_valid()){
		return false;
	}

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = ip;

	if(::connect(_sockfd, (sockaddr *)&_addr, sizeof(_addr)) == -1){
		return false;
	}

	return true;
}

bool Socket::listen(){
	if(!is_valid()){
		return false;
	}

	if(::listen(_sockfd, BACKLOG) == -1){
		return false;
	}

	return true;
}

bool Socket::accept(Socket& child_socket){
	if(!is_valid()){
		return false;
	}
	
	int addr_size = sizeof(_addr);
	int temp_fd = ::accept(_sockfd, (struct sockaddr *)&_addr,(socklen_t *) &addr_size);
	if(temp_fd == -1){
		return false;
	}

	child_socket.fd(temp_fd);

	return true;
}

int Socket::send(std::string msg){


	std::cout<<msg<<std::endl;


	if(!is_valid()){
		return 0;
	}

	return ::send(_sockfd, msg.c_str(), msg.length(), 0);
}

int Socket::recv(std::string& s){
	if(!is_valid()){
		return -1;
	}

	char buffer[MAXRECV+1];
	int status = ::recv(_sockfd, buffer, MAXRECV, 0);

	if(status > 0){
		s.assign(buffer,status);
	}

	return status;
}

bool Socket::close(){
	if(!is_valid()){
		return true;
	}

	if(::close(_sockfd) == -1){
		return false;
	}

	return true;
}