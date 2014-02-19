#include "utility.h"

std::vector<std::string> tokenize(std::string s, std::string sep){
	// Skip delimiters at beginning.
	std::string::size_type lastPos = s.find_first_not_of(sep, 0);	
	// Find first "non-delimiter", which will be between lastPos and pos
	std::string::size_type pos = s.find_first_of(sep, lastPos); 
	std::vector<std::string> tokens;
	while(pos != std::string::npos || lastPos != std::string::npos){
		tokens.push_back(s.substr(lastPos,(pos - lastPos)));
		// Skip delimiters
		lastPos = s.find_first_not_of(sep, pos);	
		// Find "non-delimiter", which will be between lastPos and pos
		pos = s.find_first_of(sep, lastPos); 
	}
	return tokens;
}

std::string gethostname(std::string hdr,std::vector<std::string> tokens){
	std::vector<std::string>::iterator it;
	for(it=tokens.begin();it!=tokens.end();it++){
		if((*it).find("Host:",0)!=std::string::npos){
			return (*it).substr(6);
		}
	}
	return "-1";
}

std::string geturl(std::string hdr, std::vector<std::string> tokens){
	std::vector<std::string>::iterator it;
	for(it=tokens.begin();it!=tokens.end();it++){
		std::string::size_type pos = (*it).find("GET ",0);
		if(pos!=std::string::npos){
			std::string::size_type endPos = (*it).find("/ ",pos);
			return (*it).substr(4,endPos-3);
		}
	}
	return "-1";
}
/*
std::string convertheader(std::string s){

}*/

int hostlookup(std::string h){
	const char *host = h.c_str();
	struct sockaddr_in inaddr;
	struct hostent *hostp;

	if ((host == NULL) || (*host == '\0')){
		return(INADDR_ANY);
	}

	memset ((char * ) &inaddr, 0, sizeof inaddr);

	if ((int)(inaddr.sin_addr.s_addr = inet_addr(host)) == -1){
		if ((hostp = gethostbyname(host)) == NULL){
			errno = EINVAL;
			return -1;
		}
		if (hostp->h_addrtype != AF_INET){ 
			errno = EPROTOTYPE;
			return(-1);
		}
		memcpy((char * ) &inaddr.sin_addr, (char * ) hostp->h_addr, sizeof(inaddr.sin_addr));
	}
	
	return(inaddr.sin_addr.s_addr);
}

void parseheader(std::string data, std::string& host, std::string &r_url, int& port){
	// "\r\n" = <CR><LF> is defined as line break in the protocol specification (RFC1945) 
	std::string::size_type pos;
	std::string sep = "\r\n";
	std::vector<std::string> tokens = tokenize(data,sep);
	std::string url = geturl(data,tokens);
	host = gethostname(data,tokens);
	if(!host.compare("-1")){
		pos = url.find("http://",0);
		if(pos!=std::string::npos){
			host = url.substr(7,(url.find("/",7)-7));
		}else{
			return ; 
		}
	}
	pos = url.find("http://",0);
	if(pos!=std::string::npos){
		r_url = url.substr(url.find("/",7));
	}else{
		r_url = url;
	}
	pos = host.find(":",0);
	if(pos!=std::string::npos){
		port = atoi(host.substr(pos+1).c_str());
		host = host.substr(0,pos);
	}else{
		 port = 80;
	}
}

int isvalidheader(std::string data){
	return 200;
}

std::string convertheader(std::string hdr)
{

  std::string ret="";
  std::string host;
  std::vector <std::string> tokens;
  tokens =tokenize(hdr,  "\r\n");

  std::vector<std::string>::iterator it;
  for( it = tokens.begin(); it != tokens.end(); ++it)
  {

    if( it->find("Host:", 0) == 0 )
    {
      host = it->substr(6);
    }
    
  }
  
  

  for( it = tokens.begin(); it != tokens.end(); ++it)
  {
    if( it->find( "GET", 0) == 0)
    {

      std::string temp = *it;
      host = "http://" + host;
      int location = temp.find(host,0);
      int length = host.length();
      temp = temp.replace( location, length, "" );
      ret += temp + "\r\n";

    }
    else if( it->find( "Proxy", 0) != 0 )
    {
      ret += *it + "\r\n";
    }
  }
  ret += "\r\n";
  
  return ret;
}
