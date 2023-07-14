#include "webserv.hpp"
#define PORT 8087
#include "ServerData.hpp"
#include "LocationData.hpp"
#include "parseConfig.hpp"

void req::get_matched(char **av)
{
	int dif;
	std::vector<ws::ServerData> servers;
	servers = ws::parseConfigFile(av[1]);
	for (std::vector <ws::ServerData>:: iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::map<std::string, ws::LocationData>	locations = it->getLocations();
		for (std::map<std::string, ws::LocationData>::iterator it2 = locations.begin(); it2 != locations.end(); it2++)
		{
			
		}
	}
};
int main(int ac, char **av)
{
	if (ac == 2)
	{
		int server_fd, new_socket;
		long valread;
		req rq;
		rq.status = 200;
		std::fstream file("request.txt",  std::ios::in | std::ios::out | std::ios::trunc);
		struct sockaddr_in address;
		int addrlen = sizeof(address);
		std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		{
			perror("In socket");
			exit(EXIT_FAILURE);
		}
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
				rq.set_init();
			}
			if (file.is_open())
				file.write(&rq.body[0], rq.body.size());
			break;
		}
		rq.get_matched(av);
		// if (rq.method == 2 && rq.status == 200)
		// {
		// 	if (check_upload_support(this->final_path))
		// 		rq.upload(file);
		// 	else
		// 		rq.post();
		// }
		// else
		// 	std::cout << rq.status << std::endl;
		// }
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return 0;
}