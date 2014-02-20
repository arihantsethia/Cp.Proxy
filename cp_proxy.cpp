#include "cp_proxy.h"
#include "utility.h"

CpProxy::CpProxy(int port){
	std::cout<<"Cp-Proxy Started\n";
	ServerSocket server(port);	
	ServerSocket *server_socket;
	std::cout<<"Listening on Port : "<<port<<std::endl;
	try{
		server_socket = new ServerSocket();
	} catch(SocketException &e){
		std::cout<<"Exception occurred : "<<e.description()<<std::endl;
		return ;
	}
	while(1){
		try{
			server.accept(*server_socket);
			connect(server_socket);
			(*server_socket).close();
		} catch(SocketException &e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			continue ;
		}
	}
}

CpProxy::~CpProxy(){

}

void CpProxy::connect(ServerSocket *server_socket){
	ClientSocket *client_socket;
	std::string buffer="",data="",returnData="";
	while(1){
		try{
			*server_socket >> buffer;
		} catch(SocketException &e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			return ; 
		}
		data += buffer;
		if(buffer.length()==0 || buffer.find("\r\n",buffer.length()-2) != std::string::npos){
			break;
		}
	}
	std::string host,r_url;
	int urlPort;
	int returnCode = parseheader(data,host,r_url,urlPort);
	if(returnCode==200){
		data = convertheader(data,host,r_url,urlPort);
		std::cout<<"Data : \n"<<data<<"\n URL :"<<r_url<<"\n Host:" << host<<std::endl;
		try{
			int ip = hostlookup(host);
			client_socket = new ClientSocket(ip,urlPort);
			(*client_socket)<<data;
		} catch(SocketException& e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			return;
		}
		while(1){
			buffer = "";
			try{
				(*client_socket)>>buffer;
				if(!buffer.length()){
					break;
				}
				(*server_socket)<<buffer;
			}catch(SocketException& e){
				std::cout<<"Exception occurred : "<<e.description()<<std::endl;
				break;
			}			
		}
	} else if(returnCode == 404){
		std::cout<<"ILL Formed Request"<<std::endl;
	} else if(returnCode == 501){
		std::cout<<"Not Implemented Request"<<std::endl;
	}
	//(*client_socket).close();
	return;
}