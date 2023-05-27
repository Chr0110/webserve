#include "webserv.hpp"

void req::error()
{
	std::cout << "Error\n" << std::endl;
	exit(0);
};

void req::parse_request_head(std::string name)
{
	int i = 0;
	int j = 0;
	std::string output;
	std::ifstream inputFile(name);
	if (!inputFile.is_open())
		this->error();
	while(getline(inputFile, output))
	{
		if (!output.length())
		{
			this->parse_request_body(name, j);
			break;
		}
		i = 0;
		j++;
		while(output[i] != ' ' && output[i] != ':')
			i++;
		this->key = output.substr(0, i);
		this->value = output.substr(i + 1);
		this->header_map[this->key] = this->value;
	}
};

int req::there_is_bracket(std::string output)
{
	int i = 0;
	while (output[i])
	{
		if (output[i] == '}' || output[i] == '{')
			return 1;
		i++;
	}
	return 0;
};

void req::parse_request_body(std::string name, int line)
{
	int i = -1;
	int j;
	std::string output;
	std::ifstream inputFile(name);
	if (!inputFile.is_open())
		this->error();
	while (++i < line + 2)
		getline(inputFile, output);
	while(getline(inputFile, output))
	{
		i = 0;
		j = 0;
		if (!this->there_is_bracket(output))
		{
			while(output[i] == ' ')
				i++;
			while (output[j] != ':')
				j++;
			this->key = output.substr(i, j - i);
			this->value = output.substr(j + 2);
			this->body_map[this->key] = this->value;
		}
	}
}
	