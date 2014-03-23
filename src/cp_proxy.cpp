#include "cp_proxy.h"
#include "utility.h"

CpProxy::CpProxy(int port){
	std::cout<<"Cp-Proxy Started\n";
	ServerSocket server(port);	
	ServerSocket *server_socket;
	std::cout<<"Listening on Port : "<<port<<std::endl;
	try{
		//Creates new socket which listens to requests from clients
		server_socket = new ServerSocket();	
	} catch(SocketException &e){
		std::cout<<"Exception occurred : "<<e.description()<<std::endl;
		return ;
	}
	while(1){
		try{
			/* 
				Accept incoming connections in sequential manner after serving the
				previous request. Once the connection is accepted the connect function fetches
				the requested data and send it back to the client. After this socket needs to be closed.  
			*/
			server.accept(*server_socket);
			connect(server_socket);
			(*server_socket).close();
		} catch(SocketException &e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			continue ;
		}
	}
}

//Destructor function
CpProxy::~CpProxy(){

}

// This function fetches the requested data and sends the requested data back to the client. 
void CpProxy::connect(ServerSocket *server_socket){
	ClientSocket *client_socket;	//socket which acts as a client and requests the data from webservers.
	std::string buffer="",data="",returnData="";
	while(1){
		try{
			//recv data from the client and store it in buffer
			*server_socket >> buffer;
		} catch(SocketException &e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			return ; 
		}
		data += buffer;
		if(data.find("\r\n\r\n")!=std::string::npos){
			break;
		}
	}
	/*
		Parse the required header to check for valid format.
		And obtain host address in host, port number in urlPort, and relative URL in r_url
	*/ 
	std::string host,r_url,urlPort;
	int returnCode = parseheader(data,host,r_url,urlPort);
	if(returnCode==200){
		data = convertheader(data,host,r_url,urlPort);	//Format the header properly.
		std::cout<<"\nUrl :"<<r_url<<"\nHost:" << host<<std::endl;
		try{
			int ip = hostlookup(host);					//Obtain the (int)IP from the host address
			client_socket = new ClientSocket(ip,atoi(urlPort.c_str())); //Create a clien socket and connect to the given IP and Port
			(*client_socket)<<data;						// Send properly formatted header to the webserver vis the client socket.
		} catch(SocketException& e){
			std::cout<<"Exception occurred : "<<e.description()<<std::endl;
			return;
		}
		while(1){
			buffer = "";
			try{
				(*client_socket)>>buffer;				//Recieve data from webserver and store it inside the buffer
				if(!buffer.length()){
					break;
				}
				(*server_socket)<<buffer;				// Send data back to the client which had earlier requested the page. 
			}catch(SocketException& e){
				std::cout<<"Exception occurred : "<<e.description()<<std::endl;
				break;
			}			
		}
		(*client_socket).close();
	} else if(returnCode == 400){
		// Send a valid "400 Bad Request" HTTP/1.0 Header
		std::cout<<"Bad Request\n"<<data<<std::endl;
		buffer = "HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close";
		buffer =buffer + "Content-Type: text/html;\r\ncharset=iso-8859-1\r\n"\
				+"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\"><html><head><title>400 Bad Request</title></head>"\
				+"<body><h1>Bad Request</h1><p>Your browser sent a request that this server could not understand."\
				+"<p>The request line contained invalid characters following the protocol string.</p></p></body></html>";
		(*server_socket)<<buffer;
	} else if(returnCode == 501){
		// Send a valid "501 Method Not Implemented" HTTP/1.0 Header
		std::cout<<"Method Not Implemented\n"<<data<<std::endl;
		buffer = "HTTP/1.0 501 Method Not Implemented\r\nContent-Type: text/html\r\nConnection: close";
		buffer= buffer +"Content-Type: text/html;\r\ncharset=iso-8859-1\r\n"\
				+"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\"><html><head><title>501 Method Not Implemented</title></head>"\
				+"<body><h1>Method Not Implemented</h1><p>Requested method is not supported by server.</p></body></html>";
		(*server_socket)<<buffer;
	}
	return;		//Return to accept further incoming connections.
}