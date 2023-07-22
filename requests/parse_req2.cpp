#include "webserv.hpp"


std::string req::compare(std::string s)
{
	if (this->location.size() == 1)
	{
		this->compaireFlag = 1;
		return this->location;
	}
	int i = s.size();
	if (this->location[i] != '\0' && this->location[i] != '/')
		return std::string();
	if (i != 1)
	{
		int o = 0;
		o = s.substr(0, i).compare(this->location.substr(0, i));
		if (o == 0)
		{
			std::string path = this->location.substr(i);
			this->compaireFlag = 1;
			return path;
		}
	}
	return std::string();
};

void req::get_matched()
{

	const std::map<std::string, ws::LocationData>& locations = server.getLocations();
	for (std::map<std::string, ws::LocationData>::const_iterator it2 = locations.begin(); it2 != locations.end(); ++it2)
	{
		compare(it2->first).size();
		std::string root = it2->second.getRoot();
		if (this->compaireFlag == 1)
		{
			if (root[root.size() - 1] == '/')
				root = root.substr(0, root.size() - 1);
			this->final_path = root + compare(it2->first);
			myLocation = it2->second;
			// std::cout << final_path << std::endl;
			return ;
		}
	}
	if (this->final_path.size() == 0)
	{
		std::cout << location << std::endl;
		std::cout << "not found 404\n";
	}
};

std::string req::generateName() {
    const std::string vowels = "aeiou";
    const std::string consonants = "bcdfghjklmnpqrstvwxyz";
    std::string name;
    
    srand(time(0)); 
    int nameLength = rand() % 5 + 1;
    
    for (int i = 0; i < nameLength; ++i) {
        if (i % 2 == 0) {
            name += vowels[rand() % vowels.size()];
        } else { 
            name += consonants[rand() % consonants.size()];
        }
    }
    
    return name;
};

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


int req::check_rn2()
{
	int j = 0;
	while (body[j])
	{
		if(body[j] == '\r' && body[j + 1] == '\n' && body[j + 2] == '\r' && body[j + 3] == '\n')
			return 1;
		j++;
	}
	return 0;
}
void	req::set_inittt()
{
	if (flag2 == 0)
	{
		if (check_rn(body))
		{
			parse_header(body);
			stopRn += 4;
			flag2 = 1;
			if (status != 200)
				set_init(-1);
		}
	}
	if (get_body_kind()== 1 && init != -1 && get_method() == 2)
	{
		usleep(200);
		if(wait_for_zero(body))
			set_init(1);
	}
	else if (get_body_kind() == 2 && get_method() == 2)
	{
		if (wait_for_size(body))
			set_init(1);
	}
	if ((init != -1 && get_method() == 1))
	{
		if (check_rn2())
			set_init(1);
	}
	else if ((init != -1 && this->get_method() == 3))
	{
		if (check_rn2())
			set_init(1);
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

int req::hexToDecimal(const std::string& hexStr) {

	if (!strcmp(hexStr.c_str(), "0\r\n\r\n"))
		return (0);
    int decimalValue = 0;
    int base = 1;

    for (int i = hexStr.length() - 1; i >= 0; i--) {
        char digit = hexStr[i];

        int digitValue;
        if (digit >= '0' && digit <= '9') {
            digitValue = digit - '0';
        } else if (digit >= 'A' && digit <= 'F') {
            digitValue = digit - 'A' + 10;
        } else if (digit >= 'a' && digit <= 'f') {
            digitValue = digit - 'a' + 10;
        } else {
            return 0;
        }
        decimalValue += digitValue * base;
        base *= 16;
    }
    return decimalValue;
};


std::string req::to_rn(std::string body)
{
	int i = 0;
	std::string hexa;
	while (body[i] != '\r' && body[i + 1] != '\n')
	{
		hexa.push_back(body[i]);
		i++;
	}
	return hexa;
};

void req::upload1(std::ofstream &filee)
{
	int tmp_stop_rn, chunk_size, x;
	int i = 0;
	if (file_cr == 0)
	{
		filee.open("../uploads/" + generateName() + "." + extention);
		file_cr++;
	}
	tmp_stop_rn = stopRn;
	while (true) {
		std::string hexa = to_rn(&body[stopRn]);
		x = stopRn;
		tmp_stop_rn += hexa.size() + 2;
		chunk_size = hexToDecimal(hexa);
		for (i = 0; i < chunk_size && (tmp_stop_rn + i) < (int)body.size();) {
			i++;
		}
		if (i != chunk_size) {
			body = body.substr(x, body.size()-x);
			stopRn = 0;
			break;
		}
		for (i = 0; i < chunk_size && (tmp_stop_rn + i) < (int)body.size(); i++) {
			filee << body[tmp_stop_rn + i];
		}
		if (!chunk_size) {
			break;
		}
		tmp_stop_rn += chunk_size + 2;
		stopRn = tmp_stop_rn;
	}
	return ;
};

void req::upload2(std::ofstream &filee)
{
	if (file_cr == 0)
	{
		filee.open("../uploads/" + generateName() + "." + extention);
		file_cr++;
	}
	while ((size_t)stopRn != body.size())
	{
		filee << body[stopRn];
		last_body.push_back(body[stopRn]);
		stopRn++;
	}
};