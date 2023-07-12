#include "webserv.hpp"
#define PORT 8087

int req::wait_for_zero(std::string body)
{
	int j = 0;
	while (body[j])
	{
		if(body[j] == '0'  && body[j + 1] == '\r' && body[j + 2] == '\n' && body[j + 3] == '\r' && body[j + 4] == '\n')
			return 1;
		j++;
	}
	return (0);
};

int req::wait_for_size(std::string body)
{
	int j = this->delim;
	while(body[j])
		j++;
	std::cout << j << "<---"<< std::endl;
	std::cout << stoi(this->header_map["\rContent-Length"]) << std::endl;
	if (j >= stoi(this->header_map["\rContent-Length"]))
		return 1;
	else
		return 0;
};
int main(int ac, char **av)
{
	if (ac == 2)
	{
		int k = 0;
		if (!std::strcmp(av[1], "request.txt"))
		{
			std::string body;
			int server_fd, new_socket;
			long valread;
			req rq;
			//int j = 0;
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
				//std::vector<char> body;
				while (valread > 0)
				{
					valread = read(new_socket, buffer, 1024);
					for (int i = 0; i < valread; ++i)
						body.push_back(buffer[i]);
					if (rq.check_rn(body))
					{
						if (k == 0)
						{
							rq.parse_header(body);
							k = 1;
							if (rq.status != 200)
								rq.init = -1;
						}
					}
					if (rq.body_kind == 2 && rq.init != -1)
					{
						std::cout << rq.wait_for_zero(body) << std::endl;
						if(!rq.wait_for_zero(body))
							rq.init = 0;
						else
							rq.init = 1;
					}
					else if (rq.body_kind == 1)
					{
						if (rq.wait_for_size(body))
						{
							printf("m here\n");
							break;
							rq.init = 1;
						}
						else
							rq.init = 0;
					}
				}
				if (file.is_open())
					file.write(&body[0], body.size());
				break;
			}
			// rq.parse_request_head(file, &j);
			// if (rq.method == 2 && rq.status == 200)
				// rq.upload(rq.body_kind, j, file);
			// else
				// std::cout << rq.status << std::endl;
		}
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return 0;
}