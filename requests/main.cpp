#include "webserv.hpp"
#define PORT 8087


int main(int ac, char **av)
{
	if (ac == 2)
	{
		if (!std::strcmp(av[1], "request.txt"))
		{
			std::string body;
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
				char buffer[300000] = {0};
				valread = 1;
				std::string b;
				std::vector<char> body;
				while (valread > 0) {
					valread = read(new_socket, buffer, 300000);
					for (int i = 0; i < valread; ++i)
						body.push_back(buffer[i]);
				}
				// parse_head(body);
				if (file.is_open())
					file.write(&body[0], body.size());
				break;
			}
			rq.parse_request_head(file);
			if (rq.status == 200)
				printf("no error\n");
			else
				printf("there is an error %d\n", rq.status);
		}
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return 0;
}