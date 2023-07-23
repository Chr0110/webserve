#include "webserv.hpp"
#define PORT 8087
#include "ServerData.hpp"
#include "LocationData.hpp"
#include "parseConfig.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		int server_fd, new_socket;
		long valread;
		ws::ServerData server;
		std::vector<ws::ServerData> servers;
		servers = ws::parseConfigFile(av[1]);
		req rq = req(servers[0]);
		rq.set_status(200);
		std::ofstream filee;
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
			char buffer[30000] = {0};
			valread = 1;
			while (valread > 0)
			{
				valread = read(new_socket, buffer, 30000);
				for (int i = 0; i < valread; ++i)
					rq.body.push_back(buffer[i]);
				rq.set_inittt();
				if (rq.get_method() == 2 && rq.get_body_kind() == 1)
					rq.upload1(filee);
				else if (rq.get_method() == 2 && rq.get_body_kind() == 2)
					rq.upload2(filee);
				if (rq.get_init() == 1)
					break;
			}
			break;
		}
		rq.get_matched();
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return 0;
}