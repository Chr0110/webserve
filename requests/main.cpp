#include "webserv.hpp"
#define PORT 8087
#include "ServerData.hpp"
#include "LocationData.hpp"
#include "parseConfig.hpp"

std::string req::compare(std::string s)
{
	if (this->location.size() == 1)
	{
		return this->location;
	}
	int i = s.size();
	if (i != 1)
	{
		int o = 0;
		o = s.substr(0, i).compare(this->location.substr(0, i));
		if (o == 0)
		{
			std::string path = this->location.substr(i);
			return path;
		}
	}
	return std::string();
}

void req::get_matched()
{
	const std::map<std::string, ws::LocationData>& locations = server.getLocations();
	for (std::map<std::string, ws::LocationData>::const_iterator it2 = locations.begin(); it2 != locations.end(); ++it2)
	{
		if (compare(it2->first).size() != 0)
		{
			this->final_path = it2->second.getRoot() + compare(it2->first);
			std::cout << this->final_path << std::endl;
		}
	}
};
int main(int ac, char **av)
{
	if (ac == 2)
	{
		int server_fd, new_socket;
		long valread;
		ws::ServerData server;
		std::vector<ws::ServerData> servers;
		servers = ws::parseConfigFile(av[1]);
		req rq = req(servers[1]);
		rq.set_status(200);

		/////////////////////////////////////////////////////////////////////////////////////////
		std::fstream file("request.txt",  std::ios::in | std::ios::out | std::ios::trunc);
		struct sockaddr_in address;
		int addrlen = sizeof(address);
		std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
			perror("In socket");
			exit(EXIT_FAILURE);
		}
		int reuse = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);
		memset(address.sin_zero, '\0', sizeof address.sin_zero);
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			perror("In bind");
			exit(EXIT_FAILURE);
		}
		if (listen(server_fd, 10) < 0)
		{
			perror("In listen");
			exit(EXIT_FAILURE);
		}
		while (1)
		{
			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
			if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
			{
				perror("In accept");
				exit(EXIT_FAILURE);
			}
			char buffer[1024] = {0};
			valread = 1;
			std::string b;
			while (valread > 0)
			{
				valread = read(new_socket, buffer, 1024);
				for (int i = 0; i < valread; ++i)
					rq.body.push_back(buffer[i]);
				rq.set_inittt();
				if (rq.get_init() == 1)
					break;
			}
			if (file.is_open())
				file.write(&rq.body[0], rq.body.size());
			break;
		}
		rq.get_matched();
		if (rq.get_method() == 2 && rq.get_status() == 200)
		{
			//if (check_upload_support(this->final_path))
				rq.upload(file);
			//else
			//	rq.post();
		}
		else
			std::cout << rq.get_status() << std::endl;
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return 0;
}