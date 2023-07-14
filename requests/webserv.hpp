#ifndef WEBSERV
#define WEBSERV
# include "LocationData.hpp"
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
		void post();
		void check_errors();
		std::map<std::string, std::string> header_map;
		std::map<std::string, std::string> body_map;
		void parse_request_head(std::fstream& file, int *j);
		void parse_header(std::string body);
		int not_allowed_char(std::string uri);
		void error();
		void upload(std::fstream& file);
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
		int init;
		int check_rn(std::string body);
		int wait_for_zero(std::string body);
		int wait_for_size(std::string body);
		std::string body;
		void set_init();
		int delim;
		int delim2;
		std::string final_path;
		void get_matched(char **av);
};

#endif