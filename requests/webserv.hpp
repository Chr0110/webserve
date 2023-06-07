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
		void check_errors();
		std::map<std::string, std::string> header_map;
		std::map<std::string, std::string> body_map;
		void parse_request_head(std::string name);
		int not_allowed_char(std::string uri);
		void error();
		void fill_body(int k, int j);
		std::string key;
		std::string value;
		int method;
		int status;
		int flag;
		int body_kind;

};

#endif