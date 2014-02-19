#include "cp_proxy.h"
#include "utility.h"

CpProxy::CpProxy(int port){
	std::cout<<"Cp-Proxy Started\n";
	ServerSocket server(port);
	std::cout<<"Listening on Port : "<<port<<std::endl;
	ServerSocket *server_socket ;
	server_socket= new ServerSocket();
	while(1){
		server.accept(*server_socket);
		connect(server_socket);
		(*server_socket).close();
	}
}

CpProxy::~CpProxy(){

}

void CpProxy::connect(ServerSocket *server_socket){
	ClientSocket *client_socket;
	std::string buffer="",data="";
	char buf[1024];
	while(1){
		try{
			*server_socket >> buffer;
		} catch(SocketException &e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			return ; 
		}
		data += buffer;
		if(buffer.find("\r\n",buffer.length()-2) != std::string::npos){
			break;
		}
	}
	std::cout<<data<<std::endl;
	int returnCode = isvalidheader(data);
	if(returnCode!=404 || returnCode!=500){
		std::string host,r_url;
		int urlPort;
		parseheader(data,host,r_url,urlPort);
		data = convertheader(data);//,host,r_url,urlPort);
		std::cout<<hostlookup(host)<<std::endl;
		client_socket = new ClientSocket(hostlookup(host),urlPort);
		(*client_socket)<<data;
		while(1){
			buffer = "";
			try{
				(*client_socket)>>buffer;
			}catch(SocketException& e){
				std::cout<<"Exception occurred : "<<e.description()<<std::endl;
				break;
			}
			std::cout<<buffer<<std::endl;
		}
	}


}