#include "webserv.hpp"
#define PORT 8087


void req::parse_header(std::string body)
{
	int j = 0;
	int first = 0;
	std::string key;
	std::string value;
	while (body[j])
	{
		if(body[j] == '\r' && body[j + 1] == '\n' && body[j + 2] == '\r' && body[j + 3] == '\n')
			break;
		while (body[j] != ' ' && body[j] != ':')
		{
			key.push_back(body[j]);
			j++;
		}
		if (first == 0)
		{
			if (!(key.compare("GET")))
				this->method = 1;
			else if (!(key.compare("POST")))
				this->method = 2;
			else if (!(key.compare("DELETE")))
				this->method = 3;
			else
			{
				this->status = 405;
				this->error();
			}
			first = 1;
		}
		while (body[j + 1] != '\r')
		{
			value.push_back(body[j + 2]);
			j++;
		}
		this->key = key;
		this->value = value;
		this->header_map[this->key] = this->value;
		while (key.size() > 0)
			key.pop_back();
		while (value.size() > 0)
			value.pop_back();
		j++;
	}
	this->check_errors();
};

int req::check_rn(std::string body)
{
	int j = 0;
	while (body[j])
	{
		if(body[j] == '\r' && body[j + 1] == '\n' && body[j + 2] == '\r' && body[j + 3] == '\n')
			return 1;
		j++;
	}
	return 0;
};

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
					if (check_rn(body))
					{
						rq.parse_header(body);
						// if (rq.body_kind == 1)
						// {
							// if (wait_for_zero(body))
								// this->init = 1;
							// else
								// this->init = 0;
						// }
						// else if (rq.body_kind == 2)
						// {
							// if (wait_for_rn(body))
								// this->init = 1;
							// else
								// this->init = 0;
						// }
					}
					break;
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