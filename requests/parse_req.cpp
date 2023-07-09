#include "webserv.hpp"

void req::error()
{
	std::cout << "Error" << std::endl;
	exit(0);
};

std::string remove_quotes(std::string string)
{
	size_t start = string.find_first_not_of(" , \"");
	size_t end = string.find_last_not_of(" , \"");

	if (start == std::string::npos || end == std::string::npos)
		return "";

	return string.substr(start, end - start + 1);
};

void req::parse_request_head(std::fstream& file, int *j)
{
	int i = 0;
	int first = 0;
	std::string output;
	if (!file.is_open())
		this->error();
	file.seekg(0);
	while(getline(file, output) && output.length() > 1)
	{
		i = 0;
		(*j)++;
		while(output[i] != ' ' && output[i] != ':')
			i++;
		this->key = output.substr(0, i);
		this->key = remove_quotes(this->key);
		if (first == 0)
		{
			if (!(this->key.compare("GET")))
				this->method = 1;
			else if (!(this->key.compare("POST")))
				this->method = 2;
			else if (!(this->key.compare("DELETE")))
				this->method = 3;
			else
			{
				this->status = 405;
				this->error();
			}
			first = 1;
		}
		else if (!(this->key.compare("Transfer-Encoding")))
		{
			this->value = output.substr(i + 1);
			int result = this->value.compare(0, 7," chunked", 7);
			if (result == 0)
				this->body_kind = 1;
		}
		else if (!(this->key.compare("Content-Length")))
			this->body_kind = 2;
		else if (!(this->key.compare("Content-Type")))
		{
			this->value = output.substr(i + 1);
			int result = this->value.compare(0, 19," multipart/form-data", 19);
			if (result == 0)
				this->body_kind = 3;
		}
		try
		{
			this->value = output.substr(i + 1);
		}
		catch(const std::exception& e)
		{
			std::cerr << "";
		}
		this->value = remove_quotes(this->value);//in this step i fill up the header key and value in map;
		this->header_map[this->key] = this->value;//in this step i fill up the header key and value in map;
	}
	this->check_errors();
};

int req::not_allowed_char(std::string uri)
{
	int i = 0;
	while(uri[i])
	{
		if (uri[i] == ' ' && uri[i + 1] && uri[i + 1] == 'H')
			break;
		else
			i++;
	}
	uri = uri.substr(0, i);
	if (uri.length() >= 2048)
	{
		this->status = 414;
		this->error();
	}
	const std::string allowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=";
	for (std::string::const_iterator it = uri.begin(); it != uri.end(); ++it) 
	{
		if (allowedCharacters.find(*it) == std::string::npos)
			return 1;
	}
	return 0;
};
void req::check_errors()
{
	int i = 0;
	std::map<std::string, std::string>::iterator it;
	for (it = this->header_map.begin(); it != this->header_map.end(); ++it)
	{
		if ((!(it->first.compare("Transfer-Encoding"))) && this->method == 2)
			this->flag = 1;
		else if (!(it->first.compare("Content-Length") && this->method == 2))
			this->flag = 1;
		else if (!(it->first.compare("Content-Type") && this->method == 2))
			this->flag = 1;
		else if (!(it->first.compare("POST")))
		{
			i = 0;
			while(it->second[i] != ' ')
				i++;
			this->location = it->second.substr(0, i);
			if (this->not_allowed_char(it->second))
			{
				this->status = 400;
				this->error();
			}
		}
		else if (!(it->first.compare("GET")))
		{
			i = 0;
			while(it->second[i] != ' ')
				i++;
			this->location = it->second.substr(0, i);
			if (this->not_allowed_char(it->second))
			{
				this->status = 400;
				this->error();
			}
		}
		else if (!(it->first.compare("DELETE")))
		{
			i = 0;
			while(it->second[i] != ' ')
				i++;
			this->location = it->second.substr(0, i);
			if (this->not_allowed_char(it->second))
			{
				this->status = 400;
				this->error();
			}
		}
	}
	if (this->flag != 1)
	{
		this->status = 400;
		this->error();
	}
};