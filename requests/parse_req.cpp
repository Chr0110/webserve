#include "webserv.hpp"
req::req(ws::ServerData &s) : method(2), status(200), body_kind(1), init(0)
{
 	this->host = s.getHost();
 	this->port = s.getPort();
 	this->serverName = s.getServerName();
 	this->bodySizeLimit = s.getBodySizeLimit();
 	this->defaultErrorPages = s.getDefaultErrorPages();
 	this->locations = s.getLocations();
 	this->path = s.getPath();
}

void req::error()
{
	std::cout << "Error" << this->status << std::endl;
	exit(0);
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
		this->set_status(414);
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
			if (body[j] == '\n')
				j++;
			key.push_back(body[j]);
			j++;
		}
		if (first == 0)
		{
			if (!(key.compare("GET")))
				this->set_method(1);
			else if (!(key.compare("POST")))
				this->set_method(2);
			else if (!(key.compare("DELETE")))
				this->set_method(3);
			else
				this->set_status(405);
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
	this->delim = j;
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

void req::check_errors()
{
	if (this->get_method() == 1)
	{
		int i = 0;
		std::string location1 = this->header_map["GET"].c_str();
		while (location1[i] != ' ')
			i++;
		this->location = location1.substr(0, i);
		if (this->not_allowed_char(this->header_map["GET"].c_str()))
			this->set_status(400);
	}
	if (this->get_method() == 2)
	{
		int i = 0;
		std::string location1 = this->header_map["POST"].c_str();
		while (location1[i] != ' ')
			i++;
		this->location = location1.substr(0, i);
		if (this->not_allowed_char(this->header_map["POST"].c_str()))
			this->set_status(400);

	}
	if (this->get_method() == 3)
	{
		int i = 0;
		std::string location1 = this->header_map["DELETE"].c_str();
		while (location1[i] != ' ')
			i++;
		this->location = location1.substr(0, i);
		if (this->not_allowed_char(this->header_map["DELETE"].c_str()))
			this->set_status(400);

	}
	{
	if (this->header_map["\rContent-Length"].size() > 0 && this->get_method() == 2)
		this->set_body_kind(2);
		this->flag = 1;
	}
	if (this->header_map["\rTransfer-Encoding"].size() > 0 && this->get_method() == 2)
	{
		if (strcmp(this->header_map["\rTransfer-Encoding"].c_str(), "chunked\r") != 0)
			this->set_status(501);
		else
		{
			this->set_body_kind(1);
			this->flag = 1;
		}
	}
	if (this->header_map["\rContent-Type"].size() > 0)
	{
		this->extention = get_extention(this->header_map["\rContent-Type"].c_str());
		this->flag = 1;
	}
	if (this->flag != 1)
		this->set_status(404);
};
