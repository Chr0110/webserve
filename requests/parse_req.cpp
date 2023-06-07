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

void req::fill_body(int k, int j)
{
	int i = 0;
	std::string output;
	std::ifstream inputFile("request.txt");
	std::string body;
	if (!inputFile.is_open())
		this->error();
	while(i < j)
	{
		getline(inputFile, output);
		i++;
	}
	i = 0;
	if (k == 1)
	{
		std::string fileContent;
		std::string line;
		if (inputFile.is_open())
		{
			while (std::getline(inputFile, line)) {
				fileContent += line + "\n";
		}
		inputFile.close();
		std::cout <<"-> "<< fileContent << " <-" << std::endl;
	}
	}
	else if (k == 2)
	{
		std::string fileContent	;
		if (inputFile.is_open())
		{
			char c;
			while (inputFile.get(c)) {
				fileContent += c;
		}
		inputFile.close();
	}
	std::cout <<"-> "<< fileContent << " <-" << std::endl;
	}
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
			this->check_errors();
			if (this->method == 2)
				this->fill_body(this->body_kind, j);
			break;
		}
		i = 0;
		j++;
		while(output[i] != ' ' && output[i] != ':')
			i++;
		this->key = output.substr(0, i);
		this->key = remove_quotes(this->key);
		if (!(this->key.compare("GET")))
			this->method = 1;
		else if (!(this->key.compare("POST")))
			this->method = 2;
		else if (!(this->key.compare("DELETE")))
			this->method = 3;
		else if (!(this->key.compare("Transfer-Encoding")))
			this->body_kind = 1;
		else if (!(this->key.compare("Content-Length")))
			this->body_kind = 2;
		else if (!(this->key.compare("Content-Type")))
		{
			this->value = output.substr(i + 1);
			int result = this->value.compare(0, 19,"multipart/form-data", 0, 19);
			if (result == 0)
				this->body_kind = 3;
		}
		this->value = output.substr(i + 1);
		this->value = remove_quotes(this->value);
		this->header_map[this->key] = this->value;
	}
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
		{
			return 1;
		}
	}
	return 0;
};
void req::check_errors()
{
	std::map<std::string, std::string>::iterator it;
	for (it = this->header_map.begin(); it != this->header_map.end(); ++it)
	{
		if (!(it->first.compare("Transfer-Encoding")) && it->second.compare("chunked"))
		{
			this->status = 501;
			this->error();
		}
		else if ((!(it->first.compare("Transfer-Encoding"))) && this->method == 2)
			this->flag = 1;
		else if (!(it->first.compare("Content-Length") && this->method == 2))
			this->flag = 1;
		else if (this->method == 2)
		{
			if (!(it->first.compare("POST")))
			{
				if (this->not_allowed_char(it->second))
				{
					this->status = 400;
					this->error();
				}
			}
			else if (!(it->first.compare("GET")))
			{
				if (this->not_allowed_char(it->second))
				{
					this->status = 400;
					this->error();
				}
			}
			else if (!(it->first.compare("DELETE")))
			{
				if (this->not_allowed_char(it->second))
				{
					this->status = 400;
					this->error();
				}
			}
		}
	}
	if (this->flag != 1)
	{
		this->status = 400;
		this->error();
	}
};