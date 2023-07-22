#include "webserv.hpp"

bool isDirectory(const std::string& path) {
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == -1) {
        return false;
    }

    return S_ISDIR(fileStat.st_mode);
}

bool hasIndexFile(const char* directoryPath) {
	DIR* directory = opendir(directoryPath);
	if (directory == NULL) {
		std::cerr << "Failed to open directory: " << directoryPath << std::endl;
		return false;
	}

	struct dirent* entry;
	while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, "index.py") == 0 || strcmp(entry->d_name, "index.php") == 0) {
			closedir(directory);
			return true;
		}
	}

	closedir(directory);
	return false;
};

void req::post()
{
	try
	{
		if (isDirectory(final_path.c_str()))
		{
			std::cout << final_path[this->final_path.size()] << std::endl;
			if (this->final_path[this->final_path.size() - 1] != '/')
			{
				this->set_status(301);
				return ;
			}
			else if (hasIndexFile(this->final_path.c_str()))
			{
				printf("ready to send to cgi\n");
				return ;
			}
			else
			{
				this->set_status(403);
				return;
			}
		}
		else 
		{
			std::string file;
			std::string ext;
			int i = final_path.size() - 1;
			while (i > 0)
			{
				if (final_path[i] == '/')
					break;
				i--;
			}
			file = final_path.substr(i);
			i = 0;
			while (file[i])
			{
				if (file[i] == '.')
					break;
				i++;
			}
			ext = file.substr(i + 1);
			if (strcmp(ext.c_str(),"py") == 0 || strcmp(ext.c_str(),"php") == 0)
			{
				printf("ready to send to cgi\n");
				//send_to_cgi
			}
			else
			{
				status = 403;
				printf("error 403\n");
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "" << '\n';
	}
};