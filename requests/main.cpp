#include "webserv.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		if (!std::strcmp(av[1], "request"))
		{
			req rq;
			rq.parse_request_head("request");
		}
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return (0);
}