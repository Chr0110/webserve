#include "webserv.hpp"

int req::wait_for_zero(std::string body)
{
	int i = body.size();
	if (body[i - 1] == '\n' && body[i - 2] == '\r' && body[i - 3] == '\n' && body[i - 4] == '\r' && body[i - 5] == '0')
		return 1;
	return 0;
};

int req::wait_for_size(std::string body)
{
	int j = this->delim;
	std::string body2 = body.substr(j);
	try
	{
		if (body2.length() >= (unsigned long)stoi(this->header_map["\rContent-Length"]))
			return 1;
	}
	catch(const std::exception& e)
	{
		std::cerr << "" << '\n';
	}
	return 0;
};

void	req::set_init()
{
	int k = 0;
	if (this->check_rn(this->body))
	{
		if (k == 0)
		{
			this->parse_header(this->body);
			k = 1;
			if (this->status != 200)
				this->init = -1;
		}
	}
	if (this->body_kind == 2 && this->init != -1)
	{
		if(this->wait_for_zero(this->body))
			this->init = 1;
	}
	else if (this->body_kind == 1)
	{
		if (this->wait_for_size(this->body))
			this->init = 1;
	}
};

int the_end(std::string line)
{
	int i = line.length();
	if (line[i] == '-' && line[i - 1] == '-')
		return 1;
	return 0;
};
int hexToDigit(const std::string& hexChar) {
	if (hexChar >= "0" && hexChar <= "9") {
		return hexChar[0] - '0';
	} else if (hexChar >= "A" && hexChar <= "F") {
		return hexChar[0] - 'A' + 10;
	} else if (hexChar >= "a" && hexChar <= "f") {
		return hexChar[0] - 'a' + 10;
	}

	return -1;
};

int hexToDecimal(std::string& hexString) {
    int decimal = 0;
    int power = 0;

    for (int i = hexString.length() - 2; i >= 0; i--) {
        char currentChar = hexString[i];
    	int digitValue;

        if (currentChar >= '0' && currentChar <= '9') {
            digitValue = currentChar - '0';
        }
        else if (currentChar >= 'A' && currentChar <= 'F') {
            digitValue = currentChar - 'A' + 10;
        }
        else if (currentChar >= 'a' && currentChar <= 'f') {
            digitValue = currentChar - 'a' + 10;
        }
        else {
            std::cerr << "Invalid hexadecimal digit: " << currentChar << std::endl;
            return 0;
        }
        decimal += digitValue * static_cast<int>(std::pow(16, power));
        power++;
    }
    return decimal;
};

void req::upload(std::fstream& file)
{
	int i = 0;
	std::string output;
	std::string body;
	if (!file.is_open())
		this->error();
	file.seekg(0);
	while(i < (int)this->header_map.size())
	{
		getline(file, output);
		i++;
	}
	if (this->body_kind == 1)
	{
		int length = 1;
		char c;
		std::string output;
		std::ofstream filee;
		std::string kk = "file.";
		kk += this->extention;
		filee.open("../uploads/" + kk);
		while(length != 0)
		{
			try
			{
				getline(file, output);
				int length = hexToDecimal(output);
				if (length == 0)
					break;
				for(int i = 0; i < length; i++)
				{
					file.get(c);
					this->last_body.push_back(c);
				}
				file.get(c);
				file.get(c);
			}
			catch(const std::exception& e)
			{
				std::cerr << "" << '\n';
			}
			
		}
		file.close();
		for (size_t i = 0; i < this->last_body.size(); ++i)
			filee << this->last_body[i];
		filee.close();
		this->status = 201;
	}
	if (this->body_kind == 2)
	{
		try
		{
			size_t length = stoi(this->header_map["\rContent-Length"]);
			std::ofstream filee;
			std::string kk = "file.";
			kk += this->extention;
			filee.open("../uploads/" + kk);
			char c;
			while (file.get(c))
				this->last_body.push_back(c);
			file.close();
			for (size_t i = 0; i < length; ++i) {
					filee << this->last_body[i];
				}
			filee.close();
			this->status = 201;
		}
		catch(const std::exception& e)
		{
			std::cerr << "" << '\n';
		}
	}
};