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
	std::string host,r_url,urlPort;
	int returnCode = parseheader(data,host,r_url,urlPort);
	if(returnCode==200){
		data = convertheader(data,host,r_url,urlPort);
		std::cout<<"Url :"<<r_url<<"\nHost:" << host<<std::endl;
		try{
			int ip = hostlookup(host);
			client_socket = new ClientSocket(ip,atoi(urlPort.c_str()));
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
		(*client_socket).close();
	} else if(returnCode == 400){
		buffer = "HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close";
		buffer =buffer + "Content-Type: text/html;\r\ncharset=iso-8859-1\r\n"\
				+"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\"><html><head><title>400 Bad Request</title></head>"\
				+"<body><h1>Bad Request</h1><p>Your browser sent a request that this server could not understand."\
				+"<p>The request line contained invalid characters following the protocol string.</p></p></body></html>";
		(*server_socket)<<buffer;
	} else if(returnCode == 501){
		buffer = "HTTP/1.0 501 Method Not Implemented\r\nContent-Type: text/html\r\nConnection: close";
		buffer= buffer +"Content-Type: text/html;\r\ncharset=iso-8859-1\r\n"\
				+"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\"><html><head><title>501 Method Not Implemented</title></head>"\
				+"<body><h1>Method Not Implemented</h1><p>Requested method is not supported by server.</p></body></html>";
		(*server_socket)<<buffer;
	}
	return;
}