#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		if (!std::strcmp(av[1], "input.txt"))
		{
			Btc btc;
			btc.search_value();
		}
	}
	else
		std::cout << "Error: could not open file" << std::endl;
	return (0);
}