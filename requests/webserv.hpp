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
#include "ServerData.hpp"

class req
{
	private:
		typedef	std::map<std::string, ws::LocationData>		myMap;
		int method;
		int status ;
		int body_kind;
		int init;
		std::string		host;
		size_t			port;
		std::string		serverName;
		size_t			bodySizeLimit;
		std::string		defaultErrorPages;
		myMap			locations;
		std::string		path;
	public:
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
		std::string get_host(){return this->host;};
		std::string get_servername(){return this->serverName;};
		std::string get_defaulterror(){return this->defaultErrorPages;};
		std::string get_path(){return this->path;};
		myMap get_locations(){myMap map; return map;};
		size_t get_port(){return this->port;};
		size_t get_bodysizelimit(){return this->bodySizeLimit;};
		//#################################
		req(void) : method(2), status(200), body_kind(1), init(0) {};
		req(ws::ServerData &s);
		~req() {};
		void post();
		void check_errors();
		void parse_request_head(std::fstream& file, int *j);
		void parse_header(std::string body);
		void set_inittt();
		void get_matched(char **av);
		void error();
		void upload(std::fstream& file);
		bool file_exists(const std::string& filePath);
		std::string location;
		std::map<std::string, std::string> header_map;
		std::map<std::string, std::string> body_map;
		std::string key;
		std::string value;
		std::vector<char> last_body;
		std::string extention;
		std::string body;
		std::string final_path;
		int not_allowed_char(std::string uri);
		int check_rn(std::string body);
		int wait_for_zero(std::string body);
		int wait_for_size(std::string body);
		int flag;
		int delim;
};

#endif