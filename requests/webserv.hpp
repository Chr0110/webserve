#ifndef WEBSERV
#define WEBSERV

#include <iostream>
#include <map>
#include <cstring>
#include <fstream>
#include <string>
#include <locale>
#include <cstdlib>
#include <sys/stat.h>
#include <vector>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <cmath>

class req
{
	public:
		void check_errors();
		std::map<std::string, std::string> header_map;
		std::map<std::string, std::string> body_map;
		void parse_request_head(std::fstream& file, int *j);
		int not_allowed_char(std::string uri);
		void error();
		void post(int k, int j, std::fstream& file);
		std::string key;
		std::string value;
		int method;
		int status ;
		int flag;
		int body_kind;
		std::string location;
		bool file_exists(const std::string& filePath);
		std::vector<char> last_body;
		std::string extention;
};

#endif