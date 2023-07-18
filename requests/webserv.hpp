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
#include <sys/stat.h>
#include <dirent.h>
#include "ServerData.hpp"

class req
{
	private:
		typedef	std::map<std::string, ws::LocationData>		myMap;
		int method;
		int status ;
		int body_kind;
		int init;
		ws::ServerData		&server;

	public:
		req(ws::ServerData &s) : method(2), status(200), body_kind(1), init(0), server(s) {};
		// req();
		~req() {};
		//setters
		void set_method(int method){this->method = method;};
		void set_status(int status){this->status = status;};
		void set_body_kind(int body_kind){this->body_kind = body_kind;};
		void set_init(int init){this->init = init;};
		//getters
		int get_status(){return this->status;};
		int get_init(){return this->init;};
		int get_method(){return this->method;};
		int get_body_kind(){return this->body_kind;};
		//#################################
		ws::LocationData myLocation;
		void post();
		void check_errors();
		void parse_header(std::string body);
		void set_inittt();
		void get_matched();
		void error();
		void upload(std::fstream& file);
		std::string location;
		std::map<std::string, std::string> header_map;
		std::map<std::string, std::string> body_map;
		std::string key;
		std::string value;
		std::vector<char> last_body;
		std::string extention;
		std::string body;
		std::string final_path;
		std::string compare(std::string s);
		std::string generateName();
		int not_allowed_char(std::string uri);
		int check_rn(std::string body);
		int wait_for_zero(std::string body);
		int wait_for_size(std::string body);
		int flag;
		int delim;
		int compaireFlag;
};

#endif