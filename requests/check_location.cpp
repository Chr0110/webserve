#include "webserv.hpp"
bool req::file_exists(const std::string &filePath)
{
	struct stat fileInfo;
	return stat(filePath.c_str(), &fileInfo) == 0;
};
