#include "webserv.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		if (!std::strcmp(av[1], "request.txt"))
		{
			req rq;
			rq.parse_request_head("request.txt");
			// rq.check_errors();
		}
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return (0);
}