#include "webserv.hpp"
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
}

void req::fill_body(int k, int j, std::fstream& file)
{
	int i = 0;
	std::string output;
	std::string body;
	if (!file.is_open())
		this->error();
	file.seekg(0);
	while(i <= j)
	{
		getline(file, output);
		std::cout << output ;
		i++;
	}
	if (k == 1)
	{
		std::vector<std::string> lines;
		std::string line;
		std::ofstream image;
		image.open("image.mp4");
		char c;
		while (file.get(c))
			this->last_body.push_back(c);
		file.close();
		for (size_t i = 0; i < this->last_body.size(); ++i) {
				image << this->last_body[i];
			}
		image.close();
	}
	else if (k == 2)
	{
		std::vector<std::string> lines;
		std::string line;
		std::ofstream image;
		image.open("image.png");
		char c;
		while (file.get(c))
			this->last_body.push_back(c);
		file.close();
		for (size_t i = 0; i < this->last_body.size(); ++i) {
				image << this->last_body[i];
			}
		image.close();
	}
	else if(k == 3)
	{
		std::vector<std::string> lines;
		std::string line;
		std::ofstream image;
		image.open(this->header_map["POST"]);
		char c;
		while (file.get(c))
			this->last_body.push_back(c);
		file.close();
		for (size_t i = 0; i < this->last_body.size(); ++i) {
				image << this->last_body[i];
			}
		image.close();
	}
};