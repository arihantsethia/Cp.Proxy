#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

std::vector<std::string> tokenize(std::string, std::string);
std::string gethostname(std::string);
std::string convertheader(std::string, std::string, std::string, int);
int hostlookup(std::string h);
int parseheader(std::string, std::string&, std::string&, int&);

#endif