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

    return -1; // Invalid hex character
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
	i = 0;
	if (k == 1)
	{
		size_t j = 0;
		std::vector<std::string> lines;
		std::string line;
		std::ofstream image;
		image.open("imagee.png");
		if (file.is_open())
		{
			while (getline(file, line))
			{
				if (hexToDigit(line) == -1)
					lines.push_back(line);
			}
			file.close();
			while(j < lines.size())
			{
				image << lines[j];
				j++;
			}
			file.close();
			image.close();
		}
	}
	else if (k == 2)
	{
		size_t j = 0;
		std::vector<std::string> lines;
		std::string line;
		std::ofstream image;
		image.open("image.png");
		if (file.is_open())
		{
			while (getline(file, line))
				lines.push_back(line);
		}
		file.close();
		while(j < lines.size())
		{
			image << lines[j];
			j++;
		}
	}
	else if(k == 3)
	{
		int i = 0;
		std::string fileContent;
		std::string line;
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				i = 0;
				if (the_end(line))
					break;
				int result = line.compare(0, 4,"-------", 0, 4);
				if (result != 0)
						fileContent += line + "\n";
				else
				{
					while(i < 2)
					{
						std::getline(file, line);
						i++;
					}
				}
			}
		}
		file.close();
		// std::cout <<"-> "<< fileContent << " <-" << std::endl;
	}
};