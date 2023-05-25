#ifndef WEBSERV
#define WEBSERV

#include <string>
#include <iostream>

class request
{
	public:
		int method_kind;
		std::string method;
		std::string host;
		std::string user_agent;
		std::string content_type;
		std::string content_lenght;
};

#endif