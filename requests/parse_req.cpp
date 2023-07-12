#include "webserv.hpp"

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

std::string get_extention(std::string s)
{
	int i = 0;
	std::string ext;
	while (s[i] != '/')
		i++;
	ext = s.substr(i + 1, s.size());
	return (ext);
};

void req::check_errors()
{
	int i = 0;
	std::map<std::string, std::string>::iterator it;
	for (it = this->header_map.begin(); it != this->header_map.end(); ++it)
	{
		if ((!(it->first.compare("Transfer-Encoding"))) && this->method == 2)
		{
			if (it->second.compare("chunked"))
			{
				this->body_kind = 1;
				this->flag = 1;
			}
			else
				this->status = 501;
		}
		else if (!(it->first.compare("Content-Length") && this->method == 2))
		{
			this->body_kind = 2;
			this->flag = 1;
		}
		else if (!(it->first.compare("Content-Type") && this->method == 2))
		{
			this->extention = get_extention(it->second);
			this->flag = 1;
		}
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
