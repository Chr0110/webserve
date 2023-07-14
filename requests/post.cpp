#include "webserv.hpp"

bool isDirectory(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return false;
};

bool hasIndexFile(const char* directoryPath) {
    DIR* directory = opendir(directoryPath);
    if (directory == NULL) {
        std::cerr << "Failed to open directory: " << directoryPath << std::endl;
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, "index.py") == 0 || strcmp(entry->d_name, "index.php"))
            closedir(directory);
            return true;
    }

    closedir(directory);
    return false;
};

void req::post()
{
	if (isDirectory(this->final_path))
	{
		if (this->final_path[this->final_path.size() - 1] != '/')
			this->status = 301;
		else if (hasIndexFile(this->final_path))
			send_to_cgi();
		else
			this->status = 403;
	}
	else if (isfile(this->final_path))
	{
		if (this_file_cgi())
			send_to_cgi();
		else
			this->status = 403;
	}
	else
		this->status = 404;
};