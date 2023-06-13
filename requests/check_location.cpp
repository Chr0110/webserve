#include "webserv.hpp"
bool fileExists(const std::string& filePath)
{
	struct stat fileInfo;
	return stat(filePath.c_str(), &fileInfo) == 0;
}
void    req::check_location()
{
	int i = 0;
	if (this->method == 1)
	{
		while(this->header_map["GET"][i])
		{
			if (this->header_map["GET"][i] == 'H' && this->header_map["GET"][i + 1] && this->header_map["GET"][i + 1] == 'T')
				break;
			i++;
		}
		std::string location2 = this->header_map["GET"].substr(0, i - 1);
		if (fileExists(location2))
			std::cout << "File exists!" << std::endl;
		else
			std::cout << "File does not exist." << std::endl;
	}
	//####################################################################################################
	else if (this->method == 2)
	{
		while(this->header_map["POST"][i])
		{
			if (this->header_map["POST"][i] == 'H' && this->header_map["POST"][i + 1] && this->header_map["POST"][i + 1] == 'T')
				break;
			i++;
		}
		std::string location2 = this->header_map["POST"].substr(0, i - 1);
		if (fileExists(location2))
			std::cout << "" << std::endl;
		else
			std::cout << "File does not exist." << std::endl;
	}
	//####################################################################################################
	else if (this->method == 3)
	{
		while(this->header_map["DELETE"][i])
		{
			if (this->header_map["DELETE"][i] == 'H' && this->header_map["DELETE"][i + 1] && this->header_map["DELETE"][i + 1] == 'T')
				break;
			i++;
		}	
		std::string location2 = this->header_map["DELETE"].substr(0, i - 1);
		if (fileExists(location2))
			std::cout << "File exists!" << std::endl;
		else
			std::cout << "File does not exist." << std::endl;
	}
}