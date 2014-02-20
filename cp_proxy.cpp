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
		std::cout<<"Here\n";
		(*server_socket).close();
	}	
}

CpProxy::~CpProxy(){

}

void CpProxy::connect(ServerSocket *server_socket){
	ClientSocket *client_socket;
	std::string buffer="",data="",returnData="";
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
	std::string host,r_url;
	int urlPort;
	int returnCode = parseheader(data,host,r_url,urlPort);
	if(returnCode==200){
		data = convertheader(data,host,r_url,urlPort);//,host,r_url,urlPort);
		std::cout<<"Data : "<<data<<"\n URL :"<<r_url<<"\n Host:" << host<<std::endl;
		client_socket = new ClientSocket(hostlookup(host),urlPort);
		std::cout<<"Here Before sending data\n";
		(*client_socket)<<data;
		while(1){
			buffer = "";
			try{
				(*client_socket)>>buffer;
			}catch(SocketException& e){
				std::cout<<"Exception occurred : "<<e.description()<<std::endl;
				break;
			}
			if(!buffer.length()){
				break;
			}
			std::cout<<buffer<<std::endl;
			(*server_socket)<<buffer;
		}
	} else if(returnCode == 404){
		std::cout<<"ILL Formed Request"<<std::endl;
		std::cout<<"Data : "<<data<<std::endl;
	} else if(returnCode == 500){
		std::cout<<"Not Implemented Request"<<std::endl;
	}
	(*client_socket).close();
	return;
}