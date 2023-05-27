#ifndef WEBSERV
#define WEBSERV

#include <iostream>
#include <map>
#include <cstring>
#include <fstream>
#include <string>
#include <locale>

class req
{
	public:
		std::map<std::string, std::string> header_map;
		std::map<std::string, std::string> body_map;
		void parse_request_head(std::string name);
		void parse_request_body(std::string name, int line);
		int there_is_bracket(std::string output);
		void error();
		std::string key;
		std::string value;
};

#endif